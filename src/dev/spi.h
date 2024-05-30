#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

void SPI_INIT(void);
void SPI_write_cmd(uint8_t cmd);
void SPI_write_data(uint8_t data);

#endif
