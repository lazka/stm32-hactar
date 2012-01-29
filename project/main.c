// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stdio.h>

#include <hactar/hactar.h>

#include "examples/eval_lcd_term.h"
#include "examples/eval_framebuffer.h"
#include "examples/dogm128_framebuffer.h"
#include "examples/usart_term.h"
#include "examples/fat_fs.h"
#include "examples/fb_drawing.h"

#include <ff.h>

int main(void)
{
    hactarInitScheduler(1);

    //initEvalLCDTermExample();         // Eval Board LCD Terminal
    //initEvalFramebufferExample();     // Eval Board Framebuffer Terminal
    //initFATFSExample();               // SD Card / FAT FS
    //initUSARTTermExample();           // USART Terminal
    //initDogm128FramebufferExample();  // DOGM128 Framebuffer Terminal
    initFramebufferDrawingExample();  // Framebuffer drawing

    assert(0);
    while (1);

    return 0;
}
