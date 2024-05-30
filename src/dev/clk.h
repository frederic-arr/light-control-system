#ifndef __CLK_H__
#define __CLK_H__

#include <stdint.h>

#define SYSCLOCK 100000000 // 100 [MHz]

void delay(uint32_t ms);
void CLK_INIT(void);
void SysTick_Handler(void);

#endif // __CLK_H__
