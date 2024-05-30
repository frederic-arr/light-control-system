#include "lcd.h"
#include "config.h"
#include "clk.h"

void CMD_NOP(void) {
	LCD_WR_CMD(LCD_CMD_RAMRD);
}

void CMD_CASET(uint16_t x1, uint16_t x2) {
    LCD_WR_CMD(LCD_CMD_CASET);
    LCD_WR_DATA(x1 >> 8);
    LCD_WR_DATA(x1 & 0xFF);
    LCD_WR_DATA(x2 >> 8);
    LCD_WR_DATA(x2 & 0xFF);
}

void CMD_PASET(uint16_t y1, uint16_t y2) {
    LCD_WR_CMD(LCD_CMD_PASET);
    LCD_WR_DATA(y1 >> 8);
    LCD_WR_DATA(y1 & 0xFF);
    LCD_WR_DATA(y2 >> 8);
    LCD_WR_DATA(y2 & 0xFF);
}

void CMD_RAMWR(void)
{
	LCD_WR_CMD(LCD_CMD_RAMWR);
}

void CMD_RAMRD(void) {
	LCD_WR_CMD(LCD_CMD_RAMRD);
}

void CMD_VSCRSADD(uint16_t y) {
	LCD_WR_CMD(LCD_CMD_VSCRSADD);
	LCD_WR_DATA(y >> 8);
	LCD_WR_DATA(y & 0xFF);
}

void CMD_COLORDATA16(uint16_t color)
{
	LCD_WR_DATA(color >> 8);
	LCD_WR_DATA(color & 0xFF);
}

void CMD_RGBDATA16(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t color = (r << 11) | (g << 5) | b;
	CMD_COLORDATA16(color);
}

void ZONE_SET(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    CMD_CASET(x1, x2);
    CMD_PASET(y1, y2);
}

void ZONE_SET_REL(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height) {
    ZONE_SET(x1, y1, x1 + width - 1, y1 + height - 1);
}

void ZONE_SET_REL_C(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height) {
	uint16_t half_w = width / 2;
	uint16_t half_h = height / 2;
	ZONE_SET(x1 - half_w, y1 - half_h, x1 + half_w - 1, y1 + half_h - 1);
}

void ZONE_RESET(void) {
    ZONE_SET(0, 0, 239, 319);
}

void LCD_WR_CMD(uint8_t cmd)
{
	FIO0CLR = (1 << 16);
	FIO1CLR = (1 << 30);
	S0SPDR = cmd;
	while (((S0SPSR >> 7) & 1) != 1) {;};
	uint8_t data = S0SPDR;
	FIO0SET = (1 << 16);
}

uint8_t LCD_WR_DATA(uint8_t cmd)
{
	FIO0CLR = (1 << 16);
	FIO1SET = (1 << 30);
	S0SPDR = cmd;
	while (((S0SPSR >> 7) & 1) != 1) {;};
	uint8_t data = S0SPDR;
	FIO0SET = (1 << 16);
	return data;
}

void BG_SET_STATIC(uint16_t bg) {
	ZONE_RESET();

	CMD_RAMWR();
	for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
        CMD_COLORDATA16(bg);
    }
}

void LCD_INIT(void)
{
	LCD_WR_CMD(0x01); //software reset
	delay(5);

	LCD_WR_CMD(0x11);
	delay(120);

 	LCD_WR_CMD(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x83);
	LCD_WR_DATA(0X30);

 	LCD_WR_CMD(0xED);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0X12);
	LCD_WR_DATA(0X81);

 	LCD_WR_CMD(0xE8);
	LCD_WR_DATA(0x85);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x79);

 	LCD_WR_CMD(0xCB);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x02);

 	LCD_WR_CMD(0xF7);
	LCD_WR_DATA(0x20);

 	LCD_WR_CMD(0xEA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);


 	LCD_WR_CMD(0xC1);    //Power control
	LCD_WR_DATA(0x11);   //SAP[2:0];BT[3:0]

 	LCD_WR_CMD(0xC5);    //VCM control 1
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x3D);

 	LCD_WR_CMD(0xC7);    //VCM control 2
	LCD_WR_DATA(0xC0);

 	LCD_WR_CMD(0x36);    // Memory Access Control
	LCD_WR_DATA(0x08);

 	LCD_WR_CMD(0x3A);   	 // Pixel format
	LCD_WR_DATA(0x55);  //16bit

 	LCD_WR_CMD(0xB1);   	   // Frame rate
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1D);  //65Hz

 	LCD_WR_CMD(0xB6);    // Display Function Control
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0xA2);
	LCD_WR_DATA(0x27);
	LCD_WR_DATA(0x00);

	LCD_WR_CMD(0xb7); //Entry mode
	LCD_WR_DATA(0x07);


 	LCD_WR_CMD(0xF2);    // 3Gamma Function Disable
	LCD_WR_DATA(0x08);

 	LCD_WR_CMD(0x26);    //Gamma curve selected
	LCD_WR_DATA(0x01);


	LCD_WR_CMD(0xE0); //positive gamma correction
	LCD_WR_DATA(0x1f);
	LCD_WR_DATA(0x1a);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x0a);
	LCD_WR_DATA(0x0f);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x87);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x0a);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x00);

	LCD_WR_CMD(0xE1); //negamma correction
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x25);
	LCD_WR_DATA(0x27);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x3a);
	LCD_WR_DATA(0x78);
	LCD_WR_DATA(0x4d);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x0d);
	LCD_WR_DATA(0x38);
	LCD_WR_DATA(0x3a);
	LCD_WR_DATA(0x1f);

 	LCD_WR_CMD(0x11);    //Exit Sleep
 	delay(120);
 	LCD_WR_CMD(0x29);    //Display on
 	delay(50);
}
