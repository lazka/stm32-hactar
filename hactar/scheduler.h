// Copyright 2011-2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_SCHEDULER_H__
#define HACTAR_SCHEDULER_H__

#include <hactar/platform_check.h>
#include <hactar/locks.h>
#include <stdint.h>
#include <reent.h>

#include "stm32f10x.h"

#define PRIO_LEVELS     (1 << __NVIC_PRIO_BITS)
#define PRIO_MINIMUM    (PRIO_LEVELS - 1)
#define PRIO_MAXIMUM    (0)
#define PRIO_PENDSV     (PRIO_MINIMUM)
#define PRIO_SYSTICK    (PRIO_MINIMUM - 1)
#define PRIO_SVCALL     (PRIO_MINIMUM)

#define __SVC()     asm volatile ("SVC 0")
#define __PENDSV()  (SCB->ICSR = SCB_ICSR_PENDSVSET)

#define INTERRUPTS_DISABLE() asm volatile ("cpsid   i" : : : "memory")
#define INTERRUPTS_ENABLE() asm volatile ("cpsie   i" : : : "memory")

#define BASEPRI_SET    (PRIO_SVCALL << (8 - __NVIC_PRIO_BITS))
#define BASEPRI_UNSET  (0x0)

#define SCHEDULER_DISABLE() asm volatile \
    ("mov r0, #0xe0     \n"\
     "MSR basepri, r0   \n" : : "i" (BASEPRI_SET) : "memory", "r0")

#define SCHEDULER_ENABLE()  asm volatile \
    ("mov r0, #0x00     \n"\
     "MSR basepri, r0   \n" : : "i" (BASEPRI_UNSET) : "memory", "r0")

#define IRQ_RETURN_PSP 0xFFFFFFFD
#define IRQ_RETURN_MSP 0xFFFFFFF9
#define IRQ_RETURN_ISR 0xFFFFFFF1

typedef enum ThreadStatus {
    STATUS_ACTIVE=1,
    STATUS_SLEEPING=2,
    STATUS_MUTEX=3,
    STATUS_DIED=4,
} ThreadStatus;

/*
 * Reentrancy, two ways:
 *  1) __DYNAMIC_REENT__ is defined: newlib will call __getreent to get
 *     the currently active reent struct
 *  2) otherwise, we need to switch the active reent struct pointer
 *     on each context switch (this is the default in codesourcery g++)
 */

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
    void (*pc)(void);
    uint32_t psr;
} InitStack;

typedef struct Thread Thread;
typedef struct mutex_t mutex_t;

typedef struct Thread{
    ThreadStatus status_; // if it should get scheduled

    InitStack *sp_; // for saving the stack pointer on context switch

    Thread *next_; // used for mutex waiting list

#ifdef HACTAR_NEWLIB_REENT
    struct _reent reent_;
#endif
} Thread;

int32_t threadAdd(Thread* thread, void* func, uint8_t* stack, size_t stack_size);
int32_t threadRemove(Thread* thread);
int32_t threadSetSleep(Thread* thread, uint8_t sleep);
void    threadYield(void);

int32_t schedulerInit(uint32_t frequency);
void    schedulerLock(void);
void    schedulerUnlock(void);
Thread* schedulerActiveThread(void);

void schedulerISRNewlibStart(void);
void schedulerISRNewlibEnd(void);

#endif
