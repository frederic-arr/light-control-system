#ifndef __TOUCH_H__
#define __TOUCH_H__

#define TOUCH_WIDTH (240)
#define TOUCH_HEIGHT (320)

typedef struct {
    uint8_t nb_touch;
    
    uint16_t x1;
    uint16_t y1;

    uint16_t x2;
    uint16_t y2
} touch_t;

touch_t TOUCH_STATE(void);

#endif
