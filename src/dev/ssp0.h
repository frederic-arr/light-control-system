#ifndef __SSP0_H__
#define __SSP0_H__

#include <stdint.h>

#define SSP0CR0 (*(volatile uint32_t *)0x40088000)
#define SSP0CR1 (*(volatile uint32_t *)0x40088004)
#define SSP0DR (*(volatile uint32_t *)0x40088008)
#define SSP0SR (*(volatile uint32_t *)0x4008800C)
#define SSP0CPSR (*(volatile uint32_t *)0x40088010)
#define SSP0IMSC (*(volatile uint32_t *)0x40088014)
#define SSP0RIS (*(volatile uint32_t *)0x40088018)
#define SSP0MIS (*(volatile uint32_t *)0x4008801C)
#define SSP0ICR (*(volatile uint32_t *)0x40088020)
#define SSP0DMACR (*(volatile uint32_t *)0x40088024)

void ssp0_init(void);
uint8_t ssp0_write(uint8_t data);
void ssp0_clear(void);

#endif  // __SSP_H__
