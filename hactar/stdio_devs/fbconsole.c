// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stdio.h>

#include <hactar/stdio_devs/fbconsole.h>
#include <hactar/display.h>

static int writeFramebufferStdout(char *ptr, int len, uint8_t err)
{
    FbInfo *fb = fbconsole_info.fb_info_;
    const FontInfo *font = fbconsole_info.font_info_;

    size_t i, dx, x, y;

    dx = font->width_;
    y = fbconsole_info.y_offset_;
    x = fbconsole_info.x_offset_;

    for(i = 0; i < len  && *ptr != '\0'; i++)
    {
        if(*ptr == '\n' || *ptr == '\r' || *ptr == EOF)
        {
            y += font->height_;
            x = 0;
        }
        else
        {
            fbDrawCharacter(fb, font, *ptr, x, y, FB_DEFAULT);
            x += dx;
        }

        ptr++;
    }

    fbconsole_info.y_offset_ = y;
    fbconsole_info.x_offset_ = x;

    displayUpdate(fb->display_, fb);

    return len;
}

static void clearFramebufferStdout(void)
{
    // reset offsets
    fbconsole_info.y_offset_ = 0;
    fbconsole_info.x_offset_ = 0;

    // clear framebuffer
    fbClear(fbconsole_info.fb_info_, FB_DEFAULT);

    // update display
    displayUpdate(fbconsole_info.fb_info_->display_, fbconsole_info.fb_info_);
}

void initFramebufferStdoutDevice(FbInfo *fb_info, const FontInfo *font_info)
{
    fbconsole_info.device_.write_func_ = &writeFramebufferStdout;
    fbconsole_info.device_.clear_func_ = &clearFramebufferStdout;
    fbconsole_info.fb_info_ = fb_info;
    fbconsole_info.font_info_ = font_info;
    fbconsole_info.y_offset_ = 0;
    fbconsole_info.x_offset_ = 0;

    stdout_device = (StdoutDevice*)&fbconsole_info;
}
