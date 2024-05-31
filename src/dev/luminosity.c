#include <stdint.h>
#include <stdbool.h>
#include "touch.h"
#include "../sys/i2c.h"

#define LUM_ADDR (0x39)
#define EN_ALS (1 << 1)
#define EN_WAIT (1 << 3)
#define EN_PON (1 << 0)

#define REG_EN (0x80)
#define REG_STATUS (0x93)
#define REG_CDATA (0x94)
#define REG_CTRL1 (0x8F)
#define REG_ATIME (0x81)

void LUMINOSITY_INIT(void) {
    uint8_t set_als_gain[2] = {REG_CTRL1, 3};
    uint8_t wr2 = I2C_master_write(0x39 << 1, &set_als_gain, 2);

    uint8_t set_als_time[2] = {REG_ATIME, 192};
    uint8_t wr3 = I2C_master_write(0x39 << 1, &set_als_time, 2);

    uint8_t enable_als[2] = {REG_EN, EN_PON | EN_ALS | EN_WAIT};
    uint8_t wr = I2C_master_write(0x39 << 1, &enable_als, 2);
}

bool LUMINOSITY_IS_DARK(void) {
    // Read STATUS
    uint8_t status;

    while ((status & 1) != 1) {
    	uint8_t reg = REG_STATUS;
    	uint8_t wr1 = I2C_master_write(LUM_ADDR << 1, &reg, 1);
    	uint8_t rd1 = I2C_master_read((LUM_ADDR << 1) | 1, &status, 1);
    }

    // Read CDATA
    uint8_t data[2];
    uint8_t reg2 = REG_CDATA;
    uint8_t wr = I2C_master_write(LUM_ADDR << 1, &reg2, 1);
    uint8_t rd = I2C_master_read((LUM_ADDR << 1) | 1, data, 2);
    uint16_t cdata = (data[1] << 8) | data[0];
    return cdata < 1000;
}
