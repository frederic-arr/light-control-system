#ifndef __CLK_H__
#define __CLK_H__

#include <stdint.h>

#define STCTRL (*((volatile uint32_t *)0xE000E010))
#define STRELOAD (*((volatile uint32_t *)0xE000E014))
#define STCURR (*((volatile uint32_t *)0xE000E018))
#define SYSCLOCK 100000000  // 100 [MHz]

void delay(uint32_t ms);
void clk_init(void);
void SysTick_Handler(void);

#endif  // __CLK_H__
