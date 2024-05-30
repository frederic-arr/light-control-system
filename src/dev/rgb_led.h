#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#include <stdint.h>

#define RGB_LED_NUM (2)

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

void RGB_LED_SET(color_t *colors);

#endif // __RGB_LED_H__
