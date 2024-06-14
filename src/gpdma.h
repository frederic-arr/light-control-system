#ifndef __GPDMA_H__
#define __GPDMA_H__

#include <stdint.h>

// General
#define DMACIntStat (*((volatile uint32_t *)0x50004000))
#define DMACIntTCStat (*((volatile uint32_t *)0x50004004))
#define DMACIntTCClear (*((volatile uint32_t *)0x50004008))
#define DMACIntErrStat (*((volatile uint32_t *)0x5000400C))
#define DMACIntErrClr (*((volatile uint32_t *)0x50004010))
#define DMACRawIntTCStat (*((volatile uint32_t *)0x50004014))
#define DMACRawIntErrStat (*((volatile uint32_t *)0x50004018))
#define DMACEnbldChns (*((volatile uint32_t *)0x5000401C))
#define DMACSoftBReq (*((volatile uint32_t *)0x50004020))
#define DMACSoftSReq (*((volatile uint32_t *)0x50004024))
#define DMACSoftLBReq (*((volatile uint32_t *)0x50004028))
#define DMACSoftLSReq (*((volatile uint32_t *)0x5000402C))
#define DMACConfig (*((volatile uint32_t *)0x50004030))
#define DMACSync (*((volatile uint32_t *)0x50004034))
#define DMAREQSEL (*((volatile uint32_t *)0x400FC1C4))

// Channel 0
#define DMACC0SrcAddr (*((volatile uint32_t *)0x50004100))
#define DMACC0DestAddr (*((volatile uint32_t *)0x50004104))
#define DMACC0LLI (*((volatile uint32_t *)0x50004108))
#define DMACC0Control (*((volatile uint32_t *)0x5000410C))
#define DMACC0Config (*((volatile uint32_t *)0x50004110))

void gpdma_init(void);
void gpdma_transfer(
    uint8_t src_dev,
    uint32_t src_addr,
    uint8_t dest_dev,
    uint32_t dest_addr,
    uint32_t size,
    uint8_t type
);
void gpdma_spi_transfer(uint32_t src_addr, uint32_t size);

#endif // __GPDMA_H__
