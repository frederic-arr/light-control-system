#include "clk.h"

#include <tlm.h>

#include "../tl.h"
#include "config.h"

uint32_t clock = 0;
void SysTick_Handler(void) {
    STRELOAD = (SYSCLOCK / 100) - 1;
    clock++;
    tlm_intersection_tick(tl_get(), 100);
}

void delay(uint32_t ms) {
    uint32_t target = (clock + ms / 10) + 1;
    while (clock < target) {
        ;
    };
}

void CLK_INIT(void) {
    STRELOAD = (SYSCLOCK / 100) - 1;
    STCTRL = 0b111;
}
