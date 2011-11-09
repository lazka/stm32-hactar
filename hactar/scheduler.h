// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_SCHEDULER_H__
#define HACTAR_SCHEDULER_H__

#include "stm32f10x.h"

#define PRIO_LEVELS (1 << __NVIC_PRIO_BITS)
#define MINIMUM_PRIO  (PRIO_LEVELS - 1)
#define MAXIMUM_PRIO  (0)

#define SVCALL_PRIO (MAXIMUM_PRIO + 1)
#define PENDSV_PRIO (MINIMUM_PRIO)
#define SYSTCK_PRIO (MINIMUM_PRIO - 1)

void hactarInitScheduler(uint32_t frequency);
int configureSystickTimer(uint32_t frequency);

#endif
