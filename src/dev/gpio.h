#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>
#include <stdbool.h>

void LedSetState(uint8_t led, bool state);
void LedSetState8(uint8_t value);
uint8_t JoystickGetState(void);
bool JoystickTestState(uint8_t pos);
void GPIO_INIT(void);

#endif // __GPIO_H__
