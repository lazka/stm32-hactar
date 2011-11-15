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

static void printClockStatus(void)
{
    RCC_ClocksTypeDef test;
    RCC_GetClocksFreq(&test);

    printf("ADC   %u\n", (unsigned int)test.ADCCLK_Frequency);
    printf("HCLK  %u\n", (unsigned int)test.HCLK_Frequency);
    printf("PCLK1 %u\n", (unsigned int)test.PCLK1_Frequency);
    printf("PCLK2 %u\n", (unsigned int)test.PCLK2_Frequency);
    printf("SYS   %u\n", (unsigned int)test.SYSCLK_Frequency);
}

void startTerminal(void)
{
    TermCommand cmds[] = {
            {
                    .command_ = "clk",
                    .description_ = "Clock information",
                    .function_ = &printClockStatus,
            },
            {
                    .command_ = "help",
                    .description_ = "This command",
                    .function_ = NULL,
            },
            {
                    .command_ = "exit",
                    .description_ = "Leave terminal",
                    .function_ = NULL,
            },
    };

    char data[50];
    size_t i, size = sizeof(cmds) / sizeof(cmds[0]);

    while(1)
    {
        printf("> ");
        scanf("%s", data);

        if(strcmp(data, "exit") == 0)
        {
            printf("Bye.\n");
            break;
        }

        if(strcmp(data, "help") == 0)
        {
            printf("Available commands:\n");
            for(i = 0; i < size; i++)
            {
                printf(" %-6s - %s\n", cmds[i].command_, cmds[i].description_);
            }
            continue;
        }

        for(i = 0; i < size; i++)
        {
            if(strcmp(cmds[i].command_, data) == 0)
            {
                cmds[i].function_();
                break;
            }

            if(i == size - 1)
                printf("Unknown command\n");

        }
    }
}
