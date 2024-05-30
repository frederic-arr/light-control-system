#include "clk.h"
#include "config.h"

uint32_t clock = 0;
void SysTick_Handler(void) {
	clock++;
}

void delay(uint32_t ms) {
    uint32_t target = clock + ms / 10;
    while (clock < target) { ; };
}

void CLK_INIT(void) {
	STRELOAD = (SYSCLOCK / 100) - 1;
	STCTRL = 0b111;
}
