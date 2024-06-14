#include "config.h"
#include "ssp.h"

void ssp_init(void) {
    PCONP |= 0b1 << 21; // PCSSP0
	PCLKSEL1 |= 0b01 << 10; // PCLK_SSP - PCLK_peripheral = CCLK

    // Set pin modes
	PINSEL0 |= 0b10 << 30; // SCK
    PINSEL1 |= 0b10; // SSEL0
	PINSEL1 |= 0b10 << 2; // MISO0
	PINSEL1 |= 0b10 << 4; // MOSI0

	SSP0CR0 |= 0b0111; // 8 bit transfer
	SSP0CR1 |= 0b1 << 1; // Enable SSP

	FIO0DIR |= 1 << 16; // nCS_LCD SSEL OUT
	FIO1DIR |= 1 << 30; // LCD DC
	FIO1DIR |= 1 << 18; // Backlight

    SSP0DMACR = 0b10; // Enable RX and TX DMA
}

void ssp_send(uint8_t data) {
    SSP0DR = data;
    while (SSP0SR & (1 << 4));
}
