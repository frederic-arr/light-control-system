#include "ssp0.h"

#include "config.h"

void ssp0_init(void) {
    PCONP |= 1 << 21;  // Power on SSP0

    PINSEL0 &= ~(0b11 << 30);  // Reset PINSEL0 for SPI0
    PINSEL0 |= 0b10 << 30;     // P0.15 - SCK0

    PINSEL1 &= ~0b111111;  // Reset PINSEL1 for SPI0
    PINSEL1 |= 0b10 << 0;  // P0.16 - SSEL0
    PINSEL1 |= 0b10 << 2;  // P0.17 - MISO0
    PINSEL1 |= 0b10 << 4;  // P0.18 - MOSI0

    PCLKSEL1 &= ~(0b11 << 10);  // Reset PCLKSEL1 for SSP0
    PCLKSEL1 |= 0b01 << 10;     // Set PCLK_SSP0 to CCLK/1 (= 100 [MHz])

    SSP0CR0 = 0b0111       // 8-bit mode
              | 0b00 << 4  // SPI mode
              | 0b0 << 6   // CPOL = 0
              | 0b0 << 7   // CPHA = 0
              | 0 << 8;    // SCR = 0

    SSP0CPSR
        = 10;  // Clock prescaler (100 [MHz] / 10 = 10 [MHz]) // TODO: why 10?

    SSP0DMACR = 0b10;  // Enable DMA only for TX

    SSP0CR1 = 0b10;  // Enable SSP0

    ssp0_clear();  // The reset doesn't actually do anything, so we have to
                   // clear it manually
    // Note: to anyone working at NPX, I wish you to step on a Lego.
}

uint8_t ssp0_write(uint8_t data) {
    SSP0DR = data;
    while (SSP0SR & (1 << 4));
    data = SSP0DR;
    return data;
}

void ssp0_clear(void) {
    while (SSP0SR & (1 << 3)) {  // While Receive FIFO is not empty
        uint8_t data = SSP0DR;
    }
}
