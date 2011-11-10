// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "stm32f10x.h"

#include "platform.h"
#include "stdout.h"

static int writeUSARTStdout(char *ptr, int len)
{
    size_t i, shift = 0;
    uint32_t parity, bits9;
    uint8_t carry = 0, c;

    bits9 = !!(HACTAR_STDOUT_USART->CR1 & 0x1000);
    parity = !!(HACTAR_STDOUT_USART->CR1 & 0x400);

    for(i = 0; i < len; i++)
    {
       c = (uint8_t) *ptr++;
       HACTAR_STDOUT_USART->DR = (c << shift) | carry;
       while(!(HACTAR_STDOUT_USART->SR & USART_FLAG_TC));

       if(!bits9 && parity) // FIXME: gtkterm doesn't like it yet
       {
           carry = (c >> (7 - shift));
           shift++;
           if(shift >= 8)
           {
               shift = 0;
               HACTAR_STDOUT_USART->DR = carry;
               while(!(HACTAR_STDOUT_USART->SR & USART_FLAG_TC));
           }
       }
    }

    return len;
}

void initUSARTStdoutDevice(void)
{
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

    usart_stdout_device.write_func_ = &writeUSARTStdout;
    stdout_device = &usart_stdout_device;

    // enabled interrupts
    /*USART_ITConfig(HACTAR_STDOUT_USART, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef nvicConfiguration;
    nvicConfiguration.NVIC_IRQChannel = EVAL_COM1_IRQn;
    nvicConfiguration.NVIC_IRQChannelPreemptionPriority = 15;
    nvicConfiguration.NVIC_IRQChannelSubPriority = 15;
    nvicConfiguration.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicConfiguration);*/
}
