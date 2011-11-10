// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_PLATFORM_CHECK_H__
#define HACTAR_PLATFORM_CHECK_H__

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

// check apb1 scale value
#if (HACTAR_CLK_SCALE_USB != 2) && (HACTAR_CLK_SCALE_USB != 3)
#error HACTAR_CLK_SCALE_USB: invalid value
#endif

#endif

#endif
