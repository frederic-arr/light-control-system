#ifndef __ILI9341_H__
#define __ILI9341_H__

#include <stdint.h>

#define __ILI9341_WIDTH 240
#define __ILI9341_HEIGHT 320

#define __ILI9341_CMD_NOP 0x00
#define __ILI9341_CMD_CASET 0x2A
#define __ILI9341_CMD_PASET 0x2B
#define __ILI9341_CMD_RAMWR 0x2C
#define __ILI9341_CMD_RAMRD 0x2E
#define __ILI9341_CMD_VSCRSADD 0x37

#define LCD_CS_PIN  16
#define LCD_RS_PIN  30
#define LCD_CS_ENA() {FIO1CLR=(1<<LCD_CS_PIN);}
#define SET_LCD_CMD_MODE() FIO1CLR=(1<<30)
#define SET_LCD_DATA_MODE() FIO1SET=(1<<30)

void ili9341_init(void);

void ili9341_wr_cmd(uint8_t cmd);
void ili9341_wr_data(uint8_t cmd);

void ili9341_cmd_nop(void);
void ili9341_cmd_caset(uint16_t x1, uint16_t x2);
void ili9341_cmd_paset(uint16_t y1, uint16_t y2);
void ili9341_cmd_ramwr(void);
void ili9341_cmd_ramrd(void);
void ili9341_cmd_vscrsadd(uint16_t y);
void ili9341_data_color(uint8_t a, uint8_t b);
void ili9341_data_color16(uint16_t color);
void ili9341_data_rgb(uint8_t r, uint8_t g, uint8_t b);
void ili9341_zone_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ili9341_zone_set_rel(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height);
void ili9341_zone_reset(void);
void ili9341_bg_set(uint16_t bg);

#endif // __ILI9341_H__
