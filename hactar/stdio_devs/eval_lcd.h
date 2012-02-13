// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifdef USE_STM3210C_EVAL

#ifndef HACTAR_STDIO_DEVS_EVAL_LCD_H__
#define HACTAR_STDIO_DEVS_EVAL_LCD_H__

#include <stddef.h>

#include "stm32_eval.h"

#include "stm32f10x.h"
#include "STM3210C_EVAL/stm3210c_eval_lcd.h"

#include <hactar/stdio_dev.h>

#define EVAL_LCD_BGCOLOR            LCD_COLOR_BLACK
#define EVAL_LCD_STDOUT_TXTCOLOR    LCD_COLOR_WHITE
#define EVAL_LCD_STDERR_TXTCOLOR    LCD_COLOR_RED

typedef struct
{
    StdoutDevice device_;
    size_t line_;
    size_t column_;
} EvalLCDConsoleInfo;

EvalLCDConsoleInfo evallcdconsole_info;

void initEvalLCDStdoutDevice(void);

#endif

#endif // USE_STM3210C_EVAL
