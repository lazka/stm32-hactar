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

#ifndef __HACTAR_PLATFORM_CHECK
#define __HACTAR_PLATFORM_CHECK

#include "platform.h"

#ifdef STM32F10X_CL

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

#endif

#endif
