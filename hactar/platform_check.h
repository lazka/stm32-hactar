// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_PLATFORM_CHECK_H__
#define HACTAR_PLATFORM_CHECK_H__

/*
 * Includes the platform.h and checks all defines for errors.
 * Also defines bit masks for multiplexers.
 */

// So we can only include it from here without an error
#define __HACTAR_PLATFORM_CHECK
#include "platform.h"
#undef __HACTAR_PLATFORM_CHECK

// Connectivity Line -----------------------------------------------------------

#ifdef HACTAR_DEV_CL

// get the bit mask for SW_SRC
#if HACTAR_CLK_MUX_SW == HACTAR_CLK_MUX_SW_SRC_HSE
#define HACTAR_CLK_MUX_SW_SRC_MASK RCC_SYSCLKSource_HSE
#elif HACTAR_CLK_MUX_SW == HACTAR_CLK_MUX_SW_SRC_HSI
#define HACTAR_CLK_MUX_SW_SRC_MASK RCC_SYSCLKSource_HSI
#elif HACTAR_CLK_MUX_SW == HACTAR_CLK_MUX_SW_SRC_PLLMUL
#define HACTAR_CLK_MUX_SW_SRC_MASK RCC_SYSCLKSource_PLLCLK
#else
#error HACTAR_CLK_MUX_SW_SRC_MASK: invalid value
#endif

// get the bit mask for RTC_SRC
#if HACTAR_CLK_MUX_RTC == HACTAR_CLK_MUX_RTC_SRC_LSE
#define HACTAR_CLK_MUX_RTC_SRC_MASK RCC_RTCCLKSource_LSE
#elif HACTAR_CLK_MUX_RTC == HACTAR_CLK_MUX_RTC_SRC_LSI
#define HACTAR_CLK_MUX_RTC_SRC_MASK RCC_RTCCLKSource_LSI
#elif HACTAR_CLK_MUX_RTC == HACTAR_CLK_MUX_RTC_SRC_HSE
#define HACTAR_CLK_MUX_RTC_SRC_MASK RCC_RTCCLKSource_HSE_Div128
#else
#error HACTAR_CLK_MUX_RTC: invalid value
#endif

// check PREDIV1
#if HACTAR_CLK_MUX_PREDIV1 != HACTAR_CLK_MUX_PREDIV1_SRC_HSE && \
    HACTAR_CLK_MUX_PREDIV1 != HACTAR_CLK_MUX_PREDIV1_SRC_PLL2MUL
#error HACTAR_CLK_MUX_PREDIV1: invalid value
#endif

// check PLL src
#if HACTAR_CLK_MUX_PLL != HACTAR_CLK_MUX_PLL_SRC_DIV2 && \
    HACTAR_CLK_MUX_PLL != HACTAR_CLK_MUX_PLL_SRC_PREDIV1
#error HACTAR_CLK_MUX_PLL: invalid value
#endif

//check I2S2 src
#if HACTAR_CLK_MUX_I2S2 != HACTAR_CLK_MUX_I2S2_SRC_PLL3MUL && \
    HACTAR_CLK_MUX_I2S2 != HACTAR_CLK_MUX_I2S2_SRC_SYSCLK
#error HACTAR_CLK_MUX_I2S2: invalid value
#endif

// check I2S3 src
#if HACTAR_CLK_MUX_I2S3 != HACTAR_CLK_MUX_I2S3_SRC_MUL2 && \
    HACTAR_CLK_MUX_I2S3 != HACTAR_CLK_MUX_I2S3_SRC_SYSCLK
#error HACTAR_CLK_MUX_I2S3: invalid value
#endif

// check PLLMUL value
#if (HACTAR_CLK_SCALE_PLLMUL != 40) && (HACTAR_CLK_SCALE_PLLMUL != 50) && \
    (HACTAR_CLK_SCALE_PLLMUL != 60) && (HACTAR_CLK_SCALE_PLLMUL != 70) && \
    (HACTAR_CLK_SCALE_PLLMUL != 80) && (HACTAR_CLK_SCALE_PLLMUL != 90) && \
    (HACTAR_CLK_SCALE_PLLMUL != 65)
#error HACTAR_CLK_SCALE_PLLMUL: invalid value
#endif

// check usb scale value
#if (HACTAR_CLK_SCALE_USB != 2) && (HACTAR_CLK_SCALE_USB != 3)
#error HACTAR_CLK_SCALE_USB: invalid value
#endif

// check prediv1 value
#if (HACTAR_CLK_SCALE_PREDIV1 > 16) || (HACTAR_CLK_SCALE_PREDIV1 < 1)
#error HACTAR_CLK_SCALE_PREDIV1: invalid value
#endif

// check prediv1 value
#if (HACTAR_CLK_SCALE_PREDIV2 > 16) || (HACTAR_CLK_SCALE_PREDIV2 < 1)
#error HACTAR_CLK_SCALE_PREDIV2: invalid value
#endif

// check pll2mul value
#if (HACTAR_CLK_SCALE_PLL2MUL < 8) || \
    ((HACTAR_CLK_SCALE_PLL2MUL > 16) && (HACTAR_CLK_SCALE_PLL2MUL != 20))
#error HACTAR_CLK_SCALE_PLL2MUL: invalid value
#endif

// check pll3mul value
#if (HACTAR_CLK_SCALE_PLL3MUL < 8) || \
    ((HACTAR_CLK_SCALE_PLL3MUL > 16) && (HACTAR_CLK_SCALE_PLL3MUL != 20))
#error HACTAR_CLK_SCALE_PLL3MUL: invalid value
#endif

// check apb1 scale value
#if (HACTAR_CLK_SCALE_USB != 2) && (HACTAR_CLK_SCALE_USB != 3)
#error HACTAR_CLK_SCALE_USB: invalid value
#endif

#endif // HACTAR_DEV_CL

// LD/MD/HD/XL -----------------------------------------------------------------

#ifdef HACTAR_DEV_NO_CL

// get the bit mask for RTC_SRC
#if HACTAR_CLK_MUX_RTC == HACTAR_CLK_MUX_RTC_SRC_LSE
#define HACTAR_CLK_MUX_RTC_SRC_MASK RCC_RTCCLKSource_LSE
#elif HACTAR_CLK_MUX_RTC == HACTAR_CLK_MUX_RTC_SRC_LSI
#define HACTAR_CLK_MUX_RTC_SRC_MASK RCC_RTCCLKSource_LSI
#elif HACTAR_CLK_MUX_RTC == HACTAR_CLK_MUX_RTC_SRC_HSE
#define HACTAR_CLK_MUX_RTC_SRC_MASK RCC_RTCCLKSource_HSE_Div128
#else
#error HACTAR_CLK_MUX_RTC: invalid value
#endif

// get the bit mask for PLLXTPRE_SRC
#if HACTAR_CLK_MUX_PLLXTPRE == HACTAR_CLK_MUX_PLLXTPRE_SRC_DIV2
#define HACTAR_CLK_MUX_PLLXTPRE_SRC_MASK RCC_CFGR_PLLXTPRE_HSE_Div2
#elif HACTAR_CLK_MUX_PLLXTPRE == HACTAR_CLK_MUX_PLLXTPRE_SRC_HSE
#define HACTAR_CLK_MUX_PLLXTPRE_SRC_MASK RCC_CFGR_PLLXTPRE_HSE
#else
#error HACTAR_CLK_MUX_PLLXTPRE: invalid value
#endif

// get the bit mask for PLL_SRC
#if HACTAR_CLK_MUX_PLL == HACTAR_CLK_MUX_PLL_SRC_HSE
#define HACTAR_CLK_MUX_PLL_SRC_MASK RCC_CFGR_PLLSRC_HSE
#elif HACTAR_CLK_MUX_PLL == HACTAR_CLK_MUX_PLL_SRC_HSI
#define HACTAR_CLK_MUX_PLL_SRC_MASK RCC_CFGR_PLLSRC_HSI_Div2
#else
#error HACTAR_CLK_MUX_PLL: invalid value
#endif

// get bit mask for SW_SRC
#if HACTAR_CLK_MUX_SW == HACTAR_CLK_MUX_SW_SRC_HSE
#define HACTAR_CLK_MUX_SW_SRC_MASK RCC_CFGR_SW_HSE
#elif HACTAR_CLK_MUX_SW == HACTAR_CLK_MUX_SW_SRC_HSI
#define HACTAR_CLK_MUX_SW_SRC_MASK RCC_CFGR_SW_HSI
#elif HACTAR_CLK_MUX_SW == HACTAR_CLK_MUX_SW_SRC_PLLMUL
#define HACTAR_CLK_MUX_SW_SRC_MASK RCC_CFGR_SW_PLL
#else
#error HACTAR_CLK_MUX_SW: invalid value
#endif

// check pll mul
#if HACTAR_CLK_SCALE_PLLMULL < 2 || HACTAR_CLK_SCALE_PLLMULL > 16
#error HACTAR_CLK_SCALE_PLLMULL: invalid value
#endif

// check usb scale
#if HACTAR_CLK_SCALE_USB != 10 && HACTAR_CLK_SCALE_USB != 15
#error HACTAR_CLK_SCALE_USB: invalid value
#endif

#endif // HACTAR_DEV_NO_CL

// All devices -----------------------------------------------------------------

// check STK_SRC
#if HACTAR_CLK_MUX_STK != HACTAR_CLK_MUX_STK_SRC_DIV8 && \
    HACTAR_CLK_MUX_STK != HACTAR_CLK_MUX_STK_SRC_AHBPRE
#error HACTAR_CLK_MUX_STK: invalid value
#endif

// check hclk/ahb scale value
#if (HACTAR_CLK_SCALE_AHB != 1) && (HACTAR_CLK_SCALE_AHB != 2) && \
    (HACTAR_CLK_SCALE_AHB != 4) && (HACTAR_CLK_SCALE_AHB != 8) && \
    (HACTAR_CLK_SCALE_AHB != 16) && (HACTAR_CLK_SCALE_AHB != 64) && \
    (HACTAR_CLK_SCALE_AHB != 128) && (HACTAR_CLK_SCALE_AHB != 256) && \
    (HACTAR_CLK_SCALE_AHB != 512)
#error HACTAR_CLK_SCALE_AHB: invalid value
#endif

// check adc scale value
#if (HACTAR_CLK_SCALE_ADC != 2) && (HACTAR_CLK_SCALE_ADC != 4) && \
    (HACTAR_CLK_SCALE_ADC != 6) && (HACTAR_CLK_SCALE_ADC != 8)
#error HACTAR_CLK_SCALE_ADC: invalid value
#endif

// check apb1 scale value
#if (HACTAR_CLK_SCALE_APB1 != 1) && (HACTAR_CLK_SCALE_APB1 != 2) && \
    (HACTAR_CLK_SCALE_APB1 != 4) && (HACTAR_CLK_SCALE_APB1 != 8) && \
    (HACTAR_CLK_SCALE_APB1 != 16)
#error HACTAR_CLK_SCALE_APB1: invalid value
#endif

// check apb1 scale value
#if (HACTAR_CLK_SCALE_APB2 != 1) && (HACTAR_CLK_SCALE_APB2 != 2) && \
    (HACTAR_CLK_SCALE_APB2 != 4) && (HACTAR_CLK_SCALE_APB2 != 8) && \
    (HACTAR_CLK_SCALE_APB2 != 16)
#error HACTAR_CLK_SCALE_APB2: invalid value
#endif

#endif // HACTAR_PLATFORM_CHECK_H__
