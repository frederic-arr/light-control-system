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
#include "sprites/light_veh_off.h"
#include "sprites/light_veh_wait.h"
#include "sprites/light_veh_ready.h"
#include "sprites/light_veh_go.h"
#include "sprites/light_veh_stop.h"

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

uint8_t sequence[6][5] = {
	{2,0, 2,2, 0},
	{3,0, 3,2, 1},
	{0,0, 0,2, 2},
	{1,1, 0,3, 3},
	{2,2, 0,0, 0},
	{2,3, 1,1, 0},
};

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
	DRAW_SPRITE(0, 0, 240, 320, 0x00, bg_day, 1);
	CMD_VSCRSADD(0);

	// RGB_LED_SET((color_t[]){{50, 51, 52}, {53, 54, 55}});
	
	bool was_dark = LUMINOSITY_IS_DARK();
	uint16_t time = 0;
	uint16_t state = 0;
	uint8_t* light_veh[4] = {light_veh_wait, light_veh_ready, light_veh_go, light_veh_stop};
	while (true)
	{
		bool is_dark = LUMINOSITY_IS_DARK(); 
		if (is_dark != was_dark) {
			DRAW_SPRITE(0, 0, 240, 320, 0x00, bg_day, is_dark ? 0.75 : 1.0);
			was_dark = is_dark;
		}

		// CHA->GVA
		DRAW_SPRITE(38, 70, 14, 32, 0x00, light_veh[sequence[state][0]], is_dark ? 0.75 : 1.0);
		DRAW_SPRITE(68, 70, 14, 32, 0x00, light_veh[sequence[state][1]], is_dark ? 0.75 : 1.0);

		// GVA->*
		DRAW_SPRITE(90, 216, 14, 32, 0x00, light_veh[sequence[state][2]], is_dark ? 0.75 : 1.0);
		DRAW_SPRITE(136, 216, 14, 32, 0x00, light_veh[sequence[state][3]], is_dark ? 0.75 : 1.0);

		// VEG->GVA
		DRAW_SPRITE(195, 135, 14, 32, 0x00, light_veh[sequence[state][4]], is_dark ? 0.75 : 1.0);
		time = (time + 1) % 100;
		if (time == 0) {
			state = (state + 1) % 6;
		}
	}

	return 0 ;
}

