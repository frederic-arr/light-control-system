#include "ft6x06.h"

#include <stdint.h>

#include "../sys/i2c.h"

bool i2c_read(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t data_len) {
    uint8_t wr = I2C_master_write(addr << 1, &reg, 1);
    if (wr != 0) {
        return false;
    }

    uint8_t rd = I2C_master_read((addr << 1) | 1, data, data_len);
    if (rd != 0) {
        return false;
    }

    return true;
}

bool ft6x06_get_touch(ft6x06_touch_t *touch) {
    uint8_t data[14];
    bool rd = i2c_read(__FT6X06_ADDR, __FT6X06_REG_GEST_ID, data, 14);
    if (!rd) {
        return false;
    }

    touch->gesture_id = data[0];
    touch->nb_touch_points = data[1] & __FT6X06_TD_STATUS_TOUCH_POINTS_MASK;

    touch->p1.x = ((data[2] & __FT6X06_PN_XH_TOUCH_X_POS_MASK) << 8) | data[3];
    touch->p1.y = ((data[4] & __FT6X06_PN_YH_TOUCH_Y_POS_MASK) << 8) | data[5];
    touch->p1.event_flag = (data[2] & __FT6X06_PN_XH_EVENT_FLAG_MASK)
                           >> __FT6X06_PN_XH_EVENT_FLAG_OFFSET;
    touch->p1.touch_id = (data[4] & __FT6X06_PN_YH_TOUCH_ID_MASK)
                         >> __FT6X06_PN_YH_TOUCH_ID_OFFSET;
    touch->p1.weight = data[6];
    touch->p1.area = (data[7] & __FT6X06_PN_MISC_TOUCH_AREA_MASK)
                     >> __FT6X06_PN_MISC_TOUCH_AREA_OFFSET;

    touch->p2.x = ((data[8] & __FT6X06_PN_XH_TOUCH_X_POS_MASK) << 8) | data[9];
    touch->p2.y
        = ((data[10] & __FT6X06_PN_YH_TOUCH_Y_POS_MASK) << 8) | data[11];
    touch->p2.event_flag = (data[8] & __FT6X06_PN_XH_EVENT_FLAG_MASK)
                           >> __FT6X06_PN_XH_EVENT_FLAG_OFFSET;
    touch->p2.touch_id = (data[10] & __FT6X06_PN_YH_TOUCH_ID_MASK)
                         >> __FT6X06_PN_YH_TOUCH_ID_OFFSET;
    touch->p2.weight = data[12];
    touch->p2.area = (data[13] & __FT6X06_PN_MISC_TOUCH_AREA_MASK)
                     >> __FT6X06_PN_MISC_TOUCH_AREA_OFFSET;

    return true;
}
