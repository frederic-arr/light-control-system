#include <stdbool.h>
#include <stdint.h>
#include <tlm.h>

#include "sys/lpc1769_reg.h"
#include "sys/utils.h"
#include "sys/i2c.h"
// #include "sys/ledrgb.h"

#include "dev/config.h"
#include "dev/clk.h"
#include "dev/gpio.h"
// #include "dev/spi.h"
#include "dev/ili9341.h"
#include "dev/ft6x06.h"
#include "dev/luminosity.h"
#include "dev/sprite.h"
#include "dev/ssp0.h"
#include "dev/gpdma.h"

#include "sprites/bg_day.h"
#include "sprites/bg_night.h"
#include "sprites/light_warn_on.h"
#include "sprites/light_warn_off.h"
#include "sprites/light_ped_stop.h"
#include "sprites/light_ped_go.h"
#include "sprites/light_ped_off.h"
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
#define DMA_CFG ((BURST1 << 12) | (BURST1 << 15) \
                | (HALFWORD16_TRANSFER << 18) | \
                TERMINAL_INTERRUPT)


void INT_INIT(void) {
	ISER0 = (1 << 21) | (1 << 17) | (1 << 14); // EINT3, RTC, SSP0
}

ft6x06_touch_t state = {0};
bool pedestrian_request = false;

void DMA_IRQHandler(void) {
    // if (DMACIntTCStat & 0b1) {
    //     DMACIntTCClear = 0b1;
    // }
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

int main(void)
{
	SystemInit();
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

    wait_for_ssp_bus_free();
    SET_LCD_CMD_MODE();
    SSP0DR = 0x2C;
    
    wait_for_ssp_bus_free();
    SET_LCD_DATA_MODE();
    uint16_t buf[0xFFF];
    for (int i = 0; i < 35; i++) {
        wait_for_ssp_bus_free();
        gpdma_transfer(&bg_day[0xFFF * i], 0xFFF, true);
        delay(10);
    }

    while (true) {;};

	// ili9341_cmd_nop();
	// ili9341_bg_set(0xF0);
	// ili9341_cmd_vscrsadd(0);

    // ili9341_cmd_ramwr();
    // FIO0CLR = (1 << 16);
	// FIO1SET = (1 << 30);
    // DMACC0SrcAddr = (uint32_t)bg_day;
    // DMACC0Control = (0xFF0 & 0xFFF) | (0x01 << 18) | (1 << 26) | (1 << 31);
    // DMACC0Config = 1 | (1 << 11) |(1 << 15);

    // return;
	
	bool was_dark = LUMINOSITY_IS_DARK();
	uint16_t time = 0;

	intersection_t intersection = inter_init();

	// tlm_init();
	// uint8_t a = tlm_app_data_next();
	// uint8_t b = tlm_app_data_next();
	// uint8_t c = tlm_app_data_next();

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
