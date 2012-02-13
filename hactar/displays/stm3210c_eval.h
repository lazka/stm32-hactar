// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifdef USE_STM3210C_EVAL

#ifndef HACTER_DISPLAYS_STM32C_EVAL_H__
#define HACTER_DISPLAYS_STM32C_EVAL_H__

#include <hactar/display.h>
#include <hactar/framebuffer.h>

#include "stm32f10x.h"
#include "STM3210C_EVAL/stm3210c_eval_lcd.h"

typedef struct
{
    DisplayInfo base_;
    size_t dirty_y_min_; // the first modified y line
    size_t dirty_y_max_; // the last modified y line
    size_t dirty_x_min_; // the first modified y line
    size_t dirty_x_max_; // the last modified y line
} DisplayInfoStm32cEval;

extern DisplayInfoStm32cEval stm32c_eval_display;

#endif

#endif // USE_STM3210C_EVAL
