// from Android/Linux spinlock.h
// + modifications to make it compile with thumb2

#include <stddef.h>
#include <hactar/locks.h>
#include <hactar/misc.h>

void spinInit(spinlock_t *lock)
{
    lock->lock = 0;
}

void spinLock(spinlock_t *lock)
{
    unsigned long tmp;

    __asm__ __volatile__(
"1: ldrex   %0, [%1]\n"
"   teq     %0, #0\n"
"   itt      eq\n"
"   strexeq %0, %2, [%1]\n"
"   teqeq   %0, #0\n"
"   bne     1b"
    : "=&r" (tmp)
    : "r" (&lock->lock), "r" (1)
    : "cc");

    __asm__ ("": : :"memory");
}

void spinUnlock(spinlock_t *lock)
{
    __asm__ ("": : :"memory");

    __asm__ __volatile__(
"   str     %1, [%0]\n"
    :
    : "r" (&lock->lock), "r" (0)
    : "cc");
}

int spinTrylock(spinlock_t *lock)
{
    unsigned long tmp;

    __asm__ __volatile__(
"   ldrex   %0, [%1]\n"
"   teq     %0, #0\n"
"   it      eq\n"
"   strexeq %0, %2, [%1]"
    : "=&r" (tmp)
    : "r" (&lock->lock), "r" (1)
    : "cc");

    if (tmp == 0) {
        __asm__ ("": : :"memory");
        return 0;
    } else {
        return -1;
    }
}

void mutexInit(mutex_t *lock)
{
    spinInit(&lock->lock);
    lock->owner = NULL;
}

int  mutexTrylock(mutex_t *lock)
{
    spinLock(&lock->lock);
    if(lock->owner == NULL)
    {
        lock->owner = schedulerActiveThread();;
        spinUnlock(&lock->lock);
        return 0;
    }
    spinUnlock(&lock->lock);
    return -1;
}

void mutexLock(mutex_t *lock)
{
    Thread *last, *self;
    self = schedulerActiveThread();

    spinLock(&lock->lock);
    if(lock->owner == NULL)
    {
        lock->owner = self;
        spinUnlock(&lock->lock);
    }
    else
    {
        assert(lock->owner != self);

        // Append to the waiting list
        last = lock->owner;
        while(last->next_)
            last = last->next_;
        last->next_ = self;

        // Set mutex
        self->mutex_ = lock;

        // set sleeping while scheduling is locked and unlock spinlock
        schedulerLock();
        self->status_ = STATUS_MUTEX;
        spinUnlock(&lock->lock);
        schedulerUnlock();
        threadYield();
    }
}

void mutexUnlock(mutex_t *lock)
{
    Thread *sleeper, *self;
    self = schedulerActiveThread();

    spinLock(&lock->lock);

    assert(lock->owner == self);

    // If no next it's simply NULL
    sleeper = self->next_;
    lock->owner = sleeper;
    if(sleeper != NULL)
    {
        sleeper->mutex_ = NULL;
        sleeper->status_ = STATUS_ACTIVE;
        self->next_ = NULL;
    }

    spinUnlock(&lock->lock);
}
