// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifdef USE_STM3210C_EVAL

#include <stdio.h>

#include <hactar/stdio_devs/eval_lcd.h>

static int writeEvalLCDStdout(char *ptr, int len, uint8_t err)
{
    size_t column = evallcdconsole_info.column_;
    size_t line = evallcdconsole_info.line_;
    size_t width = LCD_GetFont()->Width;
    size_t i;

    if(err)
        LCD_SetTextColor(EVAL_LCD_STDERR_TXTCOLOR);
    else
        LCD_SetTextColor(EVAL_LCD_STDOUT_TXTCOLOR);

    for(i = 0; i < len && *ptr != 0; i++)
    {
        if(*ptr == '\n' || *ptr == '\r' || *ptr == EOF)
        {
            line++;
            if(LINE(line) > LCD_PIXEL_HEIGHT)
            {
                LCD_Clear(EVAL_LCD_BGCOLOR);
                line = 0;
            }
            column = LCD_PIXEL_WIDTH - 1;
        }
        else if(*ptr == '\b')
        {
            if(column < LCD_PIXEL_WIDTH - 1 - width)
                column += width;
            LCD_DisplayChar(LINE(line), column, ' ');
        }
        else if(column >= width)
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

static void clearEvalLCDStdout(void)
{
    LCD_Clear(EVAL_LCD_BGCOLOR);

    evallcdconsole_info.column_ = LCD_PIXEL_WIDTH - 1;
    evallcdconsole_info.line_ = 0;
}

void initEvalLCDStdoutDevice(void)
{
    LCD_Setup();
    LCD_SetFont(&Font12x12);

    LCD_SetBackColor(EVAL_LCD_BGCOLOR);

    evallcdconsole_info.device_.write_func_ = &writeEvalLCDStdout;
    evallcdconsole_info.device_.clear_func_ = &clearEvalLCDStdout;

    clearEvalLCDStdout();

    stdout_device = (StdoutDevice*)&evallcdconsole_info;
}

#endif
