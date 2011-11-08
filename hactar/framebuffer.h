// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_FRAMEBUFFER_H__
#define HACTAR_FRAMEBUFFER_H__

#define FB_DEBUG 0

#include <stddef.h>
#include <stdint.h>

#include "font.h"
#include "display.h"

#define FB_DEFAULT 0
#define FB_BLACK FB_DEFAULT
#define FB_HOR FB_DEFAULT
#define FB_WHITE (1<<0)
#define FB_FILL (1<<1)
#define FB_VERT (1<<2)

typedef struct
{
    size_t x1_;
    size_t y1_;
    size_t x2_;
    size_t y2_;
} FbRect;

typedef struct
{
    uint8_t *data_;
    size_t height_;
    size_t width_;
    uint8_t clip_;
    FbRect clip_rect_;
    DisplayInfo *display_;
} FbInfo;

void fbInit(FbInfo *fb, DisplayInfo *display, uint8_t *data,
    size_t width, size_t height);

void fbClear(FbInfo *fb, uint32_t flags);

void fbDrawPixel(FbInfo *fb, size_t x, size_t y, uint32_t flags);

uint32_t fbGetPixel(FbInfo *fb, size_t x, size_t y);

void fbDrawLine(FbInfo *fb, size_t x1, size_t y1,
    size_t x2, size_t y2, uint32_t flags);

void fbDrawRect(FbInfo *fb, size_t x1, size_t y1,
    size_t x2, size_t y2, uint32_t flags);

void fbDrawCircle(FbInfo *fb, size_t x0, size_t y0,
    size_t radius, uint32_t flags);

void fbClipRect(FbInfo *fb, size_t x1, size_t y1,
    size_t x2, size_t y2);

void fbClipReset(FbInfo *fb);

void fbUtilFixRect(size_t *x1, size_t *y1,
    size_t *x2, size_t *y2);

void fbCopyRect(FbInfo *fb, size_t x1, size_t y1,
    size_t x2, size_t y2, size_t x3, size_t y3);

#ifdef FB_DEBUG
void fbDebugPrint(FbInfo *fb);
#endif

void fbDrawCharacter(FbInfo *fb, const FontInfo *font, unsigned char c,
    size_t x, size_t y, uint32_t flags);

void fbDrawString(FbInfo *fb, const FontInfo *font, char *s,
    size_t x, size_t y, int spacing, uint32_t flags);

#endif
