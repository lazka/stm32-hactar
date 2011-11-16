// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTER_DISPLAYS_ST7565R_H__
#define HACTER_DISPLAYS_ST7565R_H__

#include <hactar/display.h>
#include <hactar/framebuffer.h>

#define ST765R_GPIO_CS   GPIO_Pin_4
#define ST765R_GPIO_CLK  GPIO_Pin_5
#define ST765R_GPIO_A0   GPIO_Pin_6
#define ST765R_GPIO_MOSI GPIO_Pin_7

// http://www.sitronix.com.tw/sitronix/product.nsf/Doc/ST7565R

#define ST7565R_SET_LINE_START(line) (0x40|line)
#define ST7565R_SET_PAGE_ADDRESS(adr) (0xB0|adr)
#define ST7565R_SET_COL_ADDRESS(adr) (0xB0|adr)

#define ST7565R_ADC_SELECT_NORMAL 0xA0
#define ST7565R_ADC_SELECT_REVERSE 0xA1

#define ST7565R_DISPLAY_OFF 0xAE
#define ST7565R_DISPLAY_ON 0xAF
#define ST7565R_DISPLAY_NORMAL 0xA6
#define ST7565R_DISPLAY_REVERSE 0xA7
#define ST7565R_DISPLAY_ALL_OFF 0xA4
#define ST7565R_DISPLAY_ALL_ON 0xA5

#define ST7565R_LCD_BIAS_1_9 0xA2
#define ST7565R_LCD_BIAS_1_7 0xA3

#define ST7565R_RMW_START 0xE0
#define ST7565R_RMW_END 0xEE
#define ST7565R_RESET 0xE2

#define ST7565R_STATUS_RESET(x) (x&(1<<5))
#define ST7565R_STATUS_DISPLAY_OFF(x) (x&(1<<6))
#define ST7565R_STATUS_ADC_NORMAL(x) (x&(1<<7))
#define ST7565R_STATUS_BUSY(x) (x&(1<<8))

#define ST7565R_COM_NORMAL 0xC0
#define ST7565R_COM_REVERSE 0xC8

#define ST7565R_PWR_CTRL_ALL_OFF 0x20
#define ST7565R_PWR_CTRL_BOOSTER (0x20|(1<<2))
#define ST7565R_PWR_CTRL_VREG (0x20|(1<<3))
#define ST7565R_PWR_CTRL_VFOL (0x20|(1<<4))

#define ST7565R_SET_VREG_RATIO(x) (0x2|(x&0x7))

#define ST7565R_ENTER_MODE_ELECTR_VOL 0x81
#define ST7565R_SET_ELECTR_VOL(x) (x&0x3F)

#define ST7565R_SLEEP_MODE_ON 0xAC
#define ST7565R_SLEEP_MODE_OFF 0xAD

#define ST7565R_ENTER_MODE_BOSSTER_RATIO 0xF8
#define ST7565R_BOOSTER_RATIO_234 0x0
#define ST7565R_BOOSTER_RATIO_5 0x1
#define ST7565R_BOOSTER_RATIO_6 0x3

#define ST7565R_NOP 0xE3

/* #define ST7565R_TEST 0xFC */ // not allowed

// Driver specific defines

#define ST7565R_COLUMNS 132
#define ST7565R_PAGES 8

#define ST7565R_WIDTH_MAX 132
#define ST7565R_HEIGHT_MAX 65

// The LCD driver has got 9 pages, 132 columns each.
// Each page can be send in one go. Thus we save a start and
// an end column for each page which encloses the modified area.
// Not benchmarked, but should give a good performance.

typedef struct
{
    DisplayInfo base_;
    uint16_t dirty_pages_; // 9 pages
    uint8_t dirty_start_[ST7565R_PAGES]; // start of dirty area
    uint8_t dirty_end_[ST7565R_PAGES]; // end of dirty area
} DisplayInfoST7565R;

void st7565rInit(DisplayInfoST7565R *display, size_t width, size_t height);

void st7565rUpdate(DisplayInfoST7565R *display, FbInfo *fb);

void st7565rGetPosition(FbInfo *fb, size_t x, size_t y,
    size_t *index, uint8_t *offset);

void st7565rInval(DisplayInfoST7565R *display, size_t x, size_t y);

void st7565rOff(DisplayInfoST7565R *display);

void st7565rOn(DisplayInfoST7565R *display);

__attribute__((unused)) // to hide the bogus unused warning
static DisplayInfoST7565R st7565r =
{
    {
        (void (*)(void *, size_t width, size_t height))st7565rInit,
        (void (*)(void *fb, size_t x, size_t y,
            size_t *index, uint8_t *offset))st7565rGetPosition,
        (void (*)(void *, void *))st7565rUpdate,
        (void (*)(void *, size_t x, size_t y))st7565rInval,
        (void (*)(void *))st7565rOff,
        (void (*)(void *))st7565rOn,
        0,
        0
    },
    0,
    {0},
    {0}
};

#endif
