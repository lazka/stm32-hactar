// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef EX_DOGM128_FB_H__
#define EX_DOGM128_FB_H__

#include "dogm128_framebuffer.h"

#include <hactar/hactar.h>
#include <stdint.h>
#include <stdio.h>

#include <hactar/displays/st7565r.h>
#include <hactar/stdio_devs/usart.h>
#include <hactar/stdio_devs/fbconsole.h>

// Must be a multiple of 8
#define DOGM_FB_WIDTH 256
#define DOGM_FB_HEIGHT 128
uint8_t fb_data[(DOGM_FB_WIDTH * DOGM_FB_HEIGHT) / 8];

void initDogm128FramebufferExample(void)
{
    DisplayInfo *display = (DisplayInfo*)&st7565r;
    displayInit(display, DOGM_FB_WIDTH, DOGM_FB_HEIGHT);

    FbInfo fb;
    fbInit(&fb, display, fb_data, DOGM_FB_WIDTH, DOGM_FB_HEIGHT);

    // USART for input without echo
    initUSARTStdioDevice(HACTAR_USART_STDIO_STDIN);

    // Output to framebuffer
    initFramebufferStdoutDevice(&fb, &font_4x6);

    iprintf("DOGM128 Framebuffer Terminal\n");

    // Start terminal
    startTerminal(NULL, 0);
}

#endif
