// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef __HACTAR_PLATFORM_CHECK
#error "Never include platform.h directly, use platform_check.h instead!"
#endif

#ifndef HACTAR_PLATFORM_DEF_H__
#define HACTAR_PLATFORM_DEF_H__

// LD/MD/HD/XL
#if defined STM32F10X_LD || defined STM32F10X_MD || defined STM32F10X_HD || \
    defined STM32F10X_XL
#define HACTAR_DEV_NO_CL
#endif

// Value Line
#if defined STM32F10X_LD_VL || defined STM32F10X_MD_VL || STM32F10X_HD_VL
#define HACTER_DEV_VL
#endif

// Connectivity Line
#ifdef STM32F10X_CL
#define HACTAR_DEV_CL
#endif

#if (defined HACTAR_DEV_CL && defined HACTAR_DEV_NO_CL && HACTER_DEV_VL) || \
    !(defined HACTAR_DEV_CL || defined HACTAR_DEV_NO_CL || HACTER_DEV_VL)
#error "Only one device class can/has to be active"
#endif

/*
 * There are 3 main device series: LD/MD/HD/XL, Value and Connectivity
 * While the IRQ handling for Value is implemented, the startup code
 * is not (because I couldn't test it), so not supported atm.
 */

#ifdef HACTER_DEV_VL
#error "No support for value line devices.. sorry.."
#endif

// Muxer state defines
#ifdef HACTAR_DEV_CL

#define HACTAR_CLK_MUX_RTC_SRC_LSE         (1 << 0)
#define HACTAR_CLK_MUX_RTC_SRC_LSI         (1 << 1)
#define HACTAR_CLK_MUX_RTC_SRC_HSE         (1 << 2)

#define HACTAR_CLK_MUX_PREDIV1_SRC_HSE     (1 << 0)
#define HACTAR_CLK_MUX_PREDIV1_SRC_PLL2MUL (1 << 1)

#define HACTAR_CLK_MUX_PLL_SRC_PREDIV1     (1 << 0)
#define HACTAR_CLK_MUX_PLL_SRC_DIV2        (1 << 1)

#define HACTAR_CLK_MUX_SW_SRC_PLLMUL       (1 << 0)
#define HACTAR_CLK_MUX_SW_SRC_HSI          (1 << 1)
#define HACTAR_CLK_MUX_SW_SRC_HSE          (1 << 2)

#define HACTAR_CLK_MUX_STK_SRC_AHBPRE      (1 << 0)
#define HACTAR_CLK_MUX_STK_SRC_DIV8        (1 << 1)

#define HACTAR_CLK_MUX_I2S2_SRC_SYSCLK     (1 << 0)
#define HACTAR_CLK_MUX_I2S2_SRC_PLL3MUL    (1 << 1)

#define HACTAR_CLK_MUX_I2S3_SRC_SYSCLK     (1 << 0)
#define HACTAR_CLK_MUX_I2S3_SRC_MUL2       (1 << 1)

#elif defined HACTAR_DEV_NO_CL

#define HACTAR_CLK_MUX_RTC_SRC_LSE         (1 << 0)
#define HACTAR_CLK_MUX_RTC_SRC_LSI         (1 << 1)
#define HACTAR_CLK_MUX_RTC_SRC_HSE         (1 << 2)

#define HACTAR_CLK_MUX_PLLXTPRE_SRC_DIV2   (1 << 0)
#define HACTAR_CLK_MUX_PLLXTPRE_SRC_HSE    (1 << 1)

#define HACTAR_CLK_MUX_PLL_SRC_HSE         (1 << 0)
#define HACTAR_CLK_MUX_PLL_SRC_HSI         (1 << 1)

#define HACTAR_CLK_MUX_SW_SRC_HSI          (1 << 0)
#define HACTAR_CLK_MUX_SW_SRC_PLLMUL       (1 << 1)
#define HACTAR_CLK_MUX_SW_SRC_HSE          (1 << 2)

#define HACTAR_CLK_MUX_STK_SRC_AHBPRE      (1 << 0)
#define HACTAR_CLK_MUX_STK_SRC_DIV8        (1 << 1)

#endif

#endif
