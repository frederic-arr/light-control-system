#include "gpio.h"
#include <stdint.h>
#include <stdbool.h>
#include "config.h"

void LedSetState(uint8_t led, bool state) {
    FIO2MASK = ~0xFF;
    FIO2PIN = (uint8_t)state << led;
}

void LedSetState8(uint8_t value) {
    FIO2MASK = ~0xFF;
    FIO2PIN = value;
}

uint8_t JoystickGetState(void) {
    uint32_t data;
    data = FIO1PIN;
    return (uint8_t)((data >> 19) & 0b11111);
}

bool JoystickTestState(uint8_t pos) {
    return (JoystickGetState() & (1 << pos)) == 0;
}

uint8_t ButtonGetState(void) {
    FIO2MASK = ~(1 << 10);
    uint32_t dataA;
    dataA = FIO2PIN;
    dataA = (dataA >> 10) & 1;

    FIO0MASK = ~(1 << 19);
    uint32_t dataB;
    dataB = FIO0PIN;
    dataB = (dataB >> 19) & 1;

    return (uint8_t)((dataA << 1) | dataB);
}

bool ButtonTestState(uint8_t pos) {
    return ((ButtonGetState() >> pos) & 1) == 0;
}

void GPIO_INIT(void) {
    FIO1DIR = 0;
    FIO2DIR = 0xFF; // P2.0-7 are output

	IO2IntEnF = (1 << 10) | (1 << 11); // BTN_A, TOUCH
	IO0IntEnF = (1 << 19); // BTN_B
}
