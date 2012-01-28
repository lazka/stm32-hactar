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

int main(void)
{
    hactarInitScheduler(1);

    //initEvalLCDTermExample();         // Eval Board LCD Terminal
    //initEvalFramebufferExample();     // Eval Board Framebuffer Terminal
    //initFATFSExample();               // SD Card / FAT FS
    //initUSARTTermExample();           // USART Terminal
    //initDogm128FramebufferExample();  // DOGM128 Framebuffer Terminal

    //initUSARTStdioDevice(HACTAR_USART_STDIO_STDIN | HACTAR_USART_STDIO_STDIN_ECHO | HACTAR_USART_STDIO_STDOUT);
    //initEvalLCDStdoutDevice();
    //initFramebufferStdoutDevice(&fb, &font_4x6);

    //CardInfo info;
    //hactarSDInit(&info, 512);

    //startTerminal(term_cmds, 1);

    assert(0);
    while (1);

    return 0;
}
