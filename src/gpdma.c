#include "gpdma.h"
#include "sys/utils.h"

void gpdma_init(void)
{
    PCONP |= 1 << 29; // PCGPDMA
    DMACConfig = 1 << 0; // Enable DMA controller
}

void gpdma_transfer(
    uint8_t src_dev,
    uint32_t src_addr,
    uint8_t dest_dev,
    uint32_t dest_addr,
    uint32_t size,
    uint8_t type
) {
    // Clear previous interrupts on channel 0
    DMACIntTCClear = 1 << 0;
    DMACIntErrClr = 1 << 0;

    // Set the source address
    DMACC0SrcAddr = src_addr;

    // Set the destination address
    DMACC0DestAddr = dest_addr;

    // Set LLI to 0
    DMACC0LLI = 0;

    // Set the control register
    DMACC0Control = (size & 0x0FFF)
                  | (1 << 26) // Source increment
                  | (1 << 31);

    // Enable the channel
    DMACC0Config = ((src_dev & 0b11111) << 1)
                 | ((dest_dev & 0b11111) << 6)
                 | ((type & 0b111) << 11);

    DMACC0Config |= 1 << 0; // Enable the channel
}

void gpdma_spi_transfer(uint32_t src_addr, uint32_t size) {
    gpdma_transfer(
        0, // Memory (?)
        src_addr,
        1, // SSP0 Rx
        0x40088008, // SSP0DR
        size,
        0b001 // Memory to peripheral
    );
}