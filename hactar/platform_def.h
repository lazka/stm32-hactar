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

#ifndef __HACTAR_PLATFORM_DEF
#define __HACTAR_PLATFORM_DEF

#include "platform.h"

#ifdef STM32F10X_CL

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

#endif

#endif
