// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTER_MISC_H__
#define HACTER_MISC_H__

#include <stdint.h>
#include <stddef.h>
#include <hactar/assert.h>

#include "stm32f10x.h"

#define COUNT_OF(array)     (sizeof(array) / sizeof(array[0]))
#define MIN(X, Y)           ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y)           ((X) > (Y) ? (X) : (Y))
#define WEAK                __attribute__ ((weak))
#define NAKED               __attribute__ ((naked))
#define STRINGIFY(s)        #s
#define LIKELY(x)           __builtin_expect((x),1)
#define UNLIKELY(x)         __builtin_expect((x),0)

void GPIO_GetPinConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,
        GPIO_InitTypeDef* GPIO_InitStruct);

#endif
