// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stdio.h>

#include "misc.h"

void HardFault_Handler(void)
{
    // Fault occurs because of vector table read on exception processing
    if(SCB->HFSR & SCB_HFSR_VECTTBL) while(1);

    // Hard Fault activated when a configurable
    // Fault was received and cannot activate
    if(SCB->HFSR & SCB_HFSR_DEBUGEVT) while(1);

    // Fault related to debug
    if(SCB->HFSR & SCB_HFSR_DEBUGEVT) while(1);
}

void BusFault_Handler(void)
{
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
}

void UsageFault_Handler(void)
{
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
}

void MemManage_Handler(void)
{
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
}

void interruptsDisable()
{
    asm volatile ("cpsid   i" : : : "memory");
}

void interruptsEnable()
{
    asm volatile ("cpsie   i" : : : "memory");
}

void assert_failed(uint8_t* file, uint32_t line)
{
    fprintf(stderr, "ASSERT: [%u] %s\n", (unsigned int)line, file);

    while(1)
        __WFI();
}

/*
 * Example:
 *   GPIO_InitTypeDef GPIO_InitStructure;
 *   GPIO_GetPinConfig(GPIOA, GPIO_Pin_6, &GPIO_InitStructure);
 */
void GPIO_GetPinConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,
        GPIO_InitTypeDef* GPIO_InitStruct)
{
    uint32_t port = 0, cnf = 0, port_config = 0;

    for(port = 0; port <= 16; port++) {
        if(GPIO_Pin == (1 << port))
            break;
    }

    if(port >= 8)
        port_config = ((GPIOx->CRH) >> ((port - 8) * 4)) & 0xF;
    else
        port_config = ((GPIOx->CRL) >> ((port) * 4)) & 0xF;

    GPIO_InitStruct->GPIO_Speed = port_config & 0x3;
    cnf = (port_config >> 2) & 0x3;

    if(GPIO_InitStruct->GPIO_Speed == 0)
    {
        if(GPIOx->ODR & GPIO_Pin)
            GPIO_InitStruct->GPIO_Mode = (cnf << 2) | 0x40;
        else
            GPIO_InitStruct->GPIO_Mode = (cnf << 2) | 0x20;
    }
    else
        GPIO_InitStruct->GPIO_Mode = (cnf << 2) | 0x10;

    GPIO_InitStruct->GPIO_Pin = GPIO_Pin;
}
