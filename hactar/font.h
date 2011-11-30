// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef FONT_H__
#define FONT_H__

typedef struct
{
    const uint8_t *data_;
    uint8_t width_;
    uint8_t height_;
} FontInfo;

extern const FontInfo font_10x18, font_4x6, font_6x11, font_8x16, font_8x8;

#endif
