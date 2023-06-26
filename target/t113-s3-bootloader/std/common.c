#include "common.h"

void delay_nop(uint32_t count) {
    for(uint32_t i = 0; i < count; i++) {
        __NOP();
    }
}