// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stddef.h>
#include <stdio.h>

#include "stm32_eval.h"

#include <hactar/hactar.h>

//#include <hactar/stdio_devs/eval_lcd.h>
#include <hactar/stdio_devs/usart.h>
#include <hactar/stdio_devs/fbconsole.h>

//#include <hactar/displays/st7565r.h>
#include <hactar/displays/stm3210c_eval.h>

static void printArgs(char **args)
{
    size_t i = 0;

    while(args[i] != NULL)
    {
        iprintf("%u) '%s'\n", i + 1, args[i]);
        i++;
    }
}

TermCommand term_cmds[] = {
        {
                .command_ = "args",
                .description_ = "Print arguments",
                .function_ = &printArgs,
        },
};

// Must be a multiple of 8
#define WIDTH 256
#define HEIGHT 128
uint8_t fb_data[(WIDTH * HEIGHT) / 8];

int main(void)
{
    DisplayInfo *display = (DisplayInfo*)&stm32c_eval_display;
    //DisplayInfo *display = (DisplayInfo*)&st7565r;
    displayInit(display, 64, 64);

    FbInfo fb;
    fbInit(&fb, display, fb_data, WIDTH, HEIGHT);

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

    initUSARTStdioDevice(HACTAR_USART_STDIO_STDIN | HACTAR_USART_STDIO_STDIN_ECHO | HACTAR_USART_STDIO_STDOUT);
    //initEvalLCDStdoutDevice();
    initFramebufferStdoutDevice(&fb, &font_4x6);

    startTerminal(term_cmds, 1);

    assert(0);

    while (1)
      __WFI();

    return 0;
}
