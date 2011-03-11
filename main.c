// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "framebuffer.h"

//#include "font_4x6.h"
#include "font_6x11.h"
#include "font_8x8.h"
//#include "font_8x16.h"
#include "font_10x18.h"

#include "display_st7565r.h"
#include "display_func.h"

// Must be a multiple of 8
#define HEIGHT 64
#define WIDTH 72

uint8_t fb_data[(HEIGHT * WIDTH) / 8];

int main()
{
    FbInfo fb;

    DisplayInfo *display = (DisplayInfo*)&st7565r;

    displayInit(display, WIDTH, HEIGHT);
    fbInit(&fb, display, fb_data, WIDTH, HEIGHT);
    displayUpdate(display, &fb);

    fbDrawPixel(&fb, 19, 0, FB_DEFAULT);
    fbDrawLine(&fb, 10, 3, 35, 16, FB_DEFAULT);
    fbDrawCircle(&fb, 10, 10, 8, FB_DEFAULT);
    fbDrawRect(&fb, 0, 0, 15, 15, FB_DEFAULT);
    fbDrawRect(&fb, 1, 21, 29, 31, FB_DEFAULT);
    fbClipRect(&fb, 1, 21, 29, 31);
    fbDrawString(&fb, &font_6x11, "foobar", 2, 22, 0, FB_DEFAULT);
    fbClipReset(&fb);
    fbDrawString(&fb, &font_10x18, "foobar", 0, 47, 0, FB_DEFAULT);
    fbDrawString(&fb, &font_8x8, "FOOBAR", 50, 2, 0, FB_VERT);

    displayUpdate(display, &fb);

    #ifdef FB_DEBUG
    fbDebugPrint(&fb);
    #endif

    return 0;
}
