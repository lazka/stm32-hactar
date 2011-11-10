// from Android/Linux spinlock.h
// + modifications to make it compile with thumb2

#ifndef HACTAR_LOCKS_H__
#define HACTAR_LOCKS_H__

typedef struct {
    volatile unsigned int lock;
} spinlock_t;

void hactarSpinInit(spinlock_t *lock)
{
    lock->lock = 0;
}

void hactarSpinLock(spinlock_t *lock)
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

void hactarSpinUnlock(spinlock_t *lock)
{
    __asm__ ("": : :"memory");

    __asm__ __volatile__(
"   str     %1, [%0]\n"
    :
    : "r" (&lock->lock), "r" (0)
    : "cc");
}

int hactarSpinTrylock(spinlock_t *lock)
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
        return 1;
    } else {
        return 0;
    }
}

#endif
