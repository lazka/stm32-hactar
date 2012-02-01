// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

#include <hactar/misc.h>

void interruptsDisable()
{
    asm volatile ("cpsid   i" : : : "memory");
}

void interruptsEnable()
{
    asm volatile ("cpsie   i" : : : "memory");
}

void __hactar_assert(const char* file, int line, const char* func,
        const char* expr)
{
    fflush(stdout);
    fiprintf(stderr, "ASSERT: %s[%i]\n%s (%s)\n", file, line, func, expr);
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

    for(port = 0; port < 16; port++) {
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

MemoryInfo getMemoryInfo(void)
{
    extern char _spretext, _epretext;
    extern char _srodata, _erodata;
    extern char _sdata, _edata;
    extern char _sbss, _ebss;
    extern char _end;
    extern char _estack, _eflash, _etext, _sflash;

    MemoryInfo info;

    info.flash_free_ = ((size_t)&_eflash - (size_t)&_etext) * 100u /
                       ((size_t)&_eflash - (size_t)&_sflash);
    info.text_ = (size_t)&_epretext - (size_t)&_spretext;
    info.ro_data_ = (size_t)&_erodata - (size_t)&_srodata;
    info.ram_free_ = ((size_t)&_estack - (size_t)sbrk(0)) * 100u /
                     ((size_t)&_estack - (size_t)&_sdata);
    info.data_ = (size_t)&_edata - (size_t)&_sdata;
    info.bss_ = (size_t)&_ebss - (size_t)&_sbss;
    info.heap_ = (size_t)sbrk(0) - (size_t)&_end;
    info.malloc_ = mallinfo().uordblks;

    return info;
}
