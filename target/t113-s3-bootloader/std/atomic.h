#pragma once
#include <stdatomic.h>
#include <stdint.h>

static inline int atomic_cas(uint32_t* lock_addr, int expected, int desired) {
    return atomic_compare_exchange_strong(lock_addr, &expected, desired);
}