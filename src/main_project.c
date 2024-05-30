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
	init_rgb();
	init_i2c(0, 400000);
    INT_INIT();
	
	FIO1PIN |= 1 << 18;
	LCD_INIT();

	CMD_NOP();
	BG_SET_STATIC(0b1111100000011111);
	CMD_VSCRSADD(0);

	RGB_LED_SET((color_t[]){{0x00, 0x00, 0xDA}, {0x0, 0xB4, 0}});

	uint16_t color = 0b1111100000011111;
	touch_t old_state = state;
	while (true)
	{
		if (state.nb_touch != old_state.nb_touch || state.x1 != old_state.x1 || state.y1 != old_state.y1 || state.x2 != old_state.x2 || state.y2 != old_state.y2)
		{
			old_state = state;
			if (state.nb_touch == 1) {
				ZONE_SET_REL_C(state.x1, state.y1, 64, 64);
				CMD_RAMWR();
				for (uint16_t i = 0; i < 64 * 64; i++)
				{
					CMD_COLORDATA16(~color);
				}
			}
		}
	}

	return 0 ;
}

