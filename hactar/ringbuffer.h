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
 * 
 * Each buffer only supports one reader and one writer (threads or interrupts).
 * If read/write from an ISR, use the WriteIRQ/ReadIRQ functions and
 * pass irq=true to the init function.
 * 
 * In case of an empty buffer read or a full buffer write, the non IRQ
 * functions will block until the the data is written/read.
 *
 * The IRQ functions will return the amount of read/written data instead,
 * so in case of writing 10 bytes to a buffer with 4 bytes free, 6 bytes will
 * get lost.
 *
 * The TryRead/TryWrite functions don't block (they just call the IRQ ones)
 *
 * When SetDataCallback is called with a function pointer, the next time
 * data is written to the buffer, the function will be called once.
 * This way you can disable an interrupt if there is no more data,
 * and enable it in the callback again if new data comes in.
 */

/*
 * USART receive example:
 *  - ringBufferInit with irq=true
 *  - ringBufferRead in process
 *  - ringBufferWriteIRQ in interrupt (log error if count != returned value)
 *
 * USART send example:
 *  - ringBufferInit with irq=true
 *  - ringBufferWrite in process
 *  - ringBufferReadIRQ in interrupt
 *   - if returned amount == 0 disable interrupt and call
 *     ringBufferSetDataCallback with a function that enables the interrupt
 *     again.
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
    void    (*callback_)(); // new data callback for irq
} RingBuffer;

// Initialize or clears the buffer, pass irq=true if interrupts are involved
void ringBufferInit(RingBuffer *rb, uint8_t *buffer, size_t size, bool irq);

// Read/Write, blocks until the whole amount is written/read
void ringBufferWrite(RingBuffer *rb, uint8_t *src, size_t count);
void ringBufferRead(RingBuffer *rb, uint8_t *dst, size_t count);

// Read/Writes the amount that is possible and returns it
size_t ringBufferTryWrite(RingBuffer *rb, uint8_t *src, size_t count);
size_t ringBufferTryRead(RingBuffer *rb, uint8_t *dst, size_t count);

// Same as TryWrite/Read
size_t ringBufferWriteIRQ(RingBuffer *rb, uint8_t *src, size_t count);
size_t ringBufferReadIRQ(RingBuffer *rb, uint8_t *dst, size_t count);

// After calling, will call cb once after the next buffer write
void ringBufferSetDataCallback(RingBuffer *rb, void (*cb)());

#endif
