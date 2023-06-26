#include <stddef.h>
#include <stdbool.h>
#include <t113s3/cmsis_t113s3.h>

// Cortex-A7/A9 handlers
void Undef_Handler(void) {
}

void SWI_Handler(void) {
}

void PAbort_Handler(void) {
}

void DAbort_Handler(void) {
}

void FIQ_Handler(void) {
}

void Hyp_Handler(void) {
}

void IRQ_Handler_GIC(void) {
}

void IRQ_Handler(void) {
}

void SystemInit(void) {
}

int main(void);

void _start(void) {
    main();
}

#include <hal/gpio.h>
#include <std/common.h>

int main(void) {
    // led gpio PD22/PWM7
    hal_gpio_init();
    hal_gpio_set_config(GPIOD, 22, GpioConfigOutput);

    while(true) {
        hal_gpio_write(GPIOD, 22, 1);
        delay_nop(500000);
        hal_gpio_write(GPIOD, 22, 0);
        delay_nop(500000);
    }

    return 0;
}