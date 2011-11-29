// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_STDOUT_H__
#define HACTAR_STDOUT_H__

#include <stdint.h>

typedef struct
{
    int (*write_func_)(char *ptr, int len, uint8_t err);
    void (*clear_func_)(void);
} StdoutDevice;

typedef struct
{
    int (*read_func_)(char *ptr, int len);
} StdinDevice;

StdoutDevice *stdout_device;
StdinDevice *stdin_device;

#endif
