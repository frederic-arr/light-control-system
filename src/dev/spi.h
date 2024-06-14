#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

void spi_init(void);
void spi_write(uint8_t data);
void spi_read(uint8_t *data);

#endif
