// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_DISPLAY_H__
#define HACTAR_DISPLAY_H__

#include <stddef.h>
#include <stdint.h>

typedef struct FbInfo FbInfo;
typedef struct DisplayInfo DisplayInfo;

struct DisplayInfo
{
    void (*init_)(DisplayInfo *display);
    void (*get_pos_)(FbInfo *fb, size_t x, size_t y,
        size_t *index, uint8_t *offset);
    void (*update_)(DisplayInfo *display, FbInfo *fb);
    void (*inval_)(DisplayInfo *display, size_t x, size_t y);
    void (*off_)(DisplayInfo *display);
    void (*on_)(DisplayInfo *display);
    size_t width_;
    size_t height_;
};

// Initializes the driver and driver data
void displayInit(DisplayInfo *display);

// Transmits all parts
void displayUpdate(DisplayInfo *display, FbInfo *fb);

// Switch the display on
void displayOn(DisplayInfo *display);

// Switch the display off
void displayOff(DisplayInfo *display);

#endif
