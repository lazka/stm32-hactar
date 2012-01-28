// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "usart_term.h"

#include <hactar/stdio_devs/usart.h>
#include <hactar/hactar.h>

#include <stdio.h>

void initUSARTTermExample(void)
{
    // USART for output, input + echo
    initUSARTStdioDevice(HACTAR_USART_STDIO_STDIN | \
                         HACTAR_USART_STDIO_STDOUT | \
                         HACTAR_USART_STDIO_STDIN_ECHO);

    iprintf("USART Terminal\n");

    // Start terminal
    startTerminal(NULL, 0);
}
