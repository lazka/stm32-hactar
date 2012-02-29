// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <hactar/misc.h>

#define BOOTRAM (void*)0xF1E0F85F
#define DEFAULT_HANDLER Default_Handler

#define SDEF(irq, handler) STRINGIFY(weak irq = handler)
#define IRQ_WEAK(irq) void WEAK irq(void); _Pragma(SDEF(irq, DEFAULT_HANDLER))

extern const void _estack;

void WEAK Default_Handler(void){;}

// Base IRQs

IRQ_WEAK(Reset_Handler)
IRQ_WEAK(NMI_Handler)
IRQ_WEAK(HardFault_Handler)
IRQ_WEAK(MemManage_Handler)
IRQ_WEAK(BusFault_Handler)
IRQ_WEAK(UsageFault_Handler)
IRQ_WEAK(SVC_Handler)
IRQ_WEAK(DebugMon_Handler)
IRQ_WEAK(PendSV_Handler)
IRQ_WEAK(SysTick_Handler)

// Common to all devices

IRQ_WEAK(DMA1_Channel1_IRQHandler)
IRQ_WEAK(DMA1_Channel2_IRQHandler)
IRQ_WEAK(DMA1_Channel3_IRQHandler)
IRQ_WEAK(DMA1_Channel4_IRQHandler)
IRQ_WEAK(DMA1_Channel5_IRQHandler)
IRQ_WEAK(DMA1_Channel6_IRQHandler)
IRQ_WEAK(DMA1_Channel7_IRQHandler)
IRQ_WEAK(EXTI0_IRQHandler)
IRQ_WEAK(EXTI15_10_IRQHandler)
IRQ_WEAK(EXTI1_IRQHandler)
IRQ_WEAK(EXTI2_IRQHandler)
IRQ_WEAK(EXTI3_IRQHandler)
IRQ_WEAK(EXTI4_IRQHandler)
IRQ_WEAK(EXTI9_5_IRQHandler)
IRQ_WEAK(FLASH_IRQHandler)
IRQ_WEAK(I2C1_ER_IRQHandler)
IRQ_WEAK(I2C1_EV_IRQHandler)
IRQ_WEAK(PVD_IRQHandler)
IRQ_WEAK(RCC_IRQHandler)
IRQ_WEAK(RTCAlarm_IRQHandler)
IRQ_WEAK(RTC_IRQHandler)
IRQ_WEAK(SPI1_IRQHandler)
IRQ_WEAK(TAMPER_IRQHandler)
IRQ_WEAK(TIM1_CC_IRQHandler)
IRQ_WEAK(TIM2_IRQHandler)
IRQ_WEAK(TIM3_IRQHandler)
IRQ_WEAK(USART1_IRQHandler)
IRQ_WEAK(USART2_IRQHandler)
IRQ_WEAK(WWDG_IRQHandler)

// And now for each device

#if defined STM32F10X_MD_VL
IRQ_WEAK(ADC1_IRQHandler)
IRQ_WEAK(CEC_IRQHandler)
IRQ_WEAK(I2C2_ER_IRQHandler)
IRQ_WEAK(I2C2_EV_IRQHandler)
IRQ_WEAK(SPI2_IRQHandler)
IRQ_WEAK(TIM1_BRK_TIM15_IRQHandler)
IRQ_WEAK(TIM1_TRG_COM_TIM17_IRQHandler)
IRQ_WEAK(TIM1_UP_TIM16_IRQHandler)
IRQ_WEAK(TIM4_IRQHandler)
IRQ_WEAK(TIM6_DAC_IRQHandler)
IRQ_WEAK(TIM7_IRQHandler)
IRQ_WEAK(USART3_IRQHandler)
#elif defined STM32F10X_HD
IRQ_WEAK(ADC1_2_IRQHandler)
IRQ_WEAK(ADC3_IRQHandler)
IRQ_WEAK(CAN1_RX1_IRQHandler)
IRQ_WEAK(CAN1_SCE_IRQHandler)
IRQ_WEAK(DMA2_Channel1_IRQHandler)
IRQ_WEAK(DMA2_Channel2_IRQHandler)
IRQ_WEAK(DMA2_Channel3_IRQHandler)
IRQ_WEAK(DMA2_Channel4_5_IRQHandler)
IRQ_WEAK(FSMC_IRQHandler)
IRQ_WEAK(I2C2_ER_IRQHandler)
IRQ_WEAK(I2C2_EV_IRQHandler)
IRQ_WEAK(SDIO_IRQHandler)
IRQ_WEAK(SPI2_IRQHandler)
IRQ_WEAK(SPI3_IRQHandler)
IRQ_WEAK(TIM1_BRK_IRQHandler)
IRQ_WEAK(TIM1_TRG_COM_IRQHandler)
IRQ_WEAK(TIM1_UP_IRQHandler)
IRQ_WEAK(TIM4_IRQHandler)
IRQ_WEAK(TIM5_IRQHandler)
IRQ_WEAK(TIM6_IRQHandler)
IRQ_WEAK(TIM7_IRQHandler)
IRQ_WEAK(TIM8_BRK_IRQHandler)
IRQ_WEAK(TIM8_CC_IRQHandler)
IRQ_WEAK(TIM8_TRG_COM_IRQHandler)
IRQ_WEAK(TIM8_UP_IRQHandler)
IRQ_WEAK(UART4_IRQHandler)
IRQ_WEAK(UART5_IRQHandler)
IRQ_WEAK(USART3_IRQHandler)
IRQ_WEAK(USBWakeUp_IRQHandler)
IRQ_WEAK(USB_HP_CAN1_TX_IRQHandler)
IRQ_WEAK(USB_LP_CAN1_RX0_IRQHandler)
#elif defined STM32F10X_LD
IRQ_WEAK(ADC1_2_IRQHandler)
IRQ_WEAK(CAN1_RX1_IRQHandler)
IRQ_WEAK(CAN1_SCE_IRQHandler)
IRQ_WEAK(TIM1_BRK_IRQHandler)
IRQ_WEAK(TIM1_TRG_COM_IRQHandler)
IRQ_WEAK(TIM1_UP_IRQHandler)
IRQ_WEAK(USBWakeUp_IRQHandler)
IRQ_WEAK(USB_HP_CAN1_TX_IRQHandler)
IRQ_WEAK(USB_LP_CAN1_RX0_IRQHandler)
#elif defined STM32F10X_XL
IRQ_WEAK(ADC1_2_IRQHandler)
IRQ_WEAK(ADC3_IRQHandler)
IRQ_WEAK(CAN1_RX1_IRQHandler)
IRQ_WEAK(CAN1_SCE_IRQHandler)
IRQ_WEAK(DMA2_Channel1_IRQHandler)
IRQ_WEAK(DMA2_Channel2_IRQHandler)
IRQ_WEAK(DMA2_Channel3_IRQHandler)
IRQ_WEAK(DMA2_Channel4_5_IRQHandler)
IRQ_WEAK(FSMC_IRQHandler)
IRQ_WEAK(I2C2_ER_IRQHandler)
IRQ_WEAK(I2C2_EV_IRQHandler)
IRQ_WEAK(SDIO_IRQHandler)
IRQ_WEAK(SPI2_IRQHandler)
IRQ_WEAK(SPI3_IRQHandler)
IRQ_WEAK(TIM1_BRK_TIM9_IRQHandler)
IRQ_WEAK(TIM1_TRG_COM_TIM11_IRQHandler)
IRQ_WEAK(TIM1_UP_TIM10_IRQHandler)
IRQ_WEAK(TIM4_IRQHandler)
IRQ_WEAK(TIM5_IRQHandler)
IRQ_WEAK(TIM6_IRQHandler)
IRQ_WEAK(TIM7_IRQHandler)
IRQ_WEAK(TIM8_BRK_TIM12_IRQHandler)
IRQ_WEAK(TIM8_CC_IRQHandler)
IRQ_WEAK(TIM8_TRG_COM_TIM14_IRQHandler)
IRQ_WEAK(TIM8_UP_TIM13_IRQHandler)
IRQ_WEAK(UART4_IRQHandler)
IRQ_WEAK(UART5_IRQHandler)
IRQ_WEAK(USART3_IRQHandler)
IRQ_WEAK(USBWakeUp_IRQHandler)
IRQ_WEAK(USB_HP_CAN1_TX_IRQHandler)
IRQ_WEAK(USB_LP_CAN1_RX0_IRQHandler)
#elif defined STM32F10X_LD_VL
IRQ_WEAK(ADC1_IRQHandler)
IRQ_WEAK(CEC_IRQHandler)
IRQ_WEAK(TIM1_BRK_TIM15_IRQHandler)
IRQ_WEAK(TIM1_TRG_COM_TIM17_IRQHandler)
IRQ_WEAK(TIM1_UP_TIM16_IRQHandler)
IRQ_WEAK(TIM6_DAC_IRQHandler)
IRQ_WEAK(TIM7_IRQHandler)
#elif defined STM32F10X_HD_VL
IRQ_WEAK(ADC1_IRQHandler)
IRQ_WEAK(CEC_IRQHandler)
IRQ_WEAK(DMA2_Channel1_IRQHandler)
IRQ_WEAK(DMA2_Channel2_IRQHandler)
IRQ_WEAK(DMA2_Channel3_IRQHandler)
IRQ_WEAK(DMA2_Channel4_5_IRQHandler)
IRQ_WEAK(DMA2_Channel5_IRQHandler)
IRQ_WEAK(I2C2_ER_IRQHandler)
IRQ_WEAK(I2C2_EV_IRQHandler)
IRQ_WEAK(SPI2_IRQHandler)
IRQ_WEAK(SPI3_IRQHandler)
IRQ_WEAK(TIM12_IRQHandler)
IRQ_WEAK(TIM13_IRQHandler)
IRQ_WEAK(TIM14_IRQHandler)
IRQ_WEAK(TIM1_BRK_TIM15_IRQHandler)
IRQ_WEAK(TIM1_TRG_COM_TIM17_IRQHandler)
IRQ_WEAK(TIM1_UP_TIM16_IRQHandler)
IRQ_WEAK(TIM4_IRQHandler)
IRQ_WEAK(TIM5_IRQHandler)
IRQ_WEAK(TIM6_DAC_IRQHandler)
IRQ_WEAK(TIM7_IRQHandler)
IRQ_WEAK(UART4_IRQHandler)
IRQ_WEAK(UART5_IRQHandler)
IRQ_WEAK(USART3_IRQHandler)
#elif defined STM32F10X_CL
IRQ_WEAK(ADC1_2_IRQHandler)
IRQ_WEAK(CAN1_RX0_IRQHandler)
IRQ_WEAK(CAN1_RX1_IRQHandler)
IRQ_WEAK(CAN1_SCE_IRQHandler)
IRQ_WEAK(CAN1_TX_IRQHandler)
IRQ_WEAK(CAN2_RX0_IRQHandler)
IRQ_WEAK(CAN2_RX1_IRQHandler)
IRQ_WEAK(CAN2_SCE_IRQHandler)
IRQ_WEAK(CAN2_TX_IRQHandler)
IRQ_WEAK(DMA2_Channel1_IRQHandler)
IRQ_WEAK(DMA2_Channel2_IRQHandler)
IRQ_WEAK(DMA2_Channel3_IRQHandler)
IRQ_WEAK(DMA2_Channel4_IRQHandler)
IRQ_WEAK(DMA2_Channel5_IRQHandler)
IRQ_WEAK(ETH_IRQHandler)
IRQ_WEAK(ETH_WKUP_IRQHandler)
IRQ_WEAK(I2C2_ER_IRQHandler)
IRQ_WEAK(I2C2_EV_IRQHandler)
IRQ_WEAK(OTG_FS_IRQHandler)
IRQ_WEAK(OTG_FS_WKUP_IRQHandler)
IRQ_WEAK(SPI2_IRQHandler)
IRQ_WEAK(SPI3_IRQHandler)
IRQ_WEAK(TIM1_BRK_IRQHandler)
IRQ_WEAK(TIM1_TRG_COM_IRQHandler)
IRQ_WEAK(TIM1_UP_IRQHandler)
IRQ_WEAK(TIM4_IRQHandler)
IRQ_WEAK(TIM5_IRQHandler)
IRQ_WEAK(TIM6_IRQHandler)
IRQ_WEAK(TIM7_IRQHandler)
IRQ_WEAK(UART4_IRQHandler)
IRQ_WEAK(UART5_IRQHandler)
IRQ_WEAK(USART3_IRQHandler)
#elif defined STM32F10X_MD
IRQ_WEAK(ADC1_2_IRQHandler)
IRQ_WEAK(CAN1_RX1_IRQHandler)
IRQ_WEAK(CAN1_SCE_IRQHandler)
IRQ_WEAK(I2C2_ER_IRQHandler)
IRQ_WEAK(I2C2_EV_IRQHandler)
IRQ_WEAK(SPI2_IRQHandler)
IRQ_WEAK(TIM1_BRK_IRQHandler)
IRQ_WEAK(TIM1_TRG_COM_IRQHandler)
IRQ_WEAK(TIM1_UP_IRQHandler)
IRQ_WEAK(TIM4_IRQHandler)
IRQ_WEAK(USART3_IRQHandler)
IRQ_WEAK(USBWakeUp_IRQHandler)
IRQ_WEAK(USB_HP_CAN1_TX_IRQHandler)
IRQ_WEAK(USB_LP_CAN1_RX0_IRQHandler)
#endif

__attribute__ ((section(".interrupt_vector")))
void (* const g_pfnVectors[])(void) = {
    &_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,

    // Common to all, but might be different in future devices
    WWDG_IRQHandler, PVD_IRQHandler, TAMPER_IRQHandler, RTC_IRQHandler,
    FLASH_IRQHandler, RCC_IRQHandler, EXTI0_IRQHandler,
    EXTI1_IRQHandler, EXTI2_IRQHandler, EXTI3_IRQHandler,
    EXTI4_IRQHandler, DMA1_Channel1_IRQHandler,
    DMA1_Channel2_IRQHandler, DMA1_Channel3_IRQHandler,
    DMA1_Channel4_IRQHandler, DMA1_Channel5_IRQHandler,
    DMA1_Channel6_IRQHandler, DMA1_Channel7_IRQHandler,

#if defined STM32F10X_HD_VL
    ADC1_IRQHandler, 0, 0, 0, 0, EXTI9_5_IRQHandler,
    TIM1_BRK_TIM15_IRQHandler, TIM1_UP_TIM16_IRQHandler,
    TIM1_TRG_COM_TIM17_IRQHandler, TIM1_CC_IRQHandler, TIM2_IRQHandler,
    TIM3_IRQHandler, TIM4_IRQHandler, I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler, I2C2_EV_IRQHandler, I2C2_ER_IRQHandler,
    SPI1_IRQHandler, SPI2_IRQHandler, USART1_IRQHandler,
    USART2_IRQHandler, USART3_IRQHandler, EXTI15_10_IRQHandler,
    RTCAlarm_IRQHandler, CEC_IRQHandler, TIM12_IRQHandler,
    TIM13_IRQHandler, TIM14_IRQHandler, 0, 0, 0, 0, TIM5_IRQHandler,
    SPI3_IRQHandler, UART4_IRQHandler, UART5_IRQHandler,
    TIM6_DAC_IRQHandler, TIM7_IRQHandler, DMA2_Channel1_IRQHandler,
    DMA2_Channel2_IRQHandler, DMA2_Channel3_IRQHandler,
    DMA2_Channel4_5_IRQHandler, DMA2_Channel5_IRQHandler, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, BOOTRAM
#elif defined STM32F10X_LD
    ADC1_2_IRQHandler, USB_HP_CAN1_TX_IRQHandler,
    USB_LP_CAN1_RX0_IRQHandler, CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler, EXTI9_5_IRQHandler, TIM1_BRK_IRQHandler,
    TIM1_UP_IRQHandler, TIM1_TRG_COM_IRQHandler, TIM1_CC_IRQHandler,
    TIM2_IRQHandler, TIM3_IRQHandler, 0, I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler, 0, 0, SPI1_IRQHandler, 0, USART1_IRQHandler,
    USART2_IRQHandler, 0, EXTI15_10_IRQHandler, RTCAlarm_IRQHandler,
    USBWakeUp_IRQHandler, 0, 0, 0, 0, 0, 0, 0, BOOTRAM
#elif defined STM32F10X_CL
    ADC1_2_IRQHandler, CAN1_TX_IRQHandler, CAN1_RX0_IRQHandler,
    CAN1_RX1_IRQHandler, CAN1_SCE_IRQHandler, EXTI9_5_IRQHandler,
    TIM1_BRK_IRQHandler, TIM1_UP_IRQHandler, TIM1_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler, TIM2_IRQHandler, TIM3_IRQHandler,
    TIM4_IRQHandler, I2C1_EV_IRQHandler, I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler, I2C2_ER_IRQHandler, SPI1_IRQHandler,
    SPI2_IRQHandler, USART1_IRQHandler, USART2_IRQHandler,
    USART3_IRQHandler, EXTI15_10_IRQHandler, RTCAlarm_IRQHandler,
    OTG_FS_WKUP_IRQHandler, 0, 0, 0, 0, 0, 0, 0, TIM5_IRQHandler,
    SPI3_IRQHandler, UART4_IRQHandler, UART5_IRQHandler,
    TIM6_IRQHandler, TIM7_IRQHandler, DMA2_Channel1_IRQHandler,
    DMA2_Channel2_IRQHandler, DMA2_Channel3_IRQHandler,
    DMA2_Channel4_IRQHandler, DMA2_Channel5_IRQHandler, ETH_IRQHandler,
    ETH_WKUP_IRQHandler, CAN2_TX_IRQHandler, CAN2_RX0_IRQHandler,
    CAN2_RX1_IRQHandler, CAN2_SCE_IRQHandler, OTG_FS_IRQHandler, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, BOOTRAM
#elif defined STM32F10X_HD
    ADC1_2_IRQHandler, USB_HP_CAN1_TX_IRQHandler,
    USB_LP_CAN1_RX0_IRQHandler, CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler, EXTI9_5_IRQHandler, TIM1_BRK_IRQHandler,
    TIM1_UP_IRQHandler, TIM1_TRG_COM_IRQHandler, TIM1_CC_IRQHandler,
    TIM2_IRQHandler, TIM3_IRQHandler, TIM4_IRQHandler,
    I2C1_EV_IRQHandler, I2C1_ER_IRQHandler, I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler, SPI1_IRQHandler, SPI2_IRQHandler,
    USART1_IRQHandler, USART2_IRQHandler, USART3_IRQHandler,
    EXTI15_10_IRQHandler, RTCAlarm_IRQHandler, USBWakeUp_IRQHandler,
    TIM8_BRK_IRQHandler, TIM8_UP_IRQHandler, TIM8_TRG_COM_IRQHandler,
    TIM8_CC_IRQHandler, ADC3_IRQHandler, FSMC_IRQHandler,
    SDIO_IRQHandler, TIM5_IRQHandler, SPI3_IRQHandler, UART4_IRQHandler,
    UART5_IRQHandler, TIM6_IRQHandler, TIM7_IRQHandler,
    DMA2_Channel1_IRQHandler, DMA2_Channel2_IRQHandler,
    DMA2_Channel3_IRQHandler, DMA2_Channel4_5_IRQHandler, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, BOOTRAM
#elif defined STM32F10X_LD_VL
    ADC1_IRQHandler, 0, 0, 0, 0, EXTI9_5_IRQHandler,
    TIM1_BRK_TIM15_IRQHandler, TIM1_UP_TIM16_IRQHandler,
    TIM1_TRG_COM_TIM17_IRQHandler, TIM1_CC_IRQHandler, TIM2_IRQHandler,
    TIM3_IRQHandler, 0, I2C1_EV_IRQHandler, I2C1_ER_IRQHandler, 0, 0,
    SPI1_IRQHandler, 0, USART1_IRQHandler, USART2_IRQHandler, 0,
    EXTI15_10_IRQHandler, RTCAlarm_IRQHandler, CEC_IRQHandler, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, TIM6_DAC_IRQHandler, TIM7_IRQHandler, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, BOOTRAM
#elif defined STM32F10X_MD
    ADC1_2_IRQHandler, USB_HP_CAN1_TX_IRQHandler,
    USB_LP_CAN1_RX0_IRQHandler, CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler, EXTI9_5_IRQHandler, TIM1_BRK_IRQHandler,
    TIM1_UP_IRQHandler, TIM1_TRG_COM_IRQHandler, TIM1_CC_IRQHandler,
    TIM2_IRQHandler, TIM3_IRQHandler, TIM4_IRQHandler,
    I2C1_EV_IRQHandler, I2C1_ER_IRQHandler, I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler, SPI1_IRQHandler, SPI2_IRQHandler,
    USART1_IRQHandler, USART2_IRQHandler, USART3_IRQHandler,
    EXTI15_10_IRQHandler, RTCAlarm_IRQHandler, USBWakeUp_IRQHandler, 0,
    0, 0, 0, 0, 0, 0, BOOTRAM
#elif defined STM32F10X_MD_VL
    ADC1_IRQHandler, 0, 0, 0, 0, EXTI9_5_IRQHandler,
    TIM1_BRK_TIM15_IRQHandler, TIM1_UP_TIM16_IRQHandler,
    TIM1_TRG_COM_TIM17_IRQHandler, TIM1_CC_IRQHandler, TIM2_IRQHandler,
    TIM3_IRQHandler, TIM4_IRQHandler, I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler, I2C2_EV_IRQHandler, I2C2_ER_IRQHandler,
    SPI1_IRQHandler, SPI2_IRQHandler, USART1_IRQHandler,
    USART2_IRQHandler, USART3_IRQHandler, EXTI15_10_IRQHandler,
    RTCAlarm_IRQHandler, CEC_IRQHandler, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, TIM6_DAC_IRQHandler, TIM7_IRQHandler, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, BOOTRAM
#elif defined STM32F10X_XL
    ADC1_2_IRQHandler, USB_HP_CAN1_TX_IRQHandler,
    USB_LP_CAN1_RX0_IRQHandler, CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler, EXTI9_5_IRQHandler, TIM1_BRK_TIM9_IRQHandler,
    TIM1_UP_TIM10_IRQHandler, TIM1_TRG_COM_TIM11_IRQHandler,
    TIM1_CC_IRQHandler, TIM2_IRQHandler, TIM3_IRQHandler,
    TIM4_IRQHandler, I2C1_EV_IRQHandler, I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler, I2C2_ER_IRQHandler, SPI1_IRQHandler,
    SPI2_IRQHandler, USART1_IRQHandler, USART2_IRQHandler,
    USART3_IRQHandler, EXTI15_10_IRQHandler, RTCAlarm_IRQHandler,
    USBWakeUp_IRQHandler, TIM8_BRK_TIM12_IRQHandler,
    TIM8_UP_TIM13_IRQHandler, TIM8_TRG_COM_TIM14_IRQHandler,
    TIM8_CC_IRQHandler, ADC3_IRQHandler, FSMC_IRQHandler,
    SDIO_IRQHandler, TIM5_IRQHandler, SPI3_IRQHandler, UART4_IRQHandler,
    UART5_IRQHandler, TIM6_IRQHandler, TIM7_IRQHandler,
    DMA2_Channel1_IRQHandler, DMA2_Channel2_IRQHandler,
    DMA2_Channel3_IRQHandler, DMA2_Channel4_5_IRQHandler, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, BOOTRAM
#else
#error "define a STM32F10X version"
#endif
};
