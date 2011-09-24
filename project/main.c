#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "stm32f10x.h"

#include "stm32_eval.h"
#include "stm3210c_eval_lcd.h"
#include "stm32_eval_i2c_ee.h"

#include "startup.h"

void SysTick_Handler(void)
{
  STM_EVAL_LEDToggle(LED2);
}

int main(void)
{
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
  STM_EVAL_LEDOff(LED3);

  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  SysTick_Config(9000000);

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

  assert_param(0);

  while(1);
}

void assert_failed(uint8_t* file, uint32_t line)
{
    char line_buffer[20];
    char *assert = "ASSERT";

    LCD_SetBackColor(LCD_COLOR_BLACK);
    LCD_SetTextColor(LCD_COLOR_RED);

    sprintf(line_buffer, "Line %u", (unsigned int)line);
    LCD_ClearLine(LINE(7));
    LCD_ClearLine(LINE(8));
    LCD_ClearLine(LINE(9));
    LCD_DisplayStringLine(LINE(7), (uint8_t *)assert);
    LCD_DisplayStringLine(LINE(8), (uint8_t *)file);
    LCD_DisplayStringLine(LINE(9), (uint8_t *)line_buffer);

    int32_t x=0, y=0;

    LCD_SetTextColor(LCD_COLOR_GREY);
    LCD_DrawUniLine(x + 10, y + 40, x + 80, y + 70);
    LCD_DrawUniLine(x + 80, y + 70, x + 50, y + 20);
    LCD_DrawUniLine(x + 50, y + 20, x + 120, y + 60);
    LCD_DrawUniLine(x + 120, y + 60, x + 118, y + 48);
    LCD_DrawUniLine(x + 120, y + 60, x + 110, y + 65);

    while (1);
}
