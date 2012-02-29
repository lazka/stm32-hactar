// from Android/Linux spinlock.h
// + modifications to make it compile with thumb2

#ifndef HACTAR_LOCKS_H__
#define HACTAR_LOCKS_H__

typedef struct spinlock_t{
    volatile unsigned int lock;
} spinlock_t;

typedef struct Thread Thread;
typedef struct mutex_t{
    spinlock_t lock;
    Thread *owner;
    Thread *waitqueue;
} mutex_t;

void spinInit(spinlock_t *lock);
void spinLock(spinlock_t *lock);
void spinUnlock(spinlock_t *lock);
int  spinTrylock(spinlock_t *lock);

void mutexInit(mutex_t *lock);
void mutexLock(mutex_t *lock);
void mutexUnlock(mutex_t *lock);
int  mutexTrylock(mutex_t *lock);

#endif
