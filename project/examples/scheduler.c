// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "scheduler.h"
#include <hactar/hactar.h>

static void foo(void)
{
    while(1);
}

static void foo2(void)
{
    while(1);
}

void initSchedulerExample(void)
{
    Thread thread;
    uint8_t stack[500];
    threadAdd(&thread, &foo, stack, 500);

    Thread thread2;
    uint8_t stack2[500];
    threadAdd(&thread2, &foo2, stack2, 500);

    schedulerInit(100);
}
