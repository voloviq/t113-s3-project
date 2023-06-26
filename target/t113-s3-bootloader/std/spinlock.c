#include "spinlock.h"
#include "atomic.h"

#define SPIINLOCK_INIT_MAGIC (0xDEADBEEFu)
#define SPIINLOCK_LOCKED (1u)
#define SPIINLOCK_UNLOCKED (0u)

void spinlock_init(SpinLock* lock) {
    assert(lock);
    assert(lock->init != SPIINLOCK_INIT_MAGIC);
    lock->init = SPIINLOCK_INIT_MAGIC;
    lock->value = SPIINLOCK_UNLOCKED;
}

void spinlock_lock(SpinLock* lock) {
    assert(lock);
    assert(lock->init == SPIINLOCK_INIT_MAGIC);
    while(!atomic_cas(&lock->value, SPIINLOCK_UNLOCKED, SPIINLOCK_LOCKED))
        ;
}

bool spinlock_trylock(SpinLock* lock) {
    assert(lock);
    assert(lock->init == SPIINLOCK_INIT_MAGIC);
    if(atomic_cas(&lock->value, SPIINLOCK_UNLOCKED, SPIINLOCK_LOCKED)) return 0;

    return false;
}

void spinlock_unlock(SpinLock* lock) {
    assert(lock);
    assert(lock->init == SPIINLOCK_INIT_MAGIC);
    atomic_cas(&lock->value, SPIINLOCK_LOCKED, SPIINLOCK_UNLOCKED);
}