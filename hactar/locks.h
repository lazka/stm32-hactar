// from Android/Linux spinlock.h
// + modifications to make it compile with thumb2

#ifndef HACTAR_LOCKS_H__
#define HACTAR_LOCKS_H__

typedef struct {
    volatile unsigned int lock;
} spinlock_t;

void hactarSpinInit(spinlock_t *lock);

void hactarSpinLock(spinlock_t *lock);

void hactarSpinUnlock(spinlock_t *lock);

int hactarSpinTrylock(spinlock_t *lock);

#endif
