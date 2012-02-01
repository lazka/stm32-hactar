// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "stm32f10x.h"

#include <hactar/displays/st7565r.h>

static void SPIWriteCommand(uint8_t writedat)
{
    /* Loop while SPI1 is busy */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
    /* Send byte through the SPI1 peripheral */
    GPIO_ResetBits(GPIOA, ST765R_GPIO_CS | ST765R_GPIO_A0);  //set A0 = !CS = 0
    SPI_I2S_SendData(SPI1, writedat);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    //while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
    GPIO_SetBits(GPIOA, ST765R_GPIO_CS);  //set !CS = 1
}

static void SPIInit(void)
{
    /* Enable SPI1 and GPIOA clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* Configure SPI1 pins: NSS, SCK and MOSI */
    GPIO_InitTypeDef GPIO_InitStructure = {
            .GPIO_Pin = ST765R_GPIO_CLK | ST765R_GPIO_MOSI,
            .GPIO_Speed = GPIO_Speed_50MHz,
            .GPIO_Mode = GPIO_Mode_AF_PP
    };
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* The MISO is used for A0 for the display */
    GPIO_InitTypeDef GPIO_InitStructure2 = {
            .GPIO_Pin = ST765R_GPIO_CS | ST765R_GPIO_A0,
            .GPIO_Speed = GPIO_Speed_50MHz,
            .GPIO_Mode = GPIO_Mode_Out_PP
    };
    GPIO_Init(GPIOA, &GPIO_InitStructure2);

    GPIO_ResetBits(GPIOA, ST765R_GPIO_A0); //set A0 = 0
    GPIO_SetBits(GPIOA, ST765R_GPIO_CS);  //set !CS = 1

    /* SPI1 configuration */
    SPI_Cmd(SPI1, DISABLE);

    SPI_InitTypeDef SPI_InitStructure = {
        .SPI_Direction = SPI_Direction_1Line_Tx,
        .SPI_Mode = SPI_Mode_Master,
        .SPI_DataSize = SPI_DataSize_8b,
        .SPI_CPOL = SPI_CPOL_High,
        .SPI_CPHA = SPI_CPHA_2Edge,
        .SPI_NSS = SPI_NSS_Soft,
        .SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256,
        .SPI_FirstBit = SPI_FirstBit_MSB,
        .SPI_CRCPolynomial = 7
    };
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_CalculateCRC(SPI1, DISABLE);

    /* Enable SPI1  */
    SPI_Cmd(SPI1, ENABLE);
}

static void init(DisplayInfo *display)
{
    SPIInit();

    SPIWriteCommand(ST7565R_LCD_BIAS_1_7); //Set bias 1/7
    SPIWriteCommand(ST7565R_PWR_CTRL_BOOSTER | ST7565R_PWR_CTRL_VREG | ST7565R_PWR_CTRL_VFOL); //Booster, Regulator and Follower on
    SPIWriteCommand(ST7565R_ENTER_MODE_BOSSTER_RATIO); //Set internal Booster...
    SPIWriteCommand(ST7565R_BOOSTER_RATIO_234); //... to 4x
    SPIWriteCommand(ST7565R_ADC_SELECT_REVERSE); //ADC reverse for Bottom View
    SPIWriteCommand(ST7565R_COM_NORMAL); //Common output mode: Normal
    SPIWriteCommand(ST7565R_SET_VREG_RATIO(0x111)); //V0 Internal Resistor Ratio Set = 0b111
    SPIWriteCommand(ST7565R_ENTER_MODE_ELECTR_VOL); //Electronic Volume mode set
    SPIWriteCommand(ST7565R_SET_ELECTR_VOL(10)); //Electronic Volume register set = 10
    SPIWriteCommand(ST7565R_SET_LINE_START(0)); //Display start line 0
    SPIWriteCommand(ST7565R_DISPLAY_NORMAL); //Display normal
    SPIWriteCommand(ST7565R_SLEEP_MODE_ON); //set indicator...
    SPIWriteCommand(0x00); //... to NONE
    SPIWriteCommand(ST7565R_DISPLAY_ON); //Display on

    SPIWriteCommand(ST7565R_DISPLAY_ALL_ON); //All Points on
    //SPIWriteCommand(ST7565R_DISPLAY_ALL_OFF); //All Points off
}

static void update(DisplayInfo *display, FbInfo *fb)
{
    DisplayInfoST7565R *ext_info = (DisplayInfoST7565R*)display;
    ext_info->dirty_pages_ = 0;
}

// Is used to get the byte and bit offset for a specific pixel and thus
// specifies the fb data layout. Because of this we can transmit straight
// from the fb data.
// Returns 0 on success (the position is in the fb)
static void getPosition(FbInfo *fb, size_t x, size_t y,
    size_t *index, uint8_t *offset)
{
    *index = (y / 8) * fb->width_ + x;
    *offset = y % 8;
}

// Gets called whenever a byte in the framebuffer changes.
// Can be used to mark certain areas dirty, so an update is faster.
static void inval(DisplayInfo *display, size_t x, size_t y)
{
    uint8_t page;
    DisplayInfoST7565R *ext_info = (DisplayInfoST7565R*)display;

    if(x >= display->width_ || x >= display->height_)
        return;

    page = y / 8;

    if((ext_info->dirty_pages_ & (1 << page)))
    {
        if(x < ext_info->dirty_start_[page])
            ext_info->dirty_start_[page] = x;
        if(x > ext_info->dirty_end_[page])
            ext_info->dirty_end_[page] = x;
    }
    else
    {
        ext_info->dirty_pages_ |= (1 << page);
        ext_info->dirty_start_[page] = x;
        ext_info->dirty_end_[page] = x;
    }
}

// Switch it off, but don't clear the controller memory
static void off(DisplayInfo *display)
{
}

static void on(DisplayInfo *display)
{
}

DisplayInfoST7565R st7565r =
{
    .base_ = {
        .init_ = &init,
        .get_pos_ = &getPosition,
        .update_ = &update,
        .inval_ = &inval,
        .off_ = &off,
        .on_ = &on,
        .width_ = ST7565R_WIDTH_MAX,
        .height_ = ST7565R_HEIGHT_MAX,
    },
    .dirty_pages_ = 0,
    .dirty_start_ = {0},
    .dirty_end_ = {0},
};
