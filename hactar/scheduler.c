// Copyright 2011-2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stdio.h>

#include <hactar/scheduler.h>
#include <hactar/hactar.h>

#define THREAD(i)           sched.threads_[i]
#define ACTIVE              sched.active_
#define NEXT                sched.next_
#define COUNT               sched.count_

static struct {
    Thread *threads_[HACTAR_N_THREADS + 1]; // thread list
    size_t active_; // active thread index
    size_t next_;   // next thread index
    size_t count_;  // above values are only valid if count > 0
#if defined(HACTAR_NEWLIB_REENT) && defined(__DYNAMIC_REENT__)
    uint8_t in_isr_;
#endif
} sched;

static uint8_t idle_stack[100];
static Thread idle_thread;

// Frequency in HZ
// Returns the actual frequency that was set (If the clock is low,
// there is a rounding error) or < 0 if setting failed.
static int32_t setSystick(uint32_t frequency)
{
    uint32_t ticks, clock;

    if(frequency == 0)
        return -1;

    clock = hactarGetSystickClock();
    ticks = clock / frequency;

    // the register needs n-1 set do trigger the event every n ticks
    // zero ticks is possible but would simply disable it
    if(ticks <= 2 || (ticks - 1) > SysTick_LOAD_RELOAD_Msk)
        return -1;

    SysTick->LOAD = ((ticks - 1) & SysTick_LOAD_RELOAD_Msk);

    SysTick->VAL = 0xdeadbeef; // any write clears it

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;

    return clock / ticks;
}

// Selects the next thread; returns != 0 if a context switch is needed
static uint32_t schedSchedule(void)
{
    size_t i, j;

    schedulerLock();

    for(i = 0; i < COUNT; i++)
    {
        j = ((ACTIVE + i) % COUNT) + 1;
        if(THREAD(j)->status_ == STATUS_ACTIVE)
        {
            NEXT = j;
            schedulerUnlock();
            return 1;
        }
    }

    // None found, check the running one (could be idle)
    if(THREAD(ACTIVE)->status_ == STATUS_ACTIVE)
    {
        schedulerUnlock();
        return 0;
    }

    // no active thread and not already idle, switch to idle
    NEXT = 0;
    schedulerUnlock();
    return 1;
}

// This gets called if a thread returns
static void schedExit(void* return_value)
{
    threadRemove(NULL);
}

// This gets scheduled if no thread is active
static void schedIdle(void)
{
    while(1);
}

// Fills the initial values for the thread stack
static void schedInitStack(Thread* thread, void* func,
        uint8_t *stack, size_t stack_size)
{
    // Map the stack frame at the top
    InitStack *stack_frame = (InitStack*)(
            (uint32_t)stack + (uint32_t)stack_size -
            (uint32_t)sizeof(InitStack));

    // Default psr
    stack_frame->psr = 0x21000000;
    // PC to start func
    stack_frame->pc = (uint32_t)func;
    // Reset value for return, shoudln't get reached
    stack_frame->lr = (uint32_t)&schedExit;

    // Save stack pointer
    thread->sp_ = (uint32_t)stack_frame;

    // Set active
    thread->status_ = STATUS_ACTIVE;

    // Mutex stuff
    thread->next_ = NULL;

#ifdef HACTAR_NEWLIB_REENT
    _REENT_INIT_PTR(&thread->reent_);
#endif

}

// Makes sure that scheduler state access is mutual exclusive
void schedulerLock(void)
{
    SCHEDULER_DISABLE();
}

void schedulerUnlock(void)
{
    SCHEDULER_ENABLE();
}

// Add a thread to the scheduler, so it gets scheduled
int32_t threadAdd(Thread* thread, void* func,
        uint8_t *stack, size_t stack_size)
{
    schedInitStack(thread, func, stack, stack_size);

    // "Append" do schedule array and make it available
    schedulerLock();
    if(COUNT == HACTAR_N_THREADS)
    {
        schedulerUnlock();
        return -1;
    }

    THREAD(++COUNT) = thread;
    schedulerUnlock();

    return 0;
}

// Tells the scheduler to move work to another thread if possible..
void threadYield(void)
{
    if(schedSchedule())
        __SVC();
}

// Remove a thread. If thread == NULL, the calling thread will be removed.
int32_t threadRemove(Thread* thread)
{
    size_t i, j;

    schedulerLock();

    if(thread == NULL)
        thread = THREAD(ACTIVE);

    // Search the list (first one is idle thread)
    for(i = 1; i < COUNT; i++)
    {
        if(THREAD(i) != thread)
            continue;

        // Move all back
        for(j = i + 1; j < COUNT; j++)
            THREAD(j - 1) = THREAD(j);

        // Adjust active/next if it was moved
        if(ACTIVE > i)
            ACTIVE--;
        if(NEXT > i)
            NEXT--;

        if(thread == THREAD(ACTIVE))
        {
            NEXT = 0;
            schedulerUnlock();
            threadYield();
            while(1);
            return -1;
        }

        schedulerUnlock();
        return 0;
    }

    schedulerUnlock();
    return -1;
}

// FIXME: set an active thread pointer on context switch so this is atomic
Thread* schedulerActiveThread(void)
{
    Thread *thread;
    schedulerLock();
    thread = THREAD(ACTIVE);
    schedulerUnlock();
    return thread;
}

// Set a thread sleeping, which means it will not be scheduled until
// it is set active again.
// Calling with NULL will set the status of the calling thread.
int32_t threadSetSleep(Thread* thread, uint8_t sleep)
{
    schedulerLock();

    if(thread == NULL)
        thread = THREAD(ACTIVE);

    if((thread->status_ == STATUS_ACTIVE) == !sleep)
    {
        schedulerUnlock();
        return -1;
    }

    if(!sleep)
        thread->status_ = STATUS_ACTIVE;
    else
    {
        thread->status_ = STATUS_SLEEPING;
        if(thread == THREAD(ACTIVE))
        {
            schedulerUnlock();
            threadYield();
            return 0;
        }
    }

    schedulerUnlock();
    return 0;
}

/* If you want to use newlib functions in an interrupt context:
 * This will set the reent struct to the global one and disable interrupts
 * for that period.
 * schedulerISRNewlibStart()
 * printf("foobar")
 * schedulerISRNewlibEnd()
 */

// Make the global reent struct the active one
void schedulerISRNewlibStart(void)
{
#if defined(HACTAR_NEWLIB_REENT)
    INTERRUPTS_DISABLE();
#if !defined(__DYNAMIC_REENT__)
    _REENT = _GLOBAL_REENT;
#else
    sched.in_isr_ = 1;
#endif
#endif
}

// Use the active thread reent struct
void schedulerISRNewlibEnd(void)
{
#if defined(HACTAR_NEWLIB_REENT)
#if !defined(__DYNAMIC_REENT__)
    _REENT = &THREAD(ACTIVE)->reent_;
#else
    sched.in_isr_ = 0;
#endif
    INTERRUPTS_ENABLE();
#endif
}

#if defined(HACTAR_NEWLIB_REENT) && defined(__DYNAMIC_REENT__)
// In case __DYNAMIC_REENT__ is defined, newlib will call
// __get_reent instead of following the _REENT pointer.
struct _reent * __getreent(void)
{
    struct _reent *reent;

    // getting the active thread is not atomic atm...
    schedulerLock();
    if(sched.in_isr_)
        reent = _GLOBAL_REENT
    else
        reent = &THREAD(ACTIVE)->reent_;

    schedulerUnlock();
    return reent;
}
#endif

// Enable the scheduler, will not return
int32_t schedulerInit(uint32_t frequency)
{
    // Make pendsv the lowest priority interrupt
    NVIC_SetPriority(PendSV_IRQn, PRIO_PENDSV);

    // Make systick use a slightly higher priority
    NVIC_SetPriority(SysTick_IRQn, PRIO_SYSTICK);

    NVIC_SetPriority(SVCall_IRQn, PRIO_SVCALL);

    // Add the idle thread as first and set is as the active one,
    // The next schedule will switch to a user thread if one is available
    schedInitStack(&idle_thread, &schedIdle, idle_stack, COUNT_OF(idle_stack));
    THREAD(0) = &idle_thread;
    ACTIVE = 0;

    // Set the systick, and return error if the frequency couldn't get set
    if(setSystick(frequency) < 0)
        return -1;

    // Select the first thread
    schedSchedule();
    ACTIVE = NEXT;

    // The CPU will restore registers from r0 and up, so move 8 up
    __set_PSP(THREAD(NEXT)->sp_ + 8 * 4);

    // set the reent struct
#if defined(HACTAR_NEWLIB_REENT) && !defined(__DYNAMIC_REENT__)
    _REENT = &THREAD(ACTIVE)->reent_;
#endif

    // Enable interrupts (SVC needs it)
    __enable_irq();

    // Trigger a System service call, which will detect that it was called
    // from the main stack and switch to the first registered thread
    // in user mode. This is needed since we can't change the
    // stack pointer in C functions.
    __SVC();

    // should not be reached..
    while(1);

    return -1;
}

// Periodic interrupt
void SysTick_Handler(void)
{
    // Do a context switch if needed
    if(schedSchedule())
        __PENDSV();

    // Do some time base work here maybe..
}

void __attribute__( ( naked ) ) PendSV_Handler(void)
{
    SCHEDULER_DISABLE();

    // This has to be done first, GCC will use r4-r11 with -Os.
    // Use r0, since that got already pushed by hardware, so it is safe here
    asm volatile (
        "MRS r0, psp            \n" // get the user stack pointer
        "STMDB r0!, {r4-r11}    \n" // push r4-r11 on the user stack and dec r0
        "MSR psp, r0            \n" // update stack pointer
    : : :
    "r0", "r4", "r5", "r6", "r8", "r9", "r10", "r11");

    // Now save the stack pointer to THREAD(ACTIVE)->sp_
    asm volatile (
        "MRS r0, psp            \n" // get the user stack pointer
        "STR r0, [%0]           \n" // save new sp value
    : :
    "r" (&THREAD(ACTIVE)->sp_) :
    "r0", "memory");

    // Switch ACTIVE <-> NEXT with one register + stack
    // Example: Scheduling triggers a pendsv, pendsv gets entered,
    // pending bit cleared, starts to execute first instruction,
    // since there is no locking active, systick can preempt and cancel
    // first instruction, systick sets pendsv pending again ->
    // pendsv gets tailchained again, getting called twice in a row without
    // a new schedule.. so this needs to be handled
    asm volatile (
        "LDR r0, [%0]           \n" // switch active <-> next:
        "PUSH {r0}              \n" //   so pendsv can be called multiple times
        "LDR r0, [%1]           \n" //   without scheduling and we don't need
        "STR r0, [%0]           \n" //   to handle it..
        "POP {r0}               \n" //
        "STR r0, [%1]           \n" //
     : :
     "r" (&ACTIVE), "r" (&NEXT) :
     "r0", "memory");

#if defined(HACTAR_NEWLIB_REENT) && !defined(__DYNAMIC_REENT__)
    // Set the reent struct pointer
    asm volatile (
        "MOV r0, %0         \n"
        "STR r0, [%1]       \n"
        : :
        "r" (&THREAD(ACTIVE)->reent_), "r" (&_REENT) :
        "r0", "memory");
#endif

    // Load the new stack pointer
    asm volatile (
        "LDR r0, [%0]           \n" // load new sp for new thread
        "MSR psp, r0            \n" // set user stack pointer
    : :
    "r" (&THREAD(ACTIVE)->sp_) :
    "r0", "memory");

    // Restore the stack and return to PSP (PendSV is lowest priority,
    // so it can never preempt another interrupt or itself, only user code)
    asm volatile (
        "MRS r0, psp            \n" // load new sp for new thread
        "LDMIA r0!, {r4-r11}    \n" // pop r4-r11 from stack, inc r0
        "MSR psp, r0            \n" // adjust user stack pointer
       : :
       "r" (&THREAD(ACTIVE)->sp_) :
       "r0", "r4", "r5", "r6", "r8", "r9", "r10", "r11");

    SCHEDULER_ENABLE();

    asm volatile (
        "bx lr                  \n" // return
    : : :);
}

void __attribute__( ( naked ) ) SVC_Handler(void)
{
    asm volatile (
        "MOV    r0, lr  \n" // If SVC gets called from the main stack
        "CMP    r0, %0  \n" // return to the user stack.
        "ITT    eq      \n"
        "MOVEQ  r0, %1  \n"
        "BXEQ   r0      \n"
    : :
    "i" (IRQ_RETURN_MSP), "i" (IRQ_RETURN_PSP) :
    "r0", "cc");

    // Since PendSV is naked, lr is wrong now, so load the user return value
    asm volatile (
        "BL     PendSV_Handler  \n"
        "MOV    lr, %0          \n"
        "bx     lr              \n"
    : :
    "i" (IRQ_RETURN_PSP) :);
}
