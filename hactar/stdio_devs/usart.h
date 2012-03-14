// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_STDIO_DEVS_USART_H__
#define HACTAR_STDIO_DEVS_USART_H__

#include <hactar/stdio_dev.h>

#define HACTAR_USART_STDIO_STDIN        (1 << 0)
#define HACTAR_USART_STDIO_STDOUT       (1 << 1)
#define HACTAR_USART_STDIO_STDIN_ECHO   (1 << 2)

typedef struct
{
    StdinDevice device_;
    uint32_t echo_;
} USARTStdinDevice;

StdoutDevice usart_stdout_device;
USARTStdinDevice usart_stdin_device;

void initUSARTStdioDevice(uint32_t flag);

#endif
