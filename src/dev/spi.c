#include "spi.h"
#include "config.h"

void spi_init(void)
{
    PCONP |= 0b1 << 8; // PCSPI
	PCLKSEL0 |= 0b01 << 16; // PCLK_SPI - PCLK_peripheral = CCLK
	S0SPCCR |= 8; // 1MHz

    // Set pin modes
	PINSEL0 |= 0b11 << 30; // SCK
	PINSEL1 |= 0b11 << 2; // MISO
	PINSEL1 |= 0b11 << 4; // MOSI
	S0SPCR |= 1 << 5;

	FIO0DIR |= 1 << 16; // nCS_LCD SSEL OUT
	FIO1DIR |= 1 << 30; // LCD DC
	FIO1DIR |= 1 << 18; // Backlight
}

void spi_write(uint8_t data)
{
	S0SPDR = data;
	while (!(S0SPSR & (1 << 7)));
}

void spi_read(uint8_t *data)
{
	while (!(S0SPSR & (1 << 0)));
	*data = S0SPDR;
}