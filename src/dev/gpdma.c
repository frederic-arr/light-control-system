#include "gpdma.h"

#include "config.h"
#include "ssp0.h"

void gpdma_configure(void) {
    PCONP |= (1 << 29);

    DMACIntTCClear = 0x01;
    DMACIntErrClr = 0x01;

    DMACC0DestAddr = (uint32_t) & (SSP0DR);

    ISER0 |= (1 << 27);

    DMACC0LLI = 0;
    DMACConfig = 0x1;
}

void gpdma_transfer(uint16_t *data, uint32_t len, bool is_incr) {
    DMACC0SrcAddr = (uint32_t)data;
    DMACC0Control = (len & 0xFFF) | DMA_CFG | (is_incr << 26);
    DMACC0Config = 0x8801;
}
