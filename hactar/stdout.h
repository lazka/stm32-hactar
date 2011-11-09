// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_STDOUT_H__
#define HACTAR_STDOUT_H__

#include "stddef.h"

typedef struct
{
    int (*write_func_)(char *ptr, int len);
    void *data_;
} StdoutDevice;

StdoutDevice stdout_device;

void initUSARTStdoutDevice(void);

#endif
