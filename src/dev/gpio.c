#include "gpio.h"

#include <stdint.h>

#include "config.h"

void gpio_init(void) {
    FIO1DIR = 0;
    FIO2DIR = 0xFF;  // P2.0-7 are output

    IO2IntEnF = (1 << 10) | (1 << 11);  // BTN_A, TOUCH
    IO0IntEnF = (1 << 19);              // BTN_B
}
