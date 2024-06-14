#include "spi.h"
#include "config.h"

#define SSP0DMACR (*((volatile uint32_t *)0x40088024))

void SPI_INIT(void)
{
    PCONP |= 0b1 << 21; // PCSSP0
	PCLKSEL0 |= 0b01 << 16; // PCLK_SPI - PCLK_peripheral = CCLK

    // Set pin modes
	PINSEL0 |= 0b11 << 30; // SCK
	PINSEL1 |= 0b11 << 2; // MISO
	PINSEL1 |= 0b11 << 4; // MOSI
	SSP0CPSR = 254;
	SSP0CR0 |= 0b0111 // 8 bit transfer
			| (0b00 << 4); // SPI mode
	SSP0CR1 |= 0b1 << 1; // Enable SSP

	FIO0DIR |= 1 << 16; // nCS_LCD SSEL OUT
	FIO1DIR |= 1 << 30; // LCD DC
	FIO1DIR |= 1 << 18; // Backlight

	SSP0DMACR = 0b11; // Enable RX and TX DMA
}
