// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

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

static void printMemoryStatus(char **args)
{
    extern char _spretext, _epretext;
    extern char _srodata, _erodata, _etext;
    extern char _sdata, _edata;
    extern char _sbss, _ebss;
    extern char _end;

    iprintf("Flash:\n");
    iprintf(" Text   %5u Bytes\n", (size_t)&_epretext - (size_t)&_spretext);
    iprintf(" ROData %5u Bytes\n", (size_t)&_erodata - (size_t)&_srodata);

    iprintf("RAM:\n");
    iprintf(" Data   %5u Bytes\n", (size_t)&_edata - (size_t)&_sdata);
    iprintf(" BSS    %5u Bytes\n", (size_t)&_ebss - (size_t)&_sbss);
    iprintf(" Heap   %5u Bytes\n", (size_t)sbrk(0) - (size_t)&_end);
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

void startTerminal(TermCommand* user_cmds, size_t num_user_cmds)
{
    static const TermCommand cmds[] = {
            {
                    .command_ = "clk",
                    .description_ = "Clock information",
                    .function_ = &printClockStatus,
            },
            {
                    .command_ = "mem",
                    .description_ = "Memory status",
                    .function_ = &printMemoryStatus,
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
    size_t i, num_cmds = sizeof(cmds) / sizeof(cmds[0]);

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
            char *help_format = "%-5s: %s\n";
            for(i = 0; i < num_cmds; i++)
            {
                iprintf(help_format, cmds[i].command_, cmds[i].description_);
            }

            if(user_cmds == NULL)
                continue;

            iprintf("--------------------------\n");

            for(i = 0; i < num_user_cmds; i++)
            {
                iprintf(help_format, user_cmds[i].command_,
                        user_cmds[i].description_);
            }

            continue;
        }

        uint32_t found = 0;

        // User commands first so new std commands don't override them
        for(i = 0; i < num_user_cmds && user_cmds != NULL; i++)
        {
            if(strcmp(user_cmds[i].command_, args[0]) == 0)
            {
                user_cmds[i].function_(&args[1]);
                found = 1;
                break;
            }
        }

        if(found)
            continue;

        // Now the builtin ones
        for(i = 0; i < num_cmds; i++)
        {
            if(strcmp(cmds[i].command_, args[0]) == 0)
            {
                cmds[i].function_(&args[1]);
                found = 1;
                break;
            }
        }

        if(!found)
        {
            fflush(stdout);
            fiprintf(stderr, "Unknown command\n");
        }
    }
}
