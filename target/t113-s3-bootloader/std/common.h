#pragma once
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef __ASM
#define __ASM __asm
#endif

#define __NOP() __ASM volatile("nop")

void delay_nop(uint32_t n);