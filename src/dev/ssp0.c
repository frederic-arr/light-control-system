#include "ssp0.h"

#include "config.h"
#include "gpdma.h"

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

    gpdma_configure_channel(GPDMA_CHANNEL_0,
                            (gpdma_channel_configuration_t){
                                .enable = true,
                                .src_peripheral = 0,  // Memory
                                .dest_peripheral = GPDMA_PERIPHERAL_SSP0_TX,
                                .transfer_type = GPDMA_TRANSFER_TYPE_M2P,
                                .enable_error_interrupt = false,
                                .enable_terminal_count_interrupt = true,
                                .halt = false,
                            });

    ssp0_clear_rx(); // This is stupid
}

uint8_t ssp0_write(uint8_t data) {
    SSP0DR = data;
    while (SSP0SR & (1 << 4));
    data = SSP0DR;
    return data;
}

void ssp0_clear_rx(void) {
    while (SSP0SR & (1 << 3)) {  // While Receive FIFO is not empty
        uint8_t data = SSP0DR;
        printf("Read data: %02X\n", data);
    }
}

void ssp0_write_buffer(uint16_t *data, uint16_t len) {
    ssp0_clear_rx();
    uint8_t bs = 0xFFF;

    DMACIntErrClr = 1 << 0;
    DMACIntTCClear = 1 << 0;
    for (uint16_t i = 0; i < len; i += bs) {
        gpdma_configure_transfer(
            GPDMA_CHANNEL_0, data + i, &SSP0DR,
            (gpdma_transfert_configuration_t){
                .transfer_size = (len - i) > bs ? bs : (len - i),
                .source_burst_size = 0b000,
                .destination_burst_size = 0b000,
                .source_width = GPDMA_CHANNEL_CONTROL_WIDTH_HALF,
                .destination_width = GPDMA_CHANNEL_CONTROL_WIDTH_HALF,
                .enable_source_increment = true,
                .enable_destination_increment = false,
                .enable_terminal_count_interrupt = true,
            });

        ssp0_clear_rx();
        while (SSP0SR & (1 << 4));
        ssp0_clear_rx();
        gpdma_busy_wait(GPDMA_CHANNEL_0);
    }
}

// void gpdma_ssp0_init(void)
// {
//     DMACC0DestAddr = (uint32_t)&SSP0DR;
//     DMACC0LLI = 0;

//     // Clear DMA channel 0 configuration
//     DMACC0Control = 0;
//     DMACC0Config = 0;

//     // Clear DMA channel 0 interrupts
//     DMACIntTCClear = 1 << 0;
//     DMACIntErrClr = 1 << 0;
// }

// void gpdma_ssp0_transfer(uint8_t src, uint32_t *data, uint16_t len)
// {
//     DMACC0SrcAddr = data;

//     if (len == 0)
//     {
//         return;
//     }

//     if (len > 0xFFF)
//     {
//         len = 0xFFF;
//     }

//     DMACC0Control = len          // Transfer size
//                     | 1 << 26    // Source increment
//                     | (1 << 29)  // Bufferable
//                     | (1 << 30)  // Cacheable
//                     | (1 << 31); // Terminal count interrupt enable

//     DMACC0Config = 1                         // Enable the channel
//                    | ((src & 0b11111) << 1)  // Memory
//                    | ((0 & 0b11111) << 6)    // SSP0 Tx
//                    | ((0b001 & 0b111) << 11) // Memory to peripheral
//                    | 1 << 14                 // Interrupt error mask
//                    | 1 << 15;                // Terminal count interrupt mask
// }
