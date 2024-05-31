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

#include "sprites/bg_day.h"
#include "sprites/bg_night.h"
#include "dev/luminosity.h"

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

typedef struct {
	uint8_t r,
			g,
			b;
} color_t;

int main(void)
{
	SystemInit();
	GPIO_INIT();
    CLK_INIT();
	SPI_INIT();
	// init_rgb();
	init_i2c(0, 400000);
	
	FIO1PIN |= 1 << 18;
	LCD_INIT();
	LUMINOSITY_INIT();

	INT_INIT();

	CMD_NOP();
	BG_SET_STATIC(0x00);
	DRAW_SPRITE(0, 0, 240, 320, 0x00, bg_day);
	CMD_VSCRSADD(0);

	// RGB_LED_SET((color_t[]){{50, 51, 52}, {53, 54, 55}});
	
	bool was_dark = LUMINOSITY_IS_DARK();
	while (true)
	{
		bool is_dark = LUMINOSITY_IS_DARK(); 
		if (is_dark != was_dark) {
			DRAW_SPRITE(0, 0, 240, 320, 0x00, is_dark ? bg_night : bg_day);
			was_dark = is_dark;
		}
	}

	return 0 ;
}

