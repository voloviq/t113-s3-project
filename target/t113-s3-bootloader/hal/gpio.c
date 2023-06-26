#include "gpio.h"
#include <std/spinlock.h>

SpinLock gpio_lock;

void hal_gpio_init(void) {
    spinlock_init(&gpio_lock);
}

void hal_gpio_set_config(GPIO_TypeDef* port, uint32_t pin, GpioConfig config) {
    spinlock_lock(&gpio_lock);
    uint32_t cfg_index = pin / (32 / 4);
    uint32_t cfg_offset = (pin % (32 / 4)) * 4;
    uint32_t cfg_reg = port->CFG[cfg_index];
    cfg_reg &= ~(0b1111 << cfg_offset);
    cfg_reg |= (((uint32_t)config & 0b1111) << cfg_offset);
    port->CFG[cfg_index] = cfg_reg;
    spinlock_unlock(&gpio_lock);
}

void hal_gpio_write(GPIO_TypeDef* port, uint32_t pin, bool value) {
    spinlock_lock(&gpio_lock);
    uint32_t data = port->DATA;
    if(value > 0) {
        data |= (1 << pin);
    } else {
        data &= ~(1 << pin);
    }
    port->DATA = data;
    spinlock_unlock(&gpio_lock);
}