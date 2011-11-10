// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "stm3210c_eval_lcd.h"

#include "evalconsole.h"

static int writeEvalLCDStdout(char *ptr, int len)
{
    // TODO: scrolling? might be to complicated here

    size_t column = evallcdconsole_info.column_;
    size_t line = evallcdconsole_info.line_;
    size_t width = LCD_GetFont()->Width;
    size_t i;

    for(i = 0; i < len && *ptr != 0 && ((column + 1) & 0xFFFF) >= width; i++)
    {
        if(*ptr == '\n')
        {
            line++;
            column = LCD_PIXEL_WIDTH - 1;
        }
        else
        {
            LCD_DisplayChar(LINE(line), column, *ptr);
            column -= width;
        }
        ptr++;
    }

    evallcdconsole_info.column_ = column;
    evallcdconsole_info.line_ = line;

    return len;
}

void initEvalLCDStdoutDevice(void)
{
    LCD_Setup();
    LCD_SetFont(&Font12x12);

    LCD_Clear(LCD_COLOR_BLUE);
    LCD_SetBackColor(LCD_COLOR_BLUE);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    evallcdconsole_info.device_.write_func_ = &writeEvalLCDStdout;
    evallcdconsole_info.column_ = LCD_PIXEL_WIDTH - 1;
    evallcdconsole_info.line_ = 0;

    stdout_device = (StdoutDevice*)&evallcdconsole_info;
}
