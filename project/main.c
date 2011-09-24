/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO/STM32
**                STMicroelectronics STM32F10x Standard Peripherals Library
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the Atollic TrueSTUDIO(R) toolchain.
**
**
*****************************************************************************
*/

/* Includes */
#include <stddef.h>
#include "stm32f10x.h"
#include <stdlib.h>
#include <stdio.h>


#include "stm32_eval.h"
#include "stm3210c_eval_lcd.h"
#include "stm32_eval_i2c_ee.h"

#define USE_BOARD
#define USE_LED
#define USE_SEE

#include "startup.h"

void SysTick_Handler(void)
{
  STM_EVAL_LEDToggle(LED2);
}

void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    STM_EVAL_LEDToggle(LED3);
  }
}

void config_timer(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1000 - 1;  // 1 MHz down to 1 KHz (1 ms)
  TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM IT enable */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}

/* Private macro */
/* Private variables */
 USART_InitTypeDef USART_InitStructure;

/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
  int i = 0;

  /* Initialize LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Turn on LEDs */
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
  STM_EVAL_LEDOff(LED3);

  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  SysTick_Config(9000000);

  //config_timer();

#ifdef USE_BOARD
  STM3210C_LCD_Init();

  /* Display message on STM3210X-EVAL LCD */
  /* Clear the LCD */
  LCD_Clear(White);

  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
  /*LCD_DisplayStringLine(Line0, (uint8_t *)MESSAGE1);
  LCD_DisplayStringLine(Line1, (uint8_t *)MESSAGE2);
  LCD_DisplayStringLine(Line2, (uint8_t *)MESSAGE3);
  LCD_DisplayStringLine(Line3, (uint8_t *)MESSAGE4);
  LCD_DisplayStringLine(Line4, (uint8_t *)MESSAGE5);
  LCD_DisplayStringLine(Line5, (uint8_t *)MESSAGE6);*/

  /* USARTx configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  //STM_EVAL_COMInit(COM1, &USART_InitStructure);
#endif

  /* TODO - Add your application code here */

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

  /* Infinite loop */
  while (1)
  {
  i++;
  //STM_EVAL_LEDToggle(LED1);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}

#ifdef USE_SEE
#ifndef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval sEE_FAIL.
  */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* Return with error code */
  return sEE_FAIL;
}
#endif
#endif /* USE_SEE */

