#ifndef __GPDMA_H__
#define __GPDMA_H__

#include <stdbool.h>
#include <stdint.h>

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
#define INCREMENT 1
#define TERMINAL_INTERRUPT 0x80000000

#define DMA_CFG                                                    \
    ((BURST1 << 12) | (BURST1 << 15) | (HALFWORD16_TRANSFER << 18) \
     | TERMINAL_INTERRUPT)

void gpdma_configure(void);
void gpdma_transfer(uint16_t *data, uint32_t len, bool is_incr);

#endif  // __GPDMA_H__
