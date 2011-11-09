// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "stm32f10x.h"

#include "stm32_eval.h"
#include "stm3210c_eval_lcd.h"
#include "stm32_eval_i2c_ee.h"

#include "hactar/hactar.h"

//#include "hactar/display_st7565r.h"

void hactarInitUSART(void)
{
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    STM_EVAL_COMInit(COM1, &USART_InitStructure);
}

int main(void)
{
  //DisplayInfo *display = (DisplayInfo*)&st7565r;
  //displayInit(display, 64, 64);

  spinlock_t lock;

  hactarSpinInit(&lock);
  hactarSpinLock(&lock);
  hactarSpinTrylock(&lock);
  hactarSpinUnlock(&lock);
  hactarSpinLock(&lock);

  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
  STM_EVAL_LEDOff(LED3);

  hactarInitUSART();
  hactarInitScheduler(1);

  printf("Hello World\n");

  STM3210C_LCD_Init();

  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);

  RCC_ClocksTypeDef test;
  RCC_GetClocksFreq(&test);
  char buffer[5][20];

  sprintf(buffer[0], "ADC %u ", (unsigned int)test.ADCCLK_Frequency);
  sprintf(buffer[1], "H %u ", (unsigned int)test.HCLK_Frequency);
  sprintf(buffer[2], "P1 %u ", (unsigned int)test.PCLK1_Frequency);
  sprintf(buffer[3], "P2 %u ", (unsigned int)test.PCLK2_Frequency);
  sprintf(buffer[4], "SYS %u ", (unsigned int)test.SYSCLK_Frequency);
  sprintf(buffer[5], "SYS2 %u ", (unsigned int)hactarGetSystemClock());

  int l;
  for(l=0; l < 6; l++)
      LCD_DisplayStringLine(LINE(l), (uint8_t *)buffer[l]);

  assert(0);

  while (1)
      __WFI();
}
