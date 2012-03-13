// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTER_RINGBUFFER_H__
#define HACTER_RINGBUFFER_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <hactar/scheduler.h>

/*
 * Ring buffer for interprocess, IRQ <-> process data passing.
 * Each buffer only supports one reader and one writer (threads or interrupts).
 * If read/write from an ISR, use the WriteIRQ/ReadIRQ functions and
 * pass irq=true to the init function.
 * In case of an empty buffer read or a full buffer write, the non IRQ
 * functions will block until the the data is written/read. The IRQ
 * functions will return the amount of read/written data instead, so in
 * case of writing 10 bytes to a buffer with 4 bytes free, 6 bytes will
 * get lost.
 */

typedef struct RingBuffer {
    uint8_t *buffer_;
    void    (*lock_)();     // lock function
    void    (*unlock_)();   // unlock function
    size_t  size_;          // size of the buffer (fixed)
    size_t  start_;         // first index of data (reader)
    size_t  end_;           // last index of data (writer)
    size_t  count_;         // amount of data in buffer (shared)
    Thread  *waiter_;       // a thread that is waiting for data to write/read
} RingBuffer;

void ringBufferInit(RingBuffer *rb, uint8_t *buffer, size_t size, bool irq);

void ringBufferWrite(RingBuffer *rb, uint8_t *src, size_t count);
void ringBufferRead(RingBuffer *rb, uint8_t *dst, size_t count);

size_t ringBufferWriteIRQ(RingBuffer *rb, uint8_t *src, size_t count);
size_t ringBufferReadIRQ(RingBuffer *rb, uint8_t *dst, size_t count);

#endif
