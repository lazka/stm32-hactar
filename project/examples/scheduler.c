// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "scheduler.h"
#include <hactar/hactar.h>
#include <errno.h>
#include <unistd.h>

mutex_t lock;

static void foo(void)
{
    while(1)
    {
        mutexLock(&lock);
        threadYield();
        mutexUnlock(&lock);
        threadYield();
    }
}

static void foo2(void)
{
    while(1)
    {
        mutexLock(&lock);
        threadYield();
        mutexUnlock(&lock);
    }
}

void initSchedulerExample(void)
{
    mutexInit(&lock);

    Thread thread;
    uint8_t stack[1000];
    threadAdd(&thread, &foo, stack, 1000);

    Thread thread2;
    uint8_t stack2[1000];
    threadAdd(&thread2, &foo2, stack2, 1000);

    schedulerInit(1000);
}
