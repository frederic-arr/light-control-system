#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <stdint.h>
#include <stdbool.h>

void DRAW_SPRITE(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *sprite, bool is_dark);

#endif // __SPRITE_H__