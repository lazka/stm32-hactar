// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <hactar/display_func.h>

void displayInit(DisplayInfo *display, size_t width, size_t height)
{
    display->init_(display, width, height);
}

void displayUpdate(DisplayInfo *display, FbInfo *fb)
{
    display->update_(display, fb);
}

void displayOn(DisplayInfo *display)
{
    display->on_(display);
}

void displayOff(DisplayInfo *display)
{
    display->off_(display);
}
