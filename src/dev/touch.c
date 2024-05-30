#include <stdint.h>
#include "touch.h"
#include "../sys/i2c.h"

touch_t TOUCH_STATE(void) {
    uint8_t data[15];
    uint8_t reg = 0x00;
    uint8_t wr = I2C_master_write(0x38 << 1, &reg, 1);
    uint8_t rd = I2C_master_read((0x38 << 1) | 1, data, 14);

    uint8_t nb_touch = data[0x02] & 0b11;
    uint16_t x1 = (data[0x03] << 8) | data[0x04];
    uint16_t y1 = (data[0x05] << 8) | data[0x06];
    uint16_t x2 = (data[0x09] << 8) | data[0x0A];
    uint16_t y2 = (data[0x0B] << 8) | data[0x0C];

    touch_t touch = {
        .nb_touch = nb_touch,
        .x1 = TOUCH_WIDTH - x1,
        .y1 = TOUCH_HEIGHT - y1,
        .x2 = TOUCH_WIDTH - x2,
        .y2 = TOUCH_HEIGHT - y2
    };
    return touch;
}
