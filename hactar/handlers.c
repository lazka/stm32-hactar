// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "stm32f10x.h"
#include <stdint.h>

extern void hactarSystemInit(void);
extern int main(void);

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

void Default_Handler(void)
{
    while(1);
}

void Reset_Handler(void) {
    uint32_t *src, *dst;

    // copy the data segment into ram
    src = &_sidata;
    dst = &_sdata;
    while(dst < &_edata)
        *(dst++) = *(src++);

    // zero the bss segment
    dst = &_sbss;
    while(dst < &_ebss)
        *(dst++) = 0;

    hactarSystemInit();

    main();

    while(1);
}

static void checkFault(void)
{
    // --- MEMORY MANAGEMENT FAULT ---

    // Instruction access violation
    if(SCB->CFSR & SCB_CFSR_IACCVIOL) while(1);

    // Data access violation
    if(SCB->CFSR & SCB_CFSR_DACCVIOL) while(1);

    // Unstacking error
    if(SCB->CFSR & SCB_CFSR_MUNSTKERR) while(1);

    // Stacking error
    if(SCB->CFSR & SCB_CFSR_MSTKERR) while(1);

    // Memory Manage Address Register address valid flag
    if(SCB->CFSR & SCB_CFSR_MMARVALID) while(1);

    // --- USAGE FAULT ---

    // The processor attempt to execute an undefined instruction
    if(SCB->CFSR & SCB_CFSR_UNDEFINSTR) while(1);

    // Invalid combination of EPSR and instruction
    if(SCB->CFSR & SCB_CFSR_INVSTATE) while(1);

    // Attempt to load EXC_RETURN into pc illegally
    if(SCB->CFSR & SCB_CFSR_INVPC) while(1);

    // Attempt to use a coprocessor instruction
    if(SCB->CFSR & SCB_CFSR_NOCP) while(1);

    // Fault occurs when there is an attempt to make an unaligned memory access
    if(SCB->CFSR & SCB_CFSR_UNALIGNED) while(1);

    // Fault occurs when SDIV or DIV instruction is used with a divisor of 0
    if(SCB->CFSR & SCB_CFSR_DIVBYZERO) while(1);

    // --- BUS FAULT ---

    // Instruction bus error flag
    if(SCB->CFSR & SCB_CFSR_IBUSERR) while(1);

    // Precise data bus error
    if(SCB->CFSR & SCB_CFSR_PRECISERR) while(1);

    // Imprecise data bus error
    if(SCB->CFSR & SCB_CFSR_IMPRECISERR) while(1);

    // Unstacking error
    if(SCB->CFSR & SCB_CFSR_UNSTKERR) while(1);

    // Stacking error
    if(SCB->CFSR & SCB_CFSR_STKERR) while(1);

    // --- HARD FAULT ---

    // Fault occurs because of vector table read on exception processing
    if(SCB->HFSR & SCB_HFSR_VECTTBL) while(1);

    // Fault related to debug
    if(SCB->HFSR & SCB_HFSR_DEBUGEVT) while(1);

    // Hard Fault activated when a configurable
    // Fault was received and cannot activate
    if(SCB->HFSR & SCB_HFSR_FORCED) while(1);

    while(1);
}

void HardFault_Handler(void)
{
    checkFault();
}

void UsageFault_Handler(void)
{
    checkFault();
}

void MemManage_Handler(void)
{
    checkFault();
}
