// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stdio.h>

#include <hactar/scheduler.h>
#include <hactar/hactar.h>

static hactarThread* threads[HACTAR_N_THREADS];

static struct {
    uint32_t active_index_;
    size_t thread_count_;
} sched_state;

// Add a thread to the scheduler, so it gets scheduled
int32_t hactarThreadAdd(hactarThread* thread)
{
    if(sched_state.thread_count_ == HACTAR_N_THREADS)
        return -1;

    thread->active_ = 0;
    thread->inactive_status_ = INIT;
    threads[sched_state.thread_count_] = thread;
    sched_state.thread_count_++;

    return 0;
}

// Schedule a thread for removal
// In case the thread is allocated on the stack/heap or you want to reuse
// the thread memory, you have to call hactarSchedulerWaitGone first.
int32_t hactarThreadRemove(hactarThread* thread)
{
    thread->active_ = 0;
    thread->inactive_status_ = DEAD;
    return 0;
}

int32_t hactarThreadSetSleep(hactarThread* thread, uint8_t sleep)
{
    if(!sleep)
        thread->active_ = 1;
    else
    {
        thread->inactive_status_ = SLEEPING;
        if(thread == threads[sched_state.active_index_])
            hactarSchedulerSchedule();
    }

    return 0;
}

// Wait until the thread memory can be reused
int32_t hactarThreadWaitGone(hactarThread* thread)
{
    if(thread->active_ || thread->inactive_status_ != DEAD)
        return -1;

    while(thread->inactive_status_ != BURIED);

    return 0;
}

// remove a thread from the list, by shifting all after it back
// sets the inactive status to buried, meaning the thread memory will
// not be touched again.
static void removeThread(uint32_t index)
{
    size_t j;
    hactarThread* to_remove = threads[index];

    for(j = index + 1; j < sched_state.thread_count_; j++)
       threads[j - 1] = threads[j];

    sched_state.thread_count_--;
    to_remove->inactive_status_ = BURIED;
}

// Do a context switch..
void hactarSchedulerSchedule(void)
{
    size_t i, j;
    hactarThread* active = threads[sched_state.active_index_];
    hactarThread* next = active;

    for(i = 0; i < sched_state.thread_count_; i++)
    {
        j = (sched_state.active_index_ + i) % sched_state.thread_count_;
        next = threads[j];
        if(next->active_)
            break;
        else
        {
            if(next->inactive_status_ == DEAD)
            {
                removeThread(j);
                i--;
                continue;
            }
        }
    }

    if(next == active)
        return;

    // go go go...
}

/*
 * Notes:
 *  - An ISR does not pre-emt itself.
 *  - lower prio go first
 *  - ISR can be set active without something beeing executed
 *  - BASEPRI can be set to mask all ISRs below (lower prio, higher number)
 *    (0 is no masking)
 *  - Everything that doesn't touch task management should be below
 *    things like IO should be above.
 *  - PendSV is async (setting the bit doesn't trigger it immediately)
 *  - SVCall is sync
 */

void hactarSchedulerInit(uint32_t frequency)
{
    // test
    interruptsDisable();
    interruptsEnable();

    NVIC_SetPriority(PendSV_IRQn, PENDSV_PRIO);
    NVIC_SetPriority(SysTick_IRQn, SYSTCK_PRIO);

    assert(hactarSystickTimer(frequency) >= 0);
}

// Frequency in HZ
// Returns the actual frequency that was set (If the clock is low,
// there is a rounding error) or < 0 if setting failed.
int hactarSystickTimer(uint32_t frequency)
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

    NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

    SysTick->VAL = 0xdeadbeef; // any write clears it

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;

    return clock / ticks;
}

void SysTick_Handler(void)
{
    //STM_EVAL_LEDToggle(LED2);

    // Trigger a PendSV IRQ
    SCB->ICSR = SCB_ICSR_PENDSVSET;
}

void PendSV_Handler(void)
{
    //STM_EVAL_LEDToggle(LED1);
}
