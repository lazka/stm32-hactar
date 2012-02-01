// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "stm32f10x.h"

#include <hactar/startup.h>
#include <hactar/platform_check.h>

static void hactarStartup(void);
static void hactarStartupPeriph(void);

// This gets called from the startup asm
void SystemInit(void)
{
    hactarStartup();
    hactarStartupPeriph();
}

static void hactarStartupPeriph(void)
{
    uint32_t ppre_lt[] = {0, 0, 4, 0, 5, 0, 0, 0, 6, 0,
                          0, 0, 0, 0, 0, 0, 7};
    // ADC Prescaler
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_ADCPRE) | \
                ((HACTAR_CLK_SCALE_ADC - 2) / 2) << 14;

    // Prescaler for PCLK1
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1) | \
                (ppre_lt[HACTAR_CLK_SCALE_APB1] << 8);

    // Prescaler for PCLK2
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE2) | \
                (ppre_lt[HACTAR_CLK_SCALE_APB2] << 11);
}

static void hactarStartup(void)
{
    // disable all clock interrupts and clear pending bits
    RCC->CIR = (RCC->CIR & ~0xFFFF) | 0xFF0000;

#if (HACTAR_CLK_MUX_RTC == HACTAR_CLK_MUX_RTC_SRC_LSE) && \
    (defined HACTAR_CLK_DEV_RTC)

    // enable LSE and wait
    RCC->BDCR |= RCC_BDCR_LSEON
    while(!(RCC->BDCR & RCC_BDCR_LSERDY));

#endif

#ifdef HACTAR_CLK_DEV_RTC

    // set RTC src
    RCC->BDCR = (RCC->BDCR & ~RCC_BDCR_RTCSEL) | HACTAR_CLK_MUX_RTC_SRC_MASK;
    // enable clock
    RCC->BDRC |= RCC_BDCR_RTCEN;

#endif

#ifdef HACTAR_CLK_HSE

    // switch on HSE and wait for ready
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY));

#endif

    // Calculate system clock for setting the number of flash wait states
    uint32_t sysclk = hactarGetSystemClock();
    uint32_t flash_wait_states;
    if(sysclk <= 24000000)
        flash_wait_states = FLASH_ACR_LATENCY_0;
    else if(sysclk <= 48000000)
        flash_wait_states = FLASH_ACR_LATENCY_1;
    else
        flash_wait_states = FLASH_ACR_LATENCY_2;

    // Prefetch buffer enable
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    // Set flash latency / wait states
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | flash_wait_states;

    // datasheet says this has to be disabled before clearing the PLL1
    RCC->AHBENR &= ~RCC_AHBENR_OTGFSEN;

    // disable all PLLs
    RCC->CR &= ~RCC_CR_PLLON;
    RCC->CR &= ~RCC_CR_PLL2ON;
    RCC->CR &= ~RCC_CR_PLL3ON;

    // set prediv2 scale (set it always, checking would be too complicated)
    // needs PLL2/3 disabled
    RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PREDIV2) | \
                 (((uint32_t)HACTAR_CLK_SCALE_PREDIV2 - 1) << 4);

// check if we need PLL3
#if ((defined HACTAR_CLK_DEV_I2S2) && \
    (HACTAR_CLK_MUX_I2S2 == HACTAR_CLK_MUX_I2S2_SRC_PLL3MUL)) || \
    ((defined HACTAR_CLK_DEV_I2S3) && \
    (HACTAR_CLK_MUX_I2S3 == HACTAR_CLK_MUX_I2S3_SRC_MUL2))

    // set multiplier (PLL3 disabled!)
    if(HACTAR_CLK_SCALE_PLL3MUL == 20)
        RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PLL3MUL) | RCC_CFGR2_PLL3MUL20;
    else {
        RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PLL3MUL) | \
                     (((uint32_t)HACTAR_CLK_SCALE_PLL3MUL - 2) << 12);
    }

    // finally enable PLL3
    RCC->CR |= RCC_CR_PLL3ON;
    while(!(RCC->CR & RCC_CR_PLL3RDY));

#endif

// We don't need PLL if SW has a different source and USB is disabled
#if (HACTAR_CLK_MUX_SW != HACTAR_CLK_MUX_SW_SRC_HSE) || \
    (defined HACTAR_CLK_DEV_USB)

    // set PLL MUL
    if(HACTAR_CLK_SCALE_PLLMUL == 65)
        RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLMULL) | RCC_CFGR_PLLMULL6_5;
    else
        RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLMULL) | \
                    (((uint32_t)(HACTAR_CLK_SCALE_PLLMUL / 10) - 2) << 18);

    // set the USB prescaler
#if (defined HACTAR_CLK_DEV_USB) && HACTAR_CLK_SCALE_USB == 2

    RCC->CFGR |= RCC_CFGR_USBPRE;

#elif (defined HACTAR_CLK_DEV_USB) && HACTAR_CLK_SCALE_USB == 3

    RCC->CFGR &= ~RCC_CFGR_USBPRE;

#endif

#if HACTAR_CLK_MUX_PLL == HACTAR_CLK_MUX_PLL_SRC_PREDIV1

    // set PLL src (the previous clock source still has to be active)
    RCC->CFGR |= RCC_CFGR_PLLSRC;

    // set prediv1 scale
    RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PREDIV1) | \
                 ((uint32_t)HACTAR_CLK_SCALE_PREDIV1 - 1);

#if HACTAR_CLK_MUX_PREDIV1 == HACTAR_CLK_MUX_PREDIV1_SRC_HSE

    // set prediv1 src to hse
    RCC->CFGR2 &= ~ RCC_CFGR2_PREDIV1SRC;

#elif HACTAR_CLK_MUX_PREDIV1 == HACTAR_CLK_MUX_PREDIV1_SRC_PLL2MUL

    // set multiplier (PLL 2 disabled!)
    if(HACTAR_CLK_SCALE_PLL2MUL == 20) {
        RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PLL2MUL) | RCC_CFGR2_PLL2MUL20;
    }
    else {
        RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PLL2MUL) | \
                     (((uint32_t)HACTAR_CLK_SCALE_PLL2MUL - 2) << 8);
    }

    // enable PLL2 again
    RCC->CR |= RCC_CR_PLL2ON;
    while(!(RCC->CR & RCC_CR_PLL2RDY));

    // set prediv1 source to pll2mul
    RCC->CFGR2 |=  RCC_CFGR2_PREDIV1SRC;

#endif

#elif HACTAR_CLK_MUX_PLL == HACTAR_CLK_MUX_PLL_SRC_DIV2

    // set PLL source to div2
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;

#endif

    // enable PLL again and wait for it to be ready
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));

#endif

    // SW source: clear last two bits and set right source
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | HACTAR_CLK_MUX_SW_SRC_MASK;

#if (HACTAR_CLK_MUX_RTC != HACTAR_CLK_MUX_RTC_SRC_LSI) && \
    !(defined HACTAR_CLK_DEV_WATCHDOG) && !(defined HACTAR_CLK_DEV_RTC)

    // LSI (we can disable if we don't use it and after switching away from it)
    RCC->CSR &= ~RCC_CSR_LSION;

#endif

#if (HACTAR_CLK_MUX_PLL != HACTAR_CLK_MUX_PLL_SRC_DIV2) && \
    (HACTAR_CLK_MUX_SW != HACTAR_CLK_MUX_SW_SRC_HSI)

    // HSI (we can disable if we don't use it and after switching away from it)
    RCC->CR &= ~RCC_CR_HSION;

#endif

    // set AHB prescaler
    int32_t i;
    for(i = 0; i < 9; i++) {
        if(HACTAR_CLK_SCALE_AHB == (1 << i)) {
            // bug in STM code.. they expect it to be zeros *grr*
            // see APBAHBPrescTable in stm32f10x_rcc
            if (i == 0)
                RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE);
            else
                RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | \
                            ((0x8 + (i - 1)) << 4);
            break;
        }
    }

    // Set systick source
#if HACTAR_CLK_MUX_STK == HACTAR_CLK_MUX_STK_SRC_AHBPRE
    SysTick->CTRL |= SysTick_CLKSource_HCLK;
#else
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
#endif

}

// Returns the system clock based on the configuration given in platform.h
uint32_t hactarGetSystemClock(void)
{
    static uint32_t sysclk = 0;

    if(sysclk != 0)
        return sysclk;

#if HACTAR_CLK_MUX_SW == HACTAR_CLK_MUX_SW_SRC_HSI
    sysclk = HSI_VALUE;
#elif HACTAR_CLK_MUX_SW == HACTAR_CLK_MUX_SW_SRC_HSE
    sysclk = HSE_VALUE;
#else

#if HACTAR_CLK_MUX_PLL == HACTAR_CLK_MUX_PLL_SRC_DIV2
    sysclk = HSI_VALUE / 2;
#else

    sysclk = HSE_VALUE;
#if HACTAR_CLK_MUX_PREDIV1 == HACTAR_CLK_MUX_PREDIV1_SRC_PLL2MUL
    sysclk /= HACTAR_CLK_SCALE_PREDIV2;
    sysclk *= HACTAR_CLK_SCALE_PLL2MUL;
#endif

    sysclk /= (uint32_t)HACTAR_CLK_SCALE_PREDIV1;
#endif

    sysclk *= ((uint32_t)HACTAR_CLK_SCALE_PLLMUL / 10);
#endif

    return sysclk;
}
