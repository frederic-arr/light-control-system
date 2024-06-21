#include "sprite.h"

#include "ili9341.h"

uint16_t darken(uint16_t color, float factor) {
    float r = (float)((color >> 11) & 0x1F);
    float g = (float)((color >> 5) & 0x3F);
    float b = (float)(color & 0x1F);

    r *= factor;
    g *= factor;
    b *= factor;
    return (((uint16_t)r) << 11) | (((uint16_t)g) << 5) | ((uint16_t)b);
}

void DRAW_SPRITE(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                 uint8_t *sprite, bool is_dark) {
    ili9341_cmd_nop();
    ili9341_zone_set_rel(x, y, width, height);

    ili9341_cmd_ramwr();
    for (int i = 0; i < width * height; i++) {
        uint16_t color = (sprite[2 * i + 1] << 8) | (sprite[2 * i]);
        color = darken(color, is_dark ? 0.75 : 1.0);
        ili9341_data_color16(color);
    }
}
