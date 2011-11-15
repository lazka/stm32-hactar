// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTER_MISC_H__
#define HACTER_MISC_H__

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void MemManage_Handler(void);

void interruptsDisable();

void interruptsEnable();

void GPIO_GetPinConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,
        GPIO_InitTypeDef* GPIO_InitStruct);

void assert_failed(uint8_t* file, uint32_t line);

#endif
