#pragma once
#include <std/common.h>
#include <t113s3/cmsis_t113s3.h>

void hal_gpio_init(void);

typedef enum {
    gpio_config_input = 0b0000,
    gpio_config_output = 0b0001,
    gpio_config_function2 = 0b0010,
    gpio_config_function3 = 0b0011,
    gpio_config_function4 = 0b0100,
    gpio_config_function5 = 0b0101,
    gpio_config_function6 = 0b0110,
    gpio_config_function7 = 0b0111,
    gpio_config_function8 = 0b1000,
    gpio_config_function14 = 0b1110,
    gpio_config_external_interrupt = 0b1110,
    gpio_config_io_diable = 0b1111
} GpioConfig;

void hal_gpio_set_config(GPIO_TypeDef* port, uint32_t pin, GpioConfig config);

void hal_gpio_write(GPIO_TypeDef* port, uint32_t pin, bool value);