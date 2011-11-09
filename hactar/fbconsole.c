// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "fbconsole.h"
#include "stdout.h"

static int writeFramebufferStdout(char *ptr, int len)
{
    // TODO: implement scrolling and line breaks.

    fbClear(fbconsole_info.fb_info, FB_DEFAULT);
    fbDrawString(fbconsole_info.fb_info, fbconsole_info.font_info,
                 ptr, len, 0, 0, 0, FB_DEFAULT);

    return len;
}

void initFramebufferStdoutDevice(FbInfo *fb_info, FontInfo *font_info)
{
    fbconsole_info.fb_info = fb_info;
    fbconsole_info.font_info = font_info;

    stdout_device.data_ = &fbconsole_info;
    stdout_device.write_func_ = &writeFramebufferStdout;
}
