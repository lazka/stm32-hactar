/* Copyright (C) 2011  Christoph Reiter <christoph.reiter@gmx.at>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __HACTAR_PLATFORM
#define __HACTAR_PLATFORM

#include "stm32f10x.h"
#include "platform_def.h"

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
#define HACTAR_CLK_MUX_STK     HACTAR_CLK_MUX_STK_SRC_AHBPRE
#define HACTAR_CLK_MUX_I2S2    HACTAR_CLK_MUX_I2S2_SRC_PLL3MUL
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

#ifdef STM32F10X_CL

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

#endif

#endif
