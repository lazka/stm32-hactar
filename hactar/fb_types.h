// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_FB_TYPES_H__
#define HACTAR_FB_TYPES_H__

#include <stddef.h>
#include <stdint.h>

typedef struct FbInfo FbInfo;
typedef struct DisplayInfo DisplayInfo;
typedef struct FbRect FbRect;

struct DisplayInfo
{
    void (*init_)(DisplayInfo *display, size_t width, size_t height);
    void (*get_pos_)(FbInfo *fb, size_t x, size_t y,
        size_t *index, uint8_t *offset);
    void (*update_)(DisplayInfo *display, FbInfo *fb);
    void (*inval_)(DisplayInfo *display, size_t x, size_t y);
    void (*off_)(DisplayInfo *display);
    void (*on_)(DisplayInfo *display);
    size_t width_;
    size_t height_;
};

struct FbRect
{
    size_t x1_;
    size_t y1_;
    size_t x2_;
    size_t y2_;
};

struct FbInfo
{
    uint8_t *data_;
    size_t height_;
    size_t width_;
    uint8_t clip_;
    FbRect clip_rect_;
    DisplayInfo *display_;
};

#endif
