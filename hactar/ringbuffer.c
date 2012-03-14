// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <string.h>
#include <hactar/misc.h>
#include <hactar/ringbuffer.h>

void ringBufferInit(RingBuffer *rb, uint8_t *buffer, size_t size, bool irq)
{
    if(irq)
    {
        rb->lock_ = interruptsDisable;
        rb->unlock_ = interruptsEnable;
    }
    else
    {
        rb->lock_ = schedulerLock;
        rb->unlock_ = schedulerUnlock;
    }

    rb->buffer_ = buffer;
    rb->size_ = size;
    rb->waiter_ = NULL;
    rb->start_ = 0;
    rb->end_ = 0;
    rb->count_ = 0;
}

static void write(RingBuffer *rb, uint8_t *src, size_t count)
{
    if(rb->end_ + count < rb->size_)
    {
        memcpy(&rb->buffer_[rb->end_ + 1], src, count);
        rb->end_ += count;
    }
    else
    {
        size_t part = rb->size_ - rb->end_ - 1;
        memcpy(&rb->buffer_[rb->end_ + 1], src, part);
        memcpy(rb->buffer_, src + part, count - part);
        rb->end_ = count - part - 1;
    }

    __sync_add_and_fetch(&rb->count_, count);

    if(rb->waiter_)
    {
        rb->waiter_->status_ = STATUS_ACTIVE;
        rb->waiter_ = NULL;
    }
}

static void read(RingBuffer *rb, uint8_t *src, size_t count)
{
    if(rb->start_ + count > rb->size_)
    {
        size_t part = rb->size_ - rb->start_;
        memcpy(src, &rb->buffer_[rb->start_], part);
        memcpy(src + part, rb->buffer_, count - part);
        rb->start_ = count - part;
    }
    else
    {
        memcpy(src, &rb->buffer_[rb->start_], count);
        rb->start_ += count;
    }

    __sync_sub_and_fetch(&rb->count_, count);

    if(rb->waiter_)
    {
        rb->waiter_->status_ = STATUS_ACTIVE;
        rb->waiter_ = NULL;
    }
}

void ringBufferWrite(RingBuffer *rb, uint8_t *src, size_t count)
{
    size_t free = rb->size_ - rb->count_;

    // check if there is enough space.
    // if there is a reader active
    while(free < count)
    {
        // if there is not enough space, write what is possible
        write(rb, src, free);
        count -= free;
        src += free;

        // go to sleep and wait for the reader to wake us up again
        rb->lock_();
        // update free and check if it's still not enough
        // or we could go to sleep with no one waking us up again.
        free = rb->size_ - rb->count_;
        if(free < count)
        {
            rb->waiter_ = schedulerActiveThread();
            rb->waiter_->status_ = STATUS_SLEEPING;
            rb->unlock_();
            threadYield();
        }
        else
            rb->unlock_();

        // update again after wakeup
        free = rb->size_ - rb->count_;
    }

    write(rb, src, count);
}

size_t ringBufferWriteIRQ(RingBuffer *rb, uint8_t *src, size_t count)
{
    count = MIN(rb->size_ - rb->count_, count);
    write(rb, src, count);
    return count;
}

void ringBufferRead(RingBuffer *rb, uint8_t *dst, size_t count)
{
    size_t used = rb->count_;

    while(count > used)
    {
        read(rb, dst, used);
        dst += used;
        count -= used;

        rb->lock_();
        if(count > rb->count_)
        {
            rb->waiter_ = schedulerActiveThread();
            rb->waiter_->status_ = STATUS_SLEEPING;
            rb->unlock_();
            threadYield();
        }
        else
            rb->unlock_();

        used = rb->count_;
    }

    read(rb, dst, count);
}

size_t ringBufferReadIRQ(RingBuffer *rb, uint8_t *dst, size_t count)
{
    count = MIN(rb->count_, count);
    read(rb, dst, count);
    return count;
}
