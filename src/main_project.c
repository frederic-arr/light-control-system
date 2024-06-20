#include <stdbool.h>
#include <stdint.h>
#include <tlm.h>

#include "sys/i2c.h"
#include "sys/lpc1769_reg.h"
#include "sys/utils.h"
// #include "sys/ledrgb.h"

#include "dev/clk.h"
#include "dev/config.h"
#include "dev/gpio.h"
// #include "dev/spi.h"
#include "dev/ft6x06.h"
#include "dev/gpdma.h"
#include "dev/ili9341.h"
#include "dev/luminosity.h"
#include "dev/sprite.h"
#include "dev/ssp0.h"
#include "sprites/bg_day.h"
#include "sprites/bg_night.h"
#include "sprites/light_ped_go.h"
#include "sprites/light_ped_off.h"
#include "sprites/light_ped_stop.h"
#include "sprites/light_warn_off.h"
#include "sprites/light_warn_on.h"
#include "tl.h"

#define DMACIntStat (*(volatile uint32_t *)0x50004000)
#define DMACIntTCStat (*(volatile uint32_t *)0x50004004)
#define DMACIntTCClear (*(volatile uint32_t *)0x50004008)
#define DMACIntErrStat (*(volatile uint32_t *)0x5000400C)
#define DMACIntErrClr (*(volatile uint32_t *)0x50004010)
#define DMACRawIntTCStat (*(volatile uint32_t *)0x50004014)
#define DMACRawIntErrStat (*(volatile uint32_t *)0x50004018)
#define DMACEnbldChns (*(volatile uint32_t *)0x5000401C)
#define DMACSoftBReq (*(volatile uint32_t *)0x50004020)
#define DMACSoftSReq (*(volatile uint32_t *)0x50004024)
#define DMACSoftLBReq (*(volatile uint32_t *)0x50004028)
#define DMACSoftLSReq (*(volatile uint32_t *)0x5000402C)
#define DMACConfig (*(volatile uint32_t *)0x50004030)
#define DMACSync (*(volatile uint32_t *)0x50004034)
#define DMAREQSEL (*(volatile uint32_t *)0x400FC1C4)
#define DMACC0SrcAddr (*(volatile uint32_t *)0x50004100)
#define DMACC0DestAddr (*(volatile uint32_t *)0x50004104)
#define DMACC0LLI (*(volatile uint32_t *)0x50004108)
#define DMACC0Control (*(volatile uint32_t *)0x5000410C)
#define DMACC0Config (*(volatile uint32_t *)0x50004110)

#define BURST1 0x0
#define HALFWORD16_TRANSFER 0x1
#define TERMINAL_INTERRUPT 0x80000000
#define DMA_CFG                                                    \
    ((BURST1 << 12) | (BURST1 << 15) | (HALFWORD16_TRANSFER << 18) \
     | TERMINAL_INTERRUPT)

void INT_INIT(void) {
    ISER0 = (1 << 21) | (1 << 17) | (1 << 14);  // EINT3, RTC, SSP0
}

ft6x06_touch_t state = {0};
bool pedestrian_request = false;

void DMA_IRQHandler(void) {
    DMACIntTCClear = 0b1;
    return;
}

void EINT3_IRQHandler(void) {
    if (ft6x06_get_touch(&state)) {
        // The user is not trying to zoom or do anything wired
        bool is_no_gesture = state.gesture_id == GESTURE_NO_GESTURE;

        // The user is touching the screen with one finger
        bool is_one_touch = state.nb_touch_points == 1;

        // The user just touched the screen, he's not moving his finger
        bool is_new = state.p1.event_flag == EVENT_PRESS_DOWN;

        if (is_no_gesture && is_one_touch && is_new) {
            pedestrian_request = true;
        }
    }

    IO2IntClr = (1 << 10);
    IO2IntClr = (1 << 11);
    IO0IntClr = (1 << 19);
}

int main(void) {
    SystemInit();

    tl_init();

    GPIO_INIT();
    CLK_INIT();
    init_i2c(0, 400000);
    ssp0_init();
    gpdma_configure();

    FIO1PIN |= 1 << 18;
    ili9341_init();

    LUMINOSITY_INIT();

    INT_INIT();

    ili9341_cmd_nop();
    ili9341_bg_set(0xF0);
    ili9341_cmd_vscrsadd(0);

    DRAW_SPRITE(0, 0, 240, 320, bg_day, false);
    while (true) {
        tl_draw();
    }

    return 0;
}
