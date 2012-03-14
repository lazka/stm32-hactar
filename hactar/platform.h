// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_PLATFORM_H__
#define HACTAR_PLATFORM_H__

#include <hactar/platform_def.h>

// General config -------------------------------------------------------------

// If defined, you need the fat driver included
// Copy ffconf.h and integer.h to your project and rename or remove them
// in the fat driver
//#define HACTAR_USE_FATFS

// If defined not debug functionality like assert
//#define NDEBUG

// Scheduler ------------------------------------------------------------------

// Maximum number of active threads
#define HACTAR_N_THREADS (10)
// Needed if you use stdlib functions from more than one thread
// (interrupts included).
#define HACTAR_NEWLIB_REENT

// If defined, pause the responsible thread and continue execution
#define HACTAR_CATCH_BUSFAULT

// Clock config ---------------------------------------------------------------

#ifdef HACTAR_DEV_CL // STM32F105/7

#define HACTAR_CLK_MUX_RTC      HACTAR_CLK_MUX_RTC_SRC_HSE
#define HACTAR_CLK_MUX_PREDIV1  HACTAR_CLK_MUX_PREDIV1_SRC_PLL2MUL
#define HACTAR_CLK_MUX_PLL      HACTAR_CLK_MUX_PLL_SRC_PREDIV1
#define HACTAR_CLK_MUX_SW       HACTAR_CLK_MUX_SW_SRC_PLLMUL
#define HACTAR_CLK_MUX_STK      HACTAR_CLK_MUX_STK_SRC_AHBPRE
#define HACTAR_CLK_MUX_I2S2     HACTAR_CLK_MUX_I2S2_SRC_SYSCLK
#define HACTAR_CLK_MUX_I2S3     HACTAR_CLK_MUX_I2S3_SRC_SYSCLK

#define HACTAR_CLK_SCALE_PREDIV2  5  // 1..16
#define HACTAR_CLK_SCALE_PREDIV1  5  // 1..16
#define HACTAR_CLK_SCALE_PLLMUL   90 // 4,5,6,7,8,9 and 6.5 (x10)
#define HACTAR_CLK_SCALE_PLL2MUL  8  // 8..14,16,20
#define HACTAR_CLK_SCALE_PLL3MUL  9  // 8..14,16,20
#define HACTAR_CLK_SCALE_AHB      1  // 1,2,4,8,16,64,128,256,512
#define HACTAR_CLK_SCALE_APB1     2  // 1,2,4,8,16
#define HACTAR_CLK_SCALE_APB2     1  // 1,2,4,8,16
#define HACTAR_CLK_SCALE_ADC      6  // 2,4,6,8
#define HACTAR_CLK_SCALE_USB      3  // 2,3

#elif defined HACTAR_DEV_NO_CL // STM32F101/2/3

#define HACTAR_CLK_MUX_RTC       HACTAR_CLK_MUX_RTC_SRC_HSE
#define HACTAR_CLK_MUX_PLLXTPRE  HACTAR_CLK_MUX_PLLXTPRE_SRC_DIV2
#define HACTAR_CLK_MUX_PLL       HACTAR_CLK_MUX_PLL_SRC_HSE
#define HACTAR_CLK_MUX_SW        HACTAR_CLK_MUX_SW_SRC_PLLMUL
#define HACTAR_CLK_MUX_STK       HACTAR_CLK_MUX_STK_SRC_DIV8

#define HACTAR_CLK_SCALE_PLLMUL  9  // 2..16
#define HACTAR_CLK_SCALE_AHB     1  // 1,2,4,8,16,64,128,256,512
#define HACTAR_CLK_SCALE_APB1    2  // 1,2,4,8,16
#define HACTAR_CLK_SCALE_APB2    1  // 1,2,4,8,16
#define HACTAR_CLK_SCALE_ADC     6  // 2,4,6,8
#define HACTAR_CLK_SCALE_USB     15 // 1,1.5 (x10)

#elif defined HACTER_DEV_VL // STM32F100

#error "nothing here yet"

#endif

#endif
