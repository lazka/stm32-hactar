// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_PLATFORM_DEF_H__
#define HACTAR_PLATFORM_DEF_H__

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
