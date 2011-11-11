// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_STDIO_DEVS_FBCONSOLE_H__
#define HACTAR_STDIO_DEVS_FBCONSOLE_H__

#include "../framebuffer.h"
#include "../stdio_dev.h"

typedef struct
{
    StdoutDevice device_;
    FbInfo *fb_info_;
    FontInfo *font_info_;
} FBConsoleInfo;

FBConsoleInfo fbconsole_info;

void initFramebufferStdoutDevice(FbInfo *fb_info, FontInfo *font_info);

#endif
