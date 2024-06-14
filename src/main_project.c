#include <stdbool.h>
#include <stdint.h>
#include <tlm.h>

#include "sys/lpc1769_reg.h"
#include "sys/utils.h"
#include "sys/i2c.h"
#include "sys/ledrgb.h"

#include "dev/config.h"
#include "dev/clk.h"
#include "dev/gpio.h"
#include "dev/spi.h"
#include "dev/ili9341.h"
#include "dev/ft6x06.h"
#include "dev/luminosity.h"
#include "dev/sprite.h"

#include "sprites/bg_day.h"
#include "sprites/bg_night.h"
#include "sprites/light_warn_on.h"
#include "sprites/light_warn_off.h"
#include "sprites/light_ped_stop.h"
#include "sprites/light_ped_go.h"
#include "sprites/light_ped_off.h"
#include "tl.h"

void INT_INIT(void) {
	ISER0 = (1 << 21) | (1 << 17); // EINT3, RTC
}

ft6x06_touch_t state = {0};
bool pedestrian_request = false;
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

int main(void)
{
	SystemInit();
	GPIO_INIT();
    CLK_INIT();
	SPI_INIT();
	init_i2c(0, 400000);
	
	FIO1PIN |= 1 << 18;
	ili9341_init();
	LUMINOSITY_INIT();

	INT_INIT();

	ili9341_cmd_nop();
	ili9341_bg_set(0x00);
	ili9341_cmd_vscrsadd(0);
	
	bool was_dark = LUMINOSITY_IS_DARK();
	uint16_t time = 0;

	intersection_t intersection = inter_init();

	tlm_init();
	uint8_t a = tlm_app_data_next();
	uint8_t b = tlm_app_data_next();
	uint8_t c = tlm_app_data_next();

	DRAW_SPRITE(0, 0, 240, 320, bg_day, was_dark);
	inter_draw(&intersection, was_dark);
	uint8_t stabilization_factor = 0;
	while (true)
	{
		bool is_dark = LUMINOSITY_IS_DARK(); 
		intersection.is_disabled = is_dark;
		if (pedestrian_request) {
			inter_block(&intersection);
			pedestrian_request = false;
		}

		if (is_dark != was_dark) {
			DRAW_SPRITE(0, 0, 240, 320, bg_day, is_dark);
			if (!is_dark && was_dark) {
				inter_enable(&intersection);
			} else {
				inter_disable(&intersection);
			};
			stabilization_factor = 0;
			intersection.stabilization_factor = 0;
			intersection.time = 0;
			inter_draw(&intersection, is_dark);
			was_dark = is_dark;
			continue;
		}

		inter_transition(&intersection);
		inter_draw(&intersection, is_dark);
		DRAW_SPRITE(200, 65, 14, 14, stabilization_factor == 0 ? light_warn_on : light_warn_off, is_dark);
		DRAW_SPRITE(85, 110, 18, 16, intersection.is_blocked ? light_ped_go : light_ped_stop, is_dark);
		DRAW_SPRITE(165, 150, 18, 16, intersection.is_blocked ? light_ped_go : light_ped_stop, is_dark);
		intersection.time++;
		if (intersection.time % 25 == 0) {
			stabilization_factor = (stabilization_factor + 1) % 2;
		}
	}

	return 0 ;
}

