#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>

#define LCD_WIDTH (240)
#define LCD_HEIGHT (320)
#define LCD_CMD_NOP (0x00)
#define LCD_CMD_CASET (0x2A)
#define LCD_CMD_PASET (0x2B)
#define LCD_CMD_RAMWR (0x2C)
#define LCD_CMD_RAMRD (0x2E)
#define LCD_CMD_VSCRSADD (0x37)

void LCD_INIT(void);
void LCD_WR_CMD(uint8_t cmd);
uint8_t LCD_WR_DATA(uint8_t cmd);

void CMD_NOP(void);
void CMD_CASET(uint16_t x1, uint16_t x2);
void CMD_PASET(uint16_t y1, uint16_t y2);
void CMD_RAMWR(void);
void CMD_RAMRD(void);
void CMD_VSCRSADD(uint16_t y);
void CMD_COLORDATA(uint8_t a, uint8_t b);
void CMD_COLORDATA16(uint16_t color);
void CMD_RGBDATA16(uint8_t r, uint8_t g, uint8_t b);
void ZONE_SET(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ZONE_SET_REL(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height);
void ZONE_RESET(void);
void BG_SET_STATIC(uint16_t bg);

#endif // __LCD_H__
