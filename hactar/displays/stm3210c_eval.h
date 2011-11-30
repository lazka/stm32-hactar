// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTER_DISPLAYS_STM32C_EVAL_H__
#define HACTER_DISPLAYS_STM32C_EVAL_H__

#include <hactar/display.h>
#include <hactar/framebuffer.h>

typedef struct
{
    DisplayInfo base_;
    size_t dirty_y_min_; // the first modified y line
    size_t dirty_y_max_; // the last modified y line
} DisplayInfoStm32cEval;

extern DisplayInfoStm32cEval stm32c_eval_display;

#endif
