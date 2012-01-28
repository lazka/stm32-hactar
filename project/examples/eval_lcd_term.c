// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <hactar/stdio_devs/eval_lcd.h>
#include <hactar/stdio_devs/usart.h>
#include <hactar/hactar.h>

#include <stdio.h>

static void printArgs(char **args)
{
    size_t i = 0;

    while(args[i] != NULL)
    {
        iprintf("%u) '%s'\n", i + 1, args[i]);
        i++;
    }
}

void initEvalLCDTermExample(void)
{
    // Define a custom command
    TermCommand term_cmds[] = {
            {
                    .command_ = "args",
                    .description_ = "Print arguments",
                    .function_ = &printArgs,
            },
    };

    // USART for input without echo
    initUSARTStdioDevice(HACTAR_USART_STDIO_STDIN);

    // The eval board LCD display as output
    initEvalLCDStdoutDevice();

    iprintf("Eval Board LCD Terminal\n");

    // Start terminal
    startTerminal(term_cmds, COUNT_OF(term_cmds));
}
