// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "stm32f10x.h"

#include "stm32_eval.h"

#include "hactar/hactar.h"

#include "hactar/stdio_devs/eval_lcd.h"
#include "hactar/displays/st7565r.h"
#include "hactar/stdio_devs/usart.h"

int main(void)
{
    DisplayInfo *display = (DisplayInfo*)&st7565r;
    displayInit(display, 64, 64);

    spinlock_t lock;

    hactarSpinInit(&lock);
    hactarSpinLock(&lock);
    hactarSpinTrylock(&lock);
    hactarSpinUnlock(&lock);
    hactarSpinLock(&lock);

    STM_EVAL_LEDInit(LED1);
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);

    STM_EVAL_LEDOn(LED1);
    STM_EVAL_LEDOn(LED2);
    STM_EVAL_LEDOn(LED3);
    STM_EVAL_LEDOn(LED4);
    STM_EVAL_LEDOff(LED3);

    hactarInitScheduler(1);

    initUSARTStdioDevice(HACTAR_USART_STDIO_STDIN | HACTAR_USART_STDIO_STDIN_ECHO);
    initEvalLCDStdoutDevice();

    RCC_ClocksTypeDef test;
    RCC_GetClocksFreq(&test);

    printf("ADC   %u\n", (unsigned int)test.ADCCLK_Frequency);
    printf("HCLK  %u\n", (unsigned int)test.HCLK_Frequency);
    printf("PCLK1 %u\n", (unsigned int)test.PCLK1_Frequency);
    printf("PCLK2 %u\n", (unsigned int)test.PCLK2_Frequency);
    printf("SYS   %u\n", (unsigned int)test.SYSCLK_Frequency);
    printf("SYS2  %u\n", (unsigned int)hactarGetSystemClock());

    char temp[100];
    printf("Name: ");
    scanf("%s", temp);
    printf("Hallo %s!\n", temp);

    assert(0);

    while (1)
      __WFI();
}
