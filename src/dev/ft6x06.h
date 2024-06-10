#ifndef __FT6X06_H__
#define __FT6X06_H__

#include <stdbool.h>
#include <stdint.h>

// https://www.displayfuture.com/Display/datasheet/controller/FT6206.pdf

#define __FT6X06_ADDR 0x38
#define __FT6X06_WIDTH 320
#define __FT6X06_HEIGHT 240

#define __FT6X06_REG_DEV_MODE 0x00
#define __FT6X06_REG_GEST_ID 0x01
#define __FT6X06_REG_TD_STATUS 0x02

#define __FT6X06_REG_P1_XH 0x03
#define __FT6X06_REG_P1_XL 0x04
#define __FT6X06_REG_P1_YH 0x05
#define __FT6X06_REG_P1_YL 0x06
#define __FT6X06_REG_P1_WEIGHT 0x07
#define __FT6X06_REG_P1_MISC 0x08

#define __FT6X06_REG_P2_XH 0x09
#define __FT6X06_REG_P2_XL 0x0A
#define __FT6X06_REG_P2_YH 0x0B
#define __FT6X06_REG_P2_YL 0x0C
#define __FT6X06_REG_P2_WEIGHT 0x0D
#define __FT6X06_REG_P2_MISC 0x0E

#define __FT6X06_REG_TH_GROUP 0x80
#define __FT6X06_REG_TH_DIFF 0x85
#define __FT6X06_REG_CTRL 0x86
#define __FT6X06_REG_TIMEENTERMONITOR 0x87
#define __FT6X06_REG_PERIODACTIVE 0x88
#define __FT6X06_REG_PERIODMONITOR 0x89

#define __FT6X06_REG_RADIAN_VALUE 0x91
#define __FT6X06_REG_OFFSET_LEFT_RIGHT 0x92
#define __FT6X06_REG_OFFSET_UP_DOWN 0x93
#define __FT6X06_REG_DISTANCE_LEFT_RIGHT 0x94
#define __FT6X06_REG_DISTANCE_UP_DOWN 0x95
#define __FT6X06_REG_DISTANCE_ZOOM 0x96
#define __FT6X06_REG_LIB_VER_H 0xA1
#define __FT6X06_REG_LIB_VER_L 0xA2
#define __FT6X06_REG_CIPHER 0xA3
#define __FT6X06_REG_G_MODE 0xA4
#define __FT6X06_REG_PWR_MODE 0xA5
#define __FT6X06_REG_FIRMID 0xA6
#define __FT6X06_REG_FOCALTECH_ID 0xA8
#define __FT6X06_REG_RELEASE_CODE_ID 0xAF
#define __FT6X06_REG_STATE 0xBC

#define __FT6X06_DEVICE_MODE_WORKING 0x00
#define __FT6X06_DEVICE_MODE_FACTORY 0x04

#define __FT6X06_GEST_ID_NO_GESTURE 0x00
#define __FT6X06_GEST_ID_MOVE_UP 0x10
#define __FT6X06_GEST_ID_MOVE_RIGHT 0x14
#define __FT6X06_GEST_ID_MOVE_DOWN 0x18
#define __FT6X06_GEST_ID_MOVE_LEFT 0x1C
#define __FT6X06_GEST_ID_ZOOM_IN 0x48
#define __FT6X06_GEST_ID_ZOOM_OUT 0x49

#define __FT6X06_TD_STATUS_TOUCH_POINTS_MASK 0x0F

#define __FT6X06_PN_XH_EVENT_FLAG_MASK 0xC0
#define __FT6X06_PN_XH_EVENT_FLAG_OFFSET 6
#define __FT6X06_PN_XH_TOUCH_X_POS_MASK 0x0F

#define __FT6X06_PN_YH_TOUCH_ID_MASK 0xF0
#define __FT6X06_PN_YH_TOUCH_ID_OFFSET 4
#define __FT6X06_PN_YH_TOUCH_Y_POS_MASK 0x0F

#define __FT6X06_PN_MISC_TOUCH_AREA_MASK 0xF0
#define __FT6X06_PN_MISC_TOUCH_AREA_OFFSET 4

#define __FT6X06_EVENT_FLAG_PRESS_DOWN 0x00
#define __FT6X06_EVENT_FLAG_LIFT_UP 0x01
#define __FT6X06_EVENT_FLAG_CONTACT 0x02
#define __FT6X06_EVENT_FLAG_NO_EVENT 0x03

typedef enum __ft6x06_gesture_id_t
{
    GESTURE_NO_GESTURE = __FT6X06_GEST_ID_NO_GESTURE,
    GESTURE_MOVE_UP = __FT6X06_GEST_ID_MOVE_UP,
    GESTURE_MOVE_RIGHT = __FT6X06_GEST_ID_MOVE_RIGHT,
    GESTURE_MOVE_DOWN = __FT6X06_GEST_ID_MOVE_DOWN,
    GESTURE_MOVE_LEFT = __FT6X06_GEST_ID_MOVE_LEFT,
    GESTURE_ZOOM_IN = __FT6X06_GEST_ID_ZOOM_IN,
    GESTURE_ZOOM_OUT = __FT6X06_GEST_ID_ZOOM_OUT,
} ft6x06_gesture_id_t;

typedef enum __ft6x06_event_flag_t
{
    EVENT_NO_TOUCH = __FT6X06_EVENT_FLAG_NO_EVENT,
    EVENT_PRESS_DOWN = __FT6X06_EVENT_FLAG_PRESS_DOWN,
    EVENT_LIFT_UP = __FT6X06_EVENT_FLAG_LIFT_UP,
    EVENT_CONTACT = __FT6X06_EVENT_FLAG_CONTACT,
} ft6x06_event_flag_t;

typedef struct __ft6x06_touch_point_t
{
    uint16_t x;
    uint16_t y;
    ft6x06_event_flag_t event_flag;
    uint8_t touch_id;
    uint8_t weight;
    uint8_t area;
} ft6x06_touch_point_t;

typedef struct __ft6x06_touch_t
{
    uint8_t nb_touch_points;
    ft6x06_gesture_id_t gesture_id;
    ft6x06_touch_point_t p1;
    ft6x06_touch_point_t p2;
} ft6x06_touch_t;

bool ft6x06_get_touch(ft6x06_touch_t *touch);

#endif // __FT6X06_H__
