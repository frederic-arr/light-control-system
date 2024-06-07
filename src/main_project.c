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
#include "dev/lcd.h"
#include "dev/touch.h"
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

touch_t state = {0};
bool pedestrian_request = false;
void EINT3_IRQHandler(void) {
	state = TOUCH_STATE();
	if (state.nb_touch >= 1) {
		pedestrian_request = true;
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
	LCD_INIT();
	LUMINOSITY_INIT();

	INT_INIT();

	CMD_NOP();
	BG_SET_STATIC(0x00);
	CMD_VSCRSADD(0);
	
	bool was_dark = LUMINOSITY_IS_DARK();
	uint16_t time = 0;

	intersection_t intersection = inter_init();

	uint8_t res = calling_rust_from_mcu(12, 22);

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

