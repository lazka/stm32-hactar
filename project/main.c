// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

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

    startTerminal();

    assert(0);

    while (1)
      __WFI();

    return 0;
}
