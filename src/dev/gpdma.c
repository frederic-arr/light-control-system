#include "gpdma.h"

#include "config.h"
#include "ssp0.h"

#define DMA_CTRL ((0x0 << 12) | (0x0 << 15) \
                | (0x1 << 18) | \
                0x80000000)


void gpdma_configure(gpdma_configuration_t config) {
    PCONP |= 1 << 29;  // Power on GPDMA

    DMACIntTCClear = 0xFF;
    DMACIntErrClr = 0xFF;
    DMACConfig = config.enable | (config.is_big_endian << 1);
}

void gpdma_configure_channel(gpdma_channel_t channel,
                             gpdma_channel_configuration_t config) {
    uint32_t cfg = config.enable | (config.src_peripheral << 1)
                   | (config.dest_peripheral << 6)
                   | (config.transfer_type << 11)
                   | (config.enable_error_interrupt << 14)
                   | (config.enable_terminal_count_interrupt << 15)
                   | (config.halt << 18);

    switch (channel) {
        case GPDMA_CHANNEL_0:
            DMACC0Config = cfg;
            break;
            // TODO: Implement other channels
    }
}

void gpdma_configure_transfer(gpdma_channel_t channel, uint32_t *src,
                              uint32_t *dst,
                              gpdma_transfert_configuration_t config) {
    uint32_t cfg = (config.transfer_size & 0xFFF) | (config.source_burst_size << 12)
                   | (config.destination_burst_size << 15)
                   | (config.source_width << 18)
                   | (config.destination_width << 21) | (config._reserved << 24)
                   | (config.enable_source_increment << 26)
                   | (config.enable_destination_increment << 27)
                   | (config.enable_terminal_count_interrupt << 31);

    cfg = (config.transfer_size & 0xFFF) | DMA_CTRL | (1 << 26);
    switch (channel) {
        case GPDMA_CHANNEL_0:
            DMACC0SrcAddr = (uint32_t)src;
            DMACC0DestAddr = (uint32_t)dst;
            DMACC0Control = cfg;
            break;
    }
}

void gpdma_busy_wait(gpdma_channel_t channel) {
    while (!(DMACIntTCStat & (1 << GPDMA_CHANNEL_0)));
    // DMACIntTCClear = 1 << GPDMA_CHANNEL_0;
}
