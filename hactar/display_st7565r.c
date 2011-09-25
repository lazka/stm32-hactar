// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "display_st7565r.h"
#ifdef FB_DEBUG
#include <stdio.h>
#endif

void st7565rInit(DisplayInfoST7565R *display, size_t width, size_t height)
{
    display->base_.width_ = width;
    display->base_.height_ = height;
}

void st7565rUpdate(DisplayInfoST7565R *display, FbInfo *fb)
{
    #ifdef FB_DEBUG
    size_t i;
    printf("Update summary ------------------------------------------\n");
    for(i = 0; i < ST7565R_PAGES; ++i)
    {
        if(display->dirty_pages_ & (1 << i))
        {
            printf("Page %d needs an update from %d -> %d\n", i,
            display->dirty_start_[i],  display->dirty_end_[i]);
        }
    }
    #endif

    display->dirty_pages_ = 0;
}

// Is used to get the byte and bit offset for a specific pixel and thus
// specifies the fb data layout. Because of this we can transmit straight
// from the fb data.
// Returns 0 on success (the position is in the fb)
void st7565rGetPosition(FbInfo *fb, size_t x, size_t y,
    size_t *index, uint8_t *offset)
{
    *index = (y / 8) * fb->width_ + x;
    *offset = y % 8;
}

// Gets called whenever a byte in the framebuffer changes.
// Can be used to mark certain areas dirty, so an update is faster.
void st7565rInval(DisplayInfoST7565R *display, size_t x, size_t y)
{
    uint8_t page;

    if(x >= display->base_.width_ || x >= display->base_.height_)
        return;

    page = y / 8;

    if((display->dirty_pages_ & (1 << page)))
    {
        if(x < display->dirty_start_[page])
            display->dirty_start_[page] = x;
        if(x > display->dirty_end_[page])
            display->dirty_end_[page] = x;
    }
    else
    {
        display->dirty_pages_ |= (1 << page);
        display->dirty_start_[page] = x;
        display->dirty_end_[page] = x;
    }
}

// Switch it off, but don't clear the controller memory
void st7565rOff(DisplayInfoST7565R *display)
{
}

void st7565rOn(DisplayInfoST7565R *display)
{
}
