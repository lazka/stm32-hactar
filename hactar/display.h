// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_DISPLAY_H__
#define HACTAR_DISPLAY_H__

#include <hactar/fb_types.h>

// Initializes the driver and driver data
void displayInit(DisplayInfo *display, size_t width,
    size_t height);

// Transmits all parts
void displayUpdate(DisplayInfo *display, FbInfo *fb);

// Switch the display on
void displayOn(DisplayInfo *display);

// Switch the display off
void displayOff(DisplayInfo *display);

#endif
