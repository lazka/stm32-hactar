// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "stm32f10x.h"
#include "ringbuf.h"
#include <hactar/hactar.h>

#define STACK_SIZE (300)
#define BUFFER_SIZE (200)

RingBuffer send_buffer;
RingBuffer receive_buffer;

void USART2_IRQHandler(void)
{
    uint8_t temp;

    if(USART_GetITStatus(HACTAR_STDOUT_USART, USART_IT_RXNE) != RESET)
    {
        temp = (USART_ReceiveData(HACTAR_STDOUT_USART) & 0x7F);
        ringBufferWriteIRQ(&receive_buffer, &temp, 1);
    }

    if(USART_GetITStatus(HACTAR_STDOUT_USART, USART_IT_TXE) != RESET)
    {
        ringBufferReadIRQ(&send_buffer, &temp, 1);
        USART_SendData(HACTAR_STDOUT_USART, temp);
    }
}

static void sender(void)
{
    uint8_t temp[6] = "01234";

    while(1)
        ringBufferWrite(&send_buffer, temp, 5);
}

static void receiver(void)
{
    uint8_t temp[3];
    while(1)
        ringBufferRead(&receive_buffer, temp, 3);
}

static void very_important_work(void)
{
    while(1);
}

void initRingBufferExample(void)
{
    // Three threads
    Thread thread;
    uint8_t stack[STACK_SIZE];
    threadAdd(&thread, &very_important_work, stack, STACK_SIZE);

    Thread thread2;
    uint8_t stack2[STACK_SIZE];
    threadAdd(&thread2, &sender, stack2, STACK_SIZE);

    Thread thread3;
    uint8_t stack3[STACK_SIZE];
    threadAdd(&thread3, &receiver, stack3, STACK_SIZE);

    // A buffer for receiving and one for sending
    uint8_t send_data[BUFFER_SIZE];
    ringBufferInit(&send_buffer, send_data, BUFFER_SIZE, true);

    uint8_t receive_data[BUFFER_SIZE];
    ringBufferInit(&receive_buffer, receive_data, BUFFER_SIZE, true);

    // Set up USART and the interrupt
    NVIC_InitTypeDef NVIC_InitStructure = {
            .NVIC_IRQChannel = USART2_IRQn,
            .NVIC_IRQChannelPreemptionPriority = 0,
            .NVIC_IRQChannelSubPriority = 0,
            .NVIC_IRQChannelCmd = ENABLE,
    };
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_AFIO, ENABLE);

    if (HACTAR_STDOUT_USART == USART2)
    {
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

    USART_ITConfig(HACTAR_STDOUT_USART, USART_IT_TXE, ENABLE);
    USART_ITConfig(HACTAR_STDOUT_USART, USART_IT_RXNE, ENABLE);

    // go go go
    schedulerInit(50);
}
