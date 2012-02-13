// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifdef USE_STM3210C_EVAL

#include "fb_drawing.h"
#include <string.h>

#include <hactar/hactar.h>

#include <hactar/displays/stm3210c_eval.h>

// Must be a multiple of 8
#define EVAL_FB_WIDTH 320
#define EVAL_FB_HEIGHT 240
uint8_t fb_data[(EVAL_FB_WIDTH * EVAL_FB_HEIGHT) / 8];

static void drawShadowText(FbInfo *fb, const FontInfo *font, char *s,
        size_t x, size_t y, uint32_t flags)
{
    size_t i, len = strlen(s);
    size_t depth = (font->height_ + font->width_) / 10;

    for(i = 0; i < depth; i++)
    {
        fbDrawString(fb, font, s, len, x + i, y + 1 + i, 0, FB_DEFAULT | flags);
        fbDrawString(fb, font, s, len, x + i, y - 1 + i, 0, FB_DEFAULT | flags);
        fbDrawString(fb, font, s, len, x + 1 + i, y + i, 0, FB_DEFAULT | flags);
        fbDrawString(fb, font, s, len, x - 1 + i, y + i, 0, FB_DEFAULT | flags);
    }

    fbDrawString(fb, font, s, len, x + i - 1, y + i - 1, 0, FB_WHITE | flags);
}

void initFramebufferDrawingExample(void)
{
    DisplayInfo *display = (DisplayInfo*)&stm32c_eval_display;
    displayInit(display);

    FbInfo fb;
    fbInit(&fb, display, fb_data, EVAL_FB_WIDTH, EVAL_FB_HEIGHT);

    fbDrawPixel(&fb, 19, 1, FB_DEFAULT);
    fbDrawLine(&fb, 10, 3, 235, 216, FB_DEFAULT);
    fbClipRect(&fb, 10, 10, 150, 150);
    fbDrawCircle(&fb, 100, 100, 80, FB_DEFAULT);
    fbClipReset(&fb);
    fbDrawRect(&fb, 10, 10, 150, 150, FB_DEFAULT);

    fbDrawRect(&fb, 1, 20, 45, 40, FB_DEFAULT);
    fbClipRect(&fb, 1, 20, 45, 40);
    drawShadowText(&fb, &font_10x18, "foobar", 2, 22, FB_DEFAULT);
    fbClipReset(&fb);

    drawShadowText(&fb, &font_8x8, "foobar", 5, 200, FB_DEFAULT);

    drawShadowText(&fb, &font_6x11, "foobar", 5, 215, FB_DEFAULT);

    drawShadowText(&fb, &font_4x6, "foobar", 5, 230, FB_DEFAULT);

    drawShadowText(&fb, &font_8x16, "QUUX - QUUX", 250, 2, FB_VERT);
    displayUpdate(display, &fb);

    fbDrawCircle(&fb, 110, 110, 90, FB_DEFAULT);
    displayUpdate(display, &fb);

    drawShadowText(&fb, &font_10x18, "foobar", 100, 100, FB_DEFAULT);
    displayUpdate(display, &fb);

    drawShadowText(&fb, &font_8x16, "quux", 100, 150, FB_VERT);
    displayUpdate(display, &fb);
}

#endif // USE_STM3210C_EVAL
