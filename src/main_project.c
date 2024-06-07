#include <stdbool.h>
#include <stdint.h>

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
#include "tl.h"

void INT_INIT(void) {
	ISER0 = (1 << 21) | (1 << 17); // EINT3, RTC
}

touch_t state = {0};
void EINT3_IRQHandler(void) {
	state = TOUCH_STATE();
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

	DRAW_SPRITE(0, 0, 240, 320, bg_day, was_dark);
	inter_draw(&intersection, was_dark);
	while (true)
	{
		bool is_dark = LUMINOSITY_IS_DARK(); 
		intersection.is_disabled = is_dark;
		if (is_dark != was_dark) {
			DRAW_SPRITE(0, 0, 240, 320, bg_day, is_dark);
			if (!is_dark && was_dark) {
				inter_enable(&intersection);
			} else {
				inter_disable(&intersection);
			};
			intersection.time = 0;
			inter_draw(&intersection, is_dark);
			was_dark = is_dark;
			continue;
		}

		inter_transition(&intersection);
		inter_draw(&intersection, is_dark);
		intersection.time++;
	}

	return 0 ;
}

