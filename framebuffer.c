// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "framebuffer.h"
#include "font.h"

#ifdef FB_DEBUG
    #include <stdio.h>

    void fbDebugPrint(FbInfo *fb)
    {
        size_t i,j;

        for(i = 0; i < fb->height_; ++i)
        {
            for(j = 0; j < fb->width_; ++j)
            {
                if(fbGetPixel(fb, j, i) & FB_WHITE)
                    printf("\xe2\x96\xa1");
                else
                    printf("\xE2\x96\xa0");
                printf(" ");
            }
            printf("\n");
        }
    }
#endif

// Initializes the fbInit struct. Has to be called before passing the struct
// to any other function.
void fbInit(FbInfo *fb, DisplayInfo *display, uint8_t *data,
    size_t width, size_t height)
{
    fb->data_ = data;
    fb->height_ = height;
    fb->width_ = width;
    fb->clip_ = 0;
    fb->display_= display;

    fbClear(fb, FB_DEFAULT);
}

// Clears the whole framebuffer.
void fbClear(FbInfo *fb, uint32_t flags)
{
    size_t i,j;

    for(i = 0; i < fb->height_; ++i)
        for(j = 0; j < fb->width_; ++j)
            fbDrawPixel(fb, j, i, flags | FB_WHITE);
}

// Draws a pixel at a specific point.
// In case clipping is active and the point is not in the clip area or not
// on in the framebuffer, it will be ignored.
void fbDrawPixel(FbInfo *fb, size_t x, size_t y,
    uint32_t flags)
{
    size_t index;
    uint8_t offset;

    FbRect *r = &fb->clip_rect_;

    if(fb->clip_ && (x > r->x2_ || x < r->x1_ || y < r->y1_ || y > r->y2_))
        return;

    if(x >= fb->width_ || y >= fb->height_)
        return;

    fb->display_->get_pos_(fb, x, y, &index, &offset);

    if(flags & FB_WHITE)
        fb->data_[index] &= ~(1<<offset);
    else
        fb->data_[index] |= 1<<offset;

    fb->display_->inval_(fb->display_, x, y);
}

// Returns FB_WHITE or FB_BLACK.. if the pixel is not in the fb: FB_WHITE
uint32_t fbGetPixel(FbInfo *fb, size_t x, size_t y)
{
    size_t index;
    uint8_t offset;

    if(x >= fb->width_ || y >= fb->height_)
        return FB_WHITE;

    fb->display_->get_pos_(fb, x, y, &index, &offset);

    if(fb->data_[index] & (1<<offset))
        return FB_BLACK;
    return FB_WHITE;
}

// Draws a straight line between two points.
void fbDrawLine(FbInfo *fb, size_t x1, size_t y1,
    size_t x2, size_t y2, uint32_t flags)
{
    int delta_x, delta_y, err, e2, sx, sy;

    if(x1 < x2) {
        delta_x = x2 - x1;
        sx = 1;
    }
    else {
        delta_x = x1 - x2;
        sx = -1;
    }
    if(y1 < y2) {
        delta_y = y2 - y1;
        sy = 1;
    }
    else {
        delta_y = y1 - y2;
        sy = -1;
    }

    err = delta_x - delta_y;

    while(1) {
        fbDrawPixel(fb, x1, y1, flags);
        if(x1 == x2 && y1 == y2)
            break;
        e2 = 2 * err;
        if(e2 > -delta_y) {
            err -= delta_y;
            x1 += sx;
        }
        if(e2 < delta_x) {
            err += delta_x;
            y1 += sy;
        }
    }
}

// Draws a rectangle by specifying two opposite corners of the rectangle
void fbDrawRect(FbInfo *fb, size_t x1, size_t y1,
    size_t x2, size_t y2, uint32_t flags)
{
    fbDrawLine(fb, x1, y1, x1, y2, flags);
    fbDrawLine(fb, x1, y1, x2, y1, flags);
    fbDrawLine(fb, x1, y2, x2, y2, flags);
    fbDrawLine(fb, x2, y1, x2, y2, flags);
}

// Draws a circle around the given center point with a specific radius
void fbDrawCircle(FbInfo *fb, size_t x, size_t y,
    size_t radius, uint32_t flags)
{
    size_t i;
    int f, ddf_y, ddf_x;

    f = 1 - radius;
    ddf_x = 1;
    ddf_y = -2 * radius;
    i = 0;

    fbDrawPixel(fb, x, y + radius, flags);
    fbDrawPixel(fb, x, y - radius, flags);
    fbDrawPixel(fb, x + radius, y, flags);
    fbDrawPixel(fb, x - radius, y, flags);

    while(i < radius)
    {
        if(f >= 0) 
        {
            --radius;
            ddf_y += 2;
            f += ddf_y;
        }
        ++i;
        ddf_x += 2;
        f += ddf_x;
        fbDrawPixel(fb, x + i, y + radius, flags);
        fbDrawPixel(fb, x - i, y + radius, flags);
        fbDrawPixel(fb, x + i, y - radius, flags);
        fbDrawPixel(fb, x - i, y - radius, flags);
        fbDrawPixel(fb, x + radius, y + i, flags);
        fbDrawPixel(fb, x - radius, y + i, flags);
        fbDrawPixel(fb, x + radius, y - i, flags);
        fbDrawPixel(fb, x - radius, y - i, flags);
    }
}

// Activates clipping for the given rectangle. After calling, every
// fb_draw_* call will only draw in this area. Clipping can be
// deactivated by calling fb_clip_reset.
void fbClipRect(FbInfo *fb, size_t x1, size_t y1,
    size_t x2, size_t y2)
{
    FbRect *r = &fb->clip_rect_;

    fb->clip_ = 1;

    fbUtilFixRect(&x1, &y1, &x2, &y2);

    r->x1_ = x1;
    r->x2_ = x2;
    r->y1_ = y1;
    r->y2_ = y2;
}

// Deactivates the clipping.
void fbClipReset(FbInfo *fb)
{
    fb->clip_ = 0;
}

// Makes sure that x1 <= x2 and y1 <= y2
void fbUtilFixRect(size_t *x1, size_t *y1,
    size_t *x2, size_t *y2)
{
    size_t temp;

    if(*x1 > *x2)
    {
        temp = *x1;
        *x1 = *x2;
        *x2 = temp;
    }
    if(*y1 > *y2)
    {
        temp = *y1;
        *y1 = *y2;
        *y2 = temp;
    }
} 

// Copies one area to a different place in the framebuffer.
// x0, y0 specify the top left corner of the destination area
// Overlapping of the two areas is not supported.
void fbCopyRect(FbInfo *fb, size_t x1, size_t y1,
    size_t x2, size_t y2, size_t x0, size_t y0)
{
    size_t i, j;

    fbUtilFixRect(&x1, &y1, &x2, &y2);

    for(i = x1; i <= x2; ++i)
        for(j = y1; j <= y2; ++j)
            fbDrawPixel(fb, x0 + i - x1, y0 + j - y1,
                fbGetPixel(fb, i , j));
}

// Draws a charakter usign the given font info. x, y specify the
// top left corner of the charakter
void fbDrawCharakter(FbInfo *fb, const FontInfo *font, uint8_t c,
    size_t x, size_t y, uint32_t flags)
{
    size_t x2, y2, skip, bytes, offset, i, j;

    bytes = (font->width_ + 7) / 8;
    skip = (bytes * 8) - font->width_;
    offset = c * bytes * font->height_ * 8;

    x2 = x + font->width_;
    y2 = y + font->height_;

    for(j = y; j < y2; ++j)
    {
        for(i = x; i < x2; ++i)
        {
            if(font->data_[offset / 8] & (1 << (7 - (offset % 8))))
                fbDrawPixel(fb, i, j, flags);
            ++offset;
        }
        offset += skip;
    }
}

// Draws a string usign the given font info. x, y specify the
// top left corner of the string. Spacing is the space between
// two charakers. A negative number means charakters overlap.
void fbDrawString(FbInfo *fb, const FontInfo *font, char *s,
    size_t x, size_t y, int spacing, uint32_t flags)
{
    size_t i, dx = 0, dy = 0;

    if(flags & FB_VERT)
        dy = font->height_ + spacing;
    else
        dx = font->width_ + spacing;

    for(i = 0; s[i] != '\0'; ++i)
    {
        fbDrawCharakter(fb, font, s[i], x, y, flags);
        x += dx;
        y += dy;
    }
}
