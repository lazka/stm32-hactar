// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_PLATFORM_H__
#define HACTAR_PLATFORM_H__

#include "stm32f10x.h"

#include <hactar/platform_def.h>

// General config -------------------------------------------------------------

// If defined, you need the fat driver included
// Copy ffconf.h and integer.h to your project and renamve or remove them
// in the fat driver
//#define HACTAR_USE_FATFS

// Clock config ---------------------------------------------------------------

#ifdef STM32F10X_CL

// It's a bit hard to check if HSE needs to be enabled so define here
#define HACTAR_CLK_HSE

// If the device/periph is used
//#define HACTAR_CLK_DEV_WATCHDOG
//#define HACTAR_CLK_DEV_RTC
//#define HACTAR_CLK_DEV_USB
//#define HACTAR_CLK_DEV_I2S2
//#define HACTAR_CLK_DEV_I2S3

// Muxers
#define HACTAR_CLK_MUX_RTC     HACTAR_CLK_MUX_RTC_SRC_HSE
#define HACTAR_CLK_MUX_PREDIV1 HACTAR_CLK_MUX_PREDIV1_SRC_PLL2MUL
#define HACTAR_CLK_MUX_PLL     HACTAR_CLK_MUX_PLL_SRC_PREDIV1
#define HACTAR_CLK_MUX_SW      HACTAR_CLK_MUX_SW_SRC_PLLMUL
#define HACTAR_CLK_MUX_STK     HACTAR_CLK_MUX_STK_SRC_DIV8
#define HACTAR_CLK_MUX_I2S2    HACTAR_CLK_MUX_I2S2_SRC_SYSCLK
#define HACTAR_CLK_MUX_I2S3    HACTAR_CLK_MUX_I2S3_SRC_SYSCLK

// Frequency multipliers / dividers
#define HACTAR_CLK_SCALE_PREDIV2 5  // 1..16
#define HACTAR_CLK_SCALE_PREDIV1 5  // 1..16
#define HACTAR_CLK_SCALE_PLLMUL  90 // 4,5,6,7,8,9 and 6.5 (x10)
#define HACTAR_CLK_SCALE_PLL2MUL 8  // 8..14,16,20
#define HACTAR_CLK_SCALE_PLL3MUL 9  // 8..14,16,20
#define HACTAR_CLK_SCALE_AHB     1  // 1..512
#define HACTAR_CLK_SCALE_APB1    2  // 1,2,4,8,16
#define HACTAR_CLK_SCALE_APB2    1  // 1,2,4,8,16
#define HACTAR_CLK_SCALE_ADC     6  // 2,4,6,8
#define HACTAR_CLK_SCALE_USB     3  // 2,3

#else

#define HACTAR_CLK_HSI
#define HACTAR_CLK_HSE
#define HACTAR_CLK_LSI
#define HACTAR_CLK_LSE

#define HACTAR_CLK_MUX_RTCS
#define HACTAR_CLK_MUX_PLLXTPRE
#define HACTAR_CLK_MUX_PLL
#define HACTAR_CLK_MUX_SW
#define HACTAR_CLK_MUX_STK

#define HACTAR_CLK_SCALE_PLLMULL
#define HACTAR_CLK_SCALE_AHB
#define HACTAR_CLK_SCALE_APB1
#define HACTAR_CLK_SCALE_APB2
#define HACTAR_CLK_SCALE_ADC
#define HACTAR_CLK_SCALE_USB

#endif

#endif
