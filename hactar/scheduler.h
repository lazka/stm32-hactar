// Copyright 2011-2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_SCHEDULER_H__
#define HACTAR_SCHEDULER_H__

#include <hactar/platform_check.h>
#include <stdint.h>

#include "stm32f10x.h"

#define PRIO_LEVELS     (1 << __NVIC_PRIO_BITS)
#define PRIO_MINIMUM    (PRIO_LEVELS - 1)
#define PRIO_MAXIMUM    (0)
#define PRIO_PENDSV     (PRIO_MINIMUM)
#define PRIO_SYSTICK    (PRIO_MINIMUM - 1)

#define __SVC()     asm volatile ("SVC 0")
#define __PENDSV()  (SCB->ICSR = SCB_ICSR_PENDSVSET)

// #define BASEPRI_SET    (PRIO_SYSTICK << (8 - __NVIC_PRIO_BITS))
// #define BASEPRI_UNSET  (0x0)
// (((1 << 4) - 1) - 1) << (8 - 4) = 0xe0
// no preprocessor possible in inline asm..

#define SCHEDULER_DISABLE() asm volatile \
    ("mov r0, #0xe0     \n"\
     "MSR basepri, r0   \n" : : : "memory", "r0")

#define SCHEDULER_ENABLE()  asm volatile \
    ("mov r0, #0x00     \n"\
     "MSR basepri, r0   \n" : : : "memory", "r0")

#define IRQ_RETURN_PSP 0xFFFFFFFD
#define IRQ_RETURN_MSP 0xFFFFFFF9

typedef enum {
    SLEEPING,
} InactiveStatus;

typedef struct {
    uint8_t active_; // if it should get scheduled
    InactiveStatus inactive_status_; // if not active, what is the reason
    uint32_t sp_; // for saving the stack pointer on context switch
} Thread;

typedef struct {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
} InitStack;

int32_t threadAdd(Thread* thread, void* func, uint8_t* stack, size_t stack_size);
int32_t threadRemove(Thread* thread);
int32_t threadSetSleep(Thread* thread, uint8_t sleep);
void    threadYield(void);

int32_t schedulerInit(uint32_t frequency);
void    schedulerLock(void);
void    schedulerUnlock(void);

#endif
