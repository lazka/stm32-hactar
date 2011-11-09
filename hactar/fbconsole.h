// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_FBCONSOLE_H__
#define HACTAR_FBCONSOLE_H__

#include "framebuffer.h"

typedef struct
{
    FbInfo *fb_info;
    FontInfo *font_info;
} FBConsoleInfo;

FBConsoleInfo fbconsole_info;

void initFramebufferStdoutDevice(FbInfo *fb_info, FontInfo *font_info);

#endif
