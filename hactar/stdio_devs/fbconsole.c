// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <hactar/stdio_devs/fbconsole.h>

static int writeFramebufferStdout(char *ptr, int len, uint8_t err)
{
    // TODO: implement scrolling and line breaks.

    fbClear(fbconsole_info.fb_info_, FB_DEFAULT);
    fbDrawString(fbconsole_info.fb_info_, fbconsole_info.font_info_,
                 ptr, len, 0, 0, 0, FB_DEFAULT);

    return len;
}

void initFramebufferStdoutDevice(FbInfo *fb_info, FontInfo *font_info)
{
    fbconsole_info.device_.write_func_ = &writeFramebufferStdout;
    fbconsole_info.fb_info_ = fb_info;
    fbconsole_info.font_info_ = font_info;

    stdout_device = (StdoutDevice*)&fbconsole_info;
}
