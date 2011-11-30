// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stdio.h>

#include "stm32f10x_rcc.h"

#include <hactar/stdio_devs/usart.h>
#include <hactar/misc.h>
#include <hactar/platform_check.h>

static int writeUSARTStdout(char *ptr, int len, uint8_t err)
{
    size_t i;

    for(i = 0; i < len; i++)
    {
       HACTAR_STDOUT_USART->DR = *ptr++;
       while(!(HACTAR_STDOUT_USART->SR & USART_FLAG_TXE));
    }

    return len;
}

static int readUSARTStdin(char *ptr, int len)
{
    size_t i;

retry:

    for(i = 0; (HACTAR_STDOUT_USART->SR & USART_FLAG_RXNE) && (i < len); i++)
    {
        ptr[i] = HACTAR_STDOUT_USART->DR;
        if(usart_stdin_device.echo_)
            putchar(ptr[i]);
    }

    if(i == 0)
    {
        __WFI();
        goto retry;
    }

    return i;
}


void initUSARTStdioDevice(uint32_t flags)
{
    assert(flags);

    // FIXME based on config

    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

    if (HACTAR_STDOUT_USART == USART2) // FIXME: ...
    {
      /* Enable the USART2 Pins Software Remapping */
      GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    }

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = HACTAR_STDOUT_USART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HACTAR_STDOUT_USART_PORT, &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = HACTAR_STDOUT_USART_RX_PIN;
    GPIO_Init(HACTAR_STDOUT_USART_PORT, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* USART configuration */
    USART_Init(HACTAR_STDOUT_USART, &USART_InitStructure);

    /* Enable USART */
    USART_Cmd(HACTAR_STDOUT_USART, ENABLE);

    if(flags & HACTAR_USART_STDIO_STDOUT)
    {
        usart_stdout_device.write_func_ = &writeUSARTStdout;
        usart_stdout_device.clear_func_ = NULL;
        stdout_device = &usart_stdout_device;
    }

    if(flags & HACTAR_USART_STDIO_STDIN)
    {
        usart_stdin_device.device_.read_func_ = &readUSARTStdin;
        stdin_device = (StdinDevice*)&usart_stdin_device;
        usart_stdin_device.echo_ = !!(flags & HACTAR_USART_STDIO_STDIN_ECHO);
    }

    // enabled interrupts
    /*USART_ITConfig(HACTAR_STDOUT_USART, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef nvicConfiguration;
    nvicConfiguration.NVIC_IRQChannel = EVAL_COM1_IRQn;
    nvicConfiguration.NVIC_IRQChannelPreemptionPriority = 15;
    nvicConfiguration.NVIC_IRQChannelSubPriority = 15;
    nvicConfiguration.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicConfiguration);*/
}
