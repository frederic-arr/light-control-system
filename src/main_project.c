#include <stdbool.h>
#include <stdint.h>
#include <tlm.h>

#include "dev/apds9960.h"
#include "dev/clk.h"
#include "dev/config.h"
#include "dev/ft6x06.h"
#include "dev/gpio.h"
#include "dev/ili9341.h"
#include "dev/sprite.h"
#include "dev/ssp0.h"
#include "sprites/bg_day.h"
#include "sprites/bg_night.h"
#include "sys/i2c.h"
#include "sys/lpc1769_reg.h"
#include "sys/utils.h"
#include "tl.h"

void nvec_init(void) {
    ISER0 = (1 << 21) | (1 << 17) | (1 << 14);  // EINT3, RTC, SSP0
}

ft6x06_touch_t state = {0};
void EINT3_IRQHandler(void) {
    if (ft6x06_get_touch(&state)) {
        // The user is not trying to zoom or do anything wired
        bool is_no_gesture = state.gesture_id == GESTURE_NO_GESTURE;

        // The user is touching the screen with one finger
        bool is_one_touch = state.nb_touch_points == 1;

        // The user just touched the screen, he's not moving his finger
        bool is_new = state.p1.event_flag == EVENT_PRESS_DOWN;

        if (is_no_gesture && is_one_touch && is_new) {
            tlm_intersection_request_pedestrian(tl_get());
        }

        IO2IntClr = (1 << 11);
        return;
    }

    IO2IntClr = (1 << 10);
    IO0IntClr = (1 << 19);
}

int main(void) {
    SystemInit();
    FIO1PIN |= 1 << 18;

    tl_init();

    gpio_init();
    clk_init();
    init_i2c(0, 400000);

    ssp0_init();
    ili9341_init();
    apds9960_init();
    nvec_init();

    ili9341_cmd_nop();
    ili9341_bg_set(0xF0);
    ili9341_cmd_vscrsadd(0);

    bool was_dark = apds9960_get_luminosity() < 200;
    DRAW_SPRITE(0, 0, 240, 320, bg_day, false);
    while (true) {
        uint16_t luminosity = apds9960_get_luminosity();
        bool is_dark = luminosity < 200;
        if (is_dark && !was_dark) {
            DRAW_SPRITE(0, 0, 240, 320, bg_night, false);
            tlm_intersection_request_maintenance(tl_get());
        } else if (!is_dark && was_dark) {
            DRAW_SPRITE(0, 0, 240, 320, bg_day, false);
            tlm_intersection_request_unblock(tl_get());
        }

        was_dark = is_dark;
        tl_draw(was_dark);
    }

    return 0;
}
