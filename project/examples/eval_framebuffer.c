// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef EX_EVAL_FRAMEBUFFER_H__
#define EX_EVAL_FRAMEBUFFER_H__

#include "eval_framebuffer.h"

#include <stdio.h>

#include <hactar/hactar.h>

#include <hactar/stdio_devs/fbconsole.h>
#include <hactar/stdio_devs/usart.h>

#include <hactar/displays/stm3210c_eval.h>

// Must be a multiple of 8
#define EVAL_FB_WIDTH 320
#define EVAL_FB_HEIGHT 240
uint8_t fb_data[(EVAL_FB_WIDTH * EVAL_FB_HEIGHT) / 8];

void initEvalFramebufferExample(void)
{
    DisplayInfo *display = (DisplayInfo*)&stm32c_eval_display;
    displayInit(display);

    FbInfo fb;
    fbInit(&fb, display, fb_data, EVAL_FB_WIDTH, EVAL_FB_HEIGHT);

    // USART for input without echo
    initUSARTStdioDevice(HACTAR_USART_STDIO_STDIN);

    // Output to framebuffer
    initFramebufferStdoutDevice(&fb, &font_8x8);

    iprintf("Eval Board Framebuffer Terminal\n");

    // Start terminal
    startTerminal(NULL, 0);
}

#endif
