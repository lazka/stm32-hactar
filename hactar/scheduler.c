// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stdio.h>

#include <hactar/scheduler.h>
#include <hactar/misc.h>
#include <hactar/startup.h>
#include <hactar/platform_check.h>

#include "stm32_eval.h"

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

void hactarInitScheduler(uint32_t frequency)
{
    // test
    interruptsDisable();
    interruptsEnable();

    NVIC_SetPriority(PendSV_IRQn, PENDSV_PRIO);
    NVIC_SetPriority(SysTick_IRQn, SYSTCK_PRIO);

    assert(configureSystickTimer(frequency) >= 0);
}

// Frequency in HZ
// Returns the actual frequency that was set (If the clock is low,
// there is a rounding error) or < 0 if setting failed.
int configureSystickTimer(uint32_t frequency)
{
    uint32_t ticks;

    if(frequency == 0)
        return -1;

#if HACTAR_CLK_MUX_STK == HACTAR_CLK_MUX_STK_SRC_DIV8
    frequency *= 8;
#endif

    ticks = hactarGetSystemClock() / (frequency * HACTAR_CLK_SCALE_AHB);

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

    return hactarGetSystemClock() / ticks;
}

void SysTick_Handler(void)
{
    STM_EVAL_LEDToggle(LED2);

    // Trigger a PendSV IRQ
    SCB->ICSR = SCB_ICSR_PENDSVSET;
}

void PendSV_Handler(void)
{
    STM_EVAL_LEDToggle(LED1);
}
