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

typedef struct
{
    void (*init_)(void *, size_t width, size_t height);
    void (*get_pos_)(void *fb, size_t x, size_t y,
        size_t *index, uint8_t *offset);
    void (*update_)(void *, void *);
    void (*inval_)(void *, size_t x, size_t y);
    void (*off_)(void *);
    void (*on_)(void *);
    size_t width_;
    size_t height_;
} DisplayInfo;

#endif
