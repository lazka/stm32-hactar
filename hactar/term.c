// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"

#include "term.h"

static void printClockStatus(char **args)
{
    RCC_ClocksTypeDef test;
    RCC_GetClocksFreq(&test);

    iprintf("ADC   %u\n", (unsigned int)test.ADCCLK_Frequency);
    iprintf("HCLK  %u\n", (unsigned int)test.HCLK_Frequency);
    iprintf("PCLK1 %u\n", (unsigned int)test.PCLK1_Frequency);
    iprintf("PCLK2 %u\n", (unsigned int)test.PCLK2_Frequency);
    iprintf("SYS   %u\n", (unsigned int)test.SYSCLK_Frequency);
}

static void printArgs(char **args)
{
    size_t i = 0;

    while(args[i] != NULL)
    {
        iprintf("%u) '%s'\n", i + 1, args[i]);
        i++;
    }
}

static void getCommand(char *dest, char **args, size_t count, size_t arg_count)
{
    char* start_arg = dest;
    size_t current_arg = 0;

    while(1)
    {
        *dest = getchar();

        if(*dest == '\n' || *dest == '\r' || *dest == ' ' || *dest == EOF)
        {
            if(current_arg < (arg_count - 1) && start_arg <= dest)
            {
                args[current_arg++] = start_arg;
                start_arg = dest + 1;
            }

            if(*dest != ' ')
            {
                args[current_arg] = NULL;
                *dest = '\0';
                break;
            }

            *dest = '\0';
        }

        if(count == 0)
            continue;

        count--;
        dest++;
    }
}

void startTerminal(void)
{
    const TermCommand cmds[] = {
            {
                    .command_ = "clk",
                    .description_ = "Clock information",
                    .function_ = &printClockStatus,
            },
            {
                    .command_ = "args",
                    .description_ = "Print arguments",
                    .function_ = &printArgs,
            },
            {
                    .command_ = "help",
                    .description_ = "Command list",
                    .function_ = NULL,
            },
            {
                    .command_ = "exit",
                    .description_ = "Exit terminal",
                    .function_ = NULL,
            },
    };

    char input[HACTAR_TERM_INPUT_BUFFER_SIZE];
    char *args[HACTAR_TERM_INPUT_ARG_COUNT];
    size_t i, size = sizeof(cmds) / sizeof(cmds[0]);

    while(1)
    {
        fflush(stdout);
        fiprintf(stderr, "> ");
        fflush(stderr);

        getCommand(input, args,
                  HACTAR_TERM_INPUT_BUFFER_SIZE,
                  HACTAR_TERM_INPUT_ARG_COUNT);

        if(strcmp(args[0], "exit") == 0)
            break;

        if(strcmp(args[0], "help") == 0)
        {
            for(i = 0; i < size; i++)
            {
                iprintf("%-5s: %s\n", cmds[i].command_, cmds[i].description_);
            }
            continue;
        }

        for(i = 0; i < size; i++)
        {
            if(strcmp(cmds[i].command_, args[0]) == 0)
            {
                cmds[i].function_(&args[1]);
                break;
            }

            if(i == size - 1)
            {
                fflush(stdout);
                fiprintf(stderr, "Unknown command\n");
            }

        }
    }
}
