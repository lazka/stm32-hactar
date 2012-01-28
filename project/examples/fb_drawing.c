// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "fb_drawing.h"

#include <hactar/hactar.h>

#include <hactar/displays/stm3210c_eval.h>

// Must be a multiple of 8
#define EVAL_FB_WIDTH 320
#define EVAL_FB_HEIGHT 240
uint8_t fb_data[(EVAL_FB_WIDTH * EVAL_FB_HEIGHT) / 8];

void initFramebufferDrawingExample(void)
{
    FbInfo fb;

    DisplayInfo *display = (DisplayInfo*)&stm32c_eval_display;

    displayInit(display, EVAL_FB_WIDTH, EVAL_FB_HEIGHT);
    fbInit(&fb, display, fb_data, EVAL_FB_WIDTH, EVAL_FB_HEIGHT);
    displayUpdate(display, &fb);

    fbDrawPixel(&fb, 19, 1, FB_DEFAULT);
    fbDrawLine(&fb, 10, 3, 235, 216, FB_DEFAULT);
    fbClipRect(&fb, 10, 10, 150, 150);
    fbDrawCircle(&fb, 100, 100, 80, FB_DEFAULT);
    fbClipReset(&fb);
    fbDrawRect(&fb, 10, 10, 150, 150, FB_DEFAULT);

    fbDrawRect(&fb, 1, 20, 45, 40, FB_DEFAULT);
    fbClipRect(&fb, 1, 20, 45, 40);
    fbDrawString(&fb, &font_10x18, "foobar", 6, 2, 22, 0, FB_DEFAULT);
    fbClipReset(&fb);

    fbDrawString(&fb, &font_4x6, "foobar", 6, 0, 200, 0, FB_DEFAULT);

    fbDrawString(&fb, &font_8x16, "QUUX - QUUX", 11, 250, 2, 2, FB_VERT);

    displayUpdate(display, &fb);
}
