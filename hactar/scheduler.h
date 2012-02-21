// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_SCHEDULER_H__
#define HACTAR_SCHEDULER_H__

#include <stdint.h>

#include "stm32f10x.h"

#define PRIO_LEVELS (1 << __NVIC_PRIO_BITS)
#define MINIMUM_PRIO  (PRIO_LEVELS - 1)
#define MAXIMUM_PRIO  (0)

#define SVCALL_PRIO (MAXIMUM_PRIO + 1)
#define PENDSV_PRIO (MINIMUM_PRIO)
#define SYSTCK_PRIO (MINIMUM_PRIO - 1)

void hactarSchedulerInit(uint32_t frequency);
int hactarSystickTimer(uint32_t frequency);
void hactarSchedulerSchedule(void);

#define HACTAR_N_THREADS (3)
#define HACTAR_STACK_SIZE (1000)

typedef enum {DEAD, SLEEPING, INIT, BURIED} hactarInactiveStatus;

typedef struct {
    uint8_t active_;
    hactarInactiveStatus inactive_status_;
    char* name_[10];
    uint8_t stack_[HACTAR_STACK_SIZE];
} hactarThread;

int32_t hactarThreadAdd(hactarThread* thread);
int32_t hactarThreadRemove(hactarThread* thread);
int32_t hactarThreadWaitGone(hactarThread* thread);
int32_t hactarThreadSetSleep(hactarThread* thread, uint8_t sleep);

#endif
