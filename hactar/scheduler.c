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
    size_t i, idle = 0;

    schedulerLock();

    // get the next active one
    for(i = ACTIVE + 1; i != ACTIVE; i++)
    {
        if(i >= COUNT)
            i = 0;

        if(THREAD(i) == &idle_thread)
            idle = i;
        else if(THREAD(i)->active_)
        {
            NEXT = i;
            schedulerUnlock();
            return 1;
        }
    }

    // None found, check the running one
    if(THREAD(ACTIVE)->active_)
    {
        schedulerUnlock();
        return 0;
    }

    // no active thread and not already idle, switch to idle
    NEXT = idle;
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
    thread->active_ = 1;
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
    if(COUNT == HACTAR_N_THREADS + 1)
    {
        schedulerUnlock();
        return -1;
    }

    THREAD(COUNT) = thread;
    COUNT++;
    schedulerUnlock();

    return 0;
}

// Tells the scheduler to move work to another thread if possible..
// This call is async, so it could take some time before the switch.
void threadYield(void)
{
    if(schedSchedule())
        __PENDSV();
}

// Remove a thread. If thread == NULL, the calling thread will be removed.
int32_t threadRemove(Thread* thread)
{
    size_t i;
    uint32_t yield = 0;

    schedulerLock();

    if(thread == NULL)
        thread = THREAD(ACTIVE);

    // Search the list (first one is idle thread)
    for(i = 1; i < COUNT; i++)
    {
        if(THREAD(i) != thread)
            continue;

        // Move all back
        for(i++ ;i < COUNT; i++)
            THREAD(i - 1) = THREAD(i);

        if(thread == THREAD(ACTIVE))
            yield = 1;

        // make sure we aren't our of bounds
        ACTIVE--;

        // done
        schedulerUnlock();

        // yield if we are the caller
        if(yield)
        {
            __SVC();
            while(1);
        }
        return 0;
    }

    schedulerUnlock();
    return -1;
}

// Set a thread sleeping, which means it will not be scheduled until
// it is set active again.
// Calling with NULL will set the status of the calling thread.
int32_t threadSetSleep(Thread* thread, uint8_t sleep)
{
    schedulerLock();

    if(thread == NULL)
        thread = THREAD(ACTIVE);

    if(!sleep)
        thread->active_ = 1;
    else
    {
        thread->active_ = 0;
        thread->inactive_status_ = SLEEPING;
        if(thread == THREAD(ACTIVE))
        {
            schedulerUnlock();
            __SVC();
            // Race ?? .. but we need to wait so this is blocking
            while(!thread->active_);
            return 0;
        }
    }

    schedulerUnlock();
    return 0;
}

// Enable the scheduler, will not return
int32_t schedulerInit(uint32_t frequency)
{
    // Make pendsv the lowest priority interrupt
    NVIC_SetPriority(PendSV_IRQn, PENDSV_PRIO);

    // Move systick below the scheduler prio mask
    NVIC_SetPriority(SysTick_IRQn, SYSTCK_PRIO);

    // Add the idle thread
    threadAdd(&idle_thread, &schedIdle, idle_stack, COUNT_OF(idle_stack));

    // Set an active one
    ACTIVE = 0;

    // Set the systick, and return error if the frequency couldn't get set
    if(setSystick(frequency) < 0)
        return -1;

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

void SVC_Handler(void)
{
    uint32_t lr;

    // get the link register
    asm volatile ("MOV %0, lr" : "=r" (lr) );

    // Check from where it was called from
    // 0xFFFFFFF1 - handler
    // 0xFFFFFFF9 - main
    // 0xFFFFFFFD - process
    if((lr ^ 0x9) & 0xF)
    {
        // was called from PSP or another interrupt

        // this could get used for sync context switching
    }
    else
    {
        // was called from MSP, which only happens on start
        // so we can use the first user stack and return to it

        // This is needed since changing the stack pointer in a C function
        // is dangerous (could use the stack) and returning from an IRQ
        // to another stack is easy.

        schedSchedule();
        ACTIVE = NEXT;

        // The CPU will restore registers from r0 and up, so move 8 up
        __set_PSP(THREAD(NEXT)->sp_ + 8 * 4);

        // Make the interrupt return to the psp stack
        // by loading the magic value in the pc
        void *addr = (void *) IRQ_RETURN_PSP;
        asm volatile("bx %0" : : "r" (addr));
    }
}

void __attribute__( ( naked ) ) PendSV_Handler(void)
{
    // This has to be done first, GCC will use r4-r11 with -Os.
    // Use r0, since that got already pushed by hardware, so it is safe here
    asm volatile (
        "MRS r0, psp            \n" // get the user stack pointer
        "STMDB r0!, {r4-r11}    \n" // push r4-r11 on the user stack and dec r0
        "MSR psp, r0            \n" // update stack pointer
    : : :
    "r0", "r4", "r5", "r6", "r8", "r9", "r10", "r11");

    SCHEDULER_DISABLE();

    // Now save the stack pointer to THREAD(ACTIVE)->sp_
    asm volatile (
        "MRS r0, psp            \n" // get the user stack pointer
        "STR r0, [%0]           \n" // save new sp value
    : :
    "r" (&THREAD(ACTIVE)->sp_) :
    "r0", "memory");

    // Switch ACTIVE <-> NEXT with one register + stack
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

    // Load the new stack pointer
    asm volatile (
        "LDR r0, [%0]           \n" // load new sp for new thread
        "MSR psp, r0            \n" // set user stack pointer
    : :
    "r" (&THREAD(ACTIVE)->sp_) :
    "r0", "memory");

    SCHEDULER_ENABLE();

    // Restore the stack and return to PSP (PendSV is lowest priority,
    // so it can never preempt another interrupt or itself, only user code)
    asm volatile (
        "MRS r0, psp            \n" // load new sp for new thread
        "LDMIA r0!, {r4-r11}    \n" // pop r4-r11 from stack, inc r0
        "MSR psp, r0            \n" // adjust user stack pointer
        "bx lr                  \n" // return
       : :
       "r" (&THREAD(ACTIVE)->sp_) :
       "r0", "r4", "r5", "r6", "r8", "r9", "r10", "r11");
}
