#pragma once
#include "common.h"

typedef struct {
    uint32_t init;
    uint32_t value;
} SpinLock;

void spinlock_init(SpinLock* lock);

void spinlock_lock(SpinLock* lock);

bool spinlock_trylock(SpinLock* lock);

void spinlock_unlock(SpinLock* lock);