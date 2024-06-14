#include "ili9341.h"
#include "config.h"
#include "clk.h"

void ili9341_cmd_nop(void)
{
	ili9341_wr_cmd(__ILI9341_CMD_RAMRD);
}

void ili9341_cmd_caset(uint16_t x1, uint16_t x2)
{
	ili9341_wr_cmd(__ILI9341_CMD_CASET);
	ili9341_wr_data(x1 >> 8);
	ili9341_wr_data(x1 & 0xFF);
	ili9341_wr_data(x2 >> 8);
	ili9341_wr_data(x2 & 0xFF);
}

void ili9341_cmd_paset(uint16_t y1, uint16_t y2)
{
	ili9341_wr_cmd(__ILI9341_CMD_PASET);
	ili9341_wr_data(y1 >> 8);
	ili9341_wr_data(y1 & 0xFF);
	ili9341_wr_data(y2 >> 8);
	ili9341_wr_data(y2 & 0xFF);
}

void ili9341_cmd_ramwr(void)
{
	ili9341_wr_cmd(__ILI9341_CMD_RAMWR);
}

void ili9341_cmd_ramrd(void)
{
	ili9341_wr_cmd(__ILI9341_CMD_RAMRD);
}

void ili9341_cmd_vscrsadd(uint16_t y)
{
	ili9341_wr_cmd(__ILI9341_CMD_VSCRSADD);
	ili9341_wr_data(y >> 8);
	ili9341_wr_data(y & 0xFF);
}

void ili9341_data_color(uint8_t a, uint8_t b)
{
	ili9341_wr_data(a);
	ili9341_wr_data(b);
}

void ili9341_data_color16(uint16_t color)
{
	ili9341_wr_data(color >> 8);
	ili9341_wr_data(color & 0xFF);
}

void ili9341_data_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t color = (r << 11) | (g << 5) | b;
	ili9341_data_color16(color);
}

void ili9341_zone_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	ili9341_cmd_caset(x1, x2);
	ili9341_cmd_paset(y1, y2);
}

void ili9341_zone_set_rel(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height)
{
	ili9341_zone_set(x1, y1, x1 + width - 1, y1 + height - 1);
}

void ZONE_SET_REL_C(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height)
{
	uint16_t half_w = width / 2;
	uint16_t half_h = height / 2;
	ili9341_zone_set(x1 - half_w, y1 - half_h, x1 + half_w - 1, y1 + half_h - 1);
}

void ili9341_zone_reset(void)
{
	ili9341_zone_set(0, 0, 239, 319);
}

void ili9341_wr_cmd(uint8_t cmd)
{
	FIO0CLR = (1 << 16);
	FIO1CLR = (1 << 30);
	SSP0DR = cmd;
	while (SSP0SR & (1 << 4))
	{
		;
	};
	uint8_t data = SSP0DR;
	FIO0SET = (1 << 16);
}

uint8_t ili9341_wr_data(uint8_t cmd)
{
	FIO0CLR = (1 << 16);
	FIO1SET = (1 << 30);
	SSP0DR = cmd;
	while (SSP0SR & (1 << 4))
	{
		;
	};
	uint8_t data = SSP0DR;
	FIO0SET = (1 << 16);
	return data;
}

void ili9341_bg_set(uint16_t bg)
{
	ili9341_zone_reset();

	ili9341_cmd_ramwr();
	for (int i = 0; i < __ILI9341_WIDTH * __ILI9341_HEIGHT; i++)
	{
		ili9341_data_color16(bg);
	}
}

void ili9341_init(void)
{
	ili9341_wr_cmd(0x01); // software reset
	delay(5);

	ili9341_wr_cmd(0x11);
	delay(120);

	ili9341_wr_cmd(0xCF);
	ili9341_wr_data(0x00);
	ili9341_wr_data(0x83);
	ili9341_wr_data(0X30);

	ili9341_wr_cmd(0xED);
	ili9341_wr_data(0x64);
	ili9341_wr_data(0x03);
	ili9341_wr_data(0X12);
	ili9341_wr_data(0X81);

	ili9341_wr_cmd(0xE8);
	ili9341_wr_data(0x85);
	ili9341_wr_data(0x01);
	ili9341_wr_data(0x79);

	ili9341_wr_cmd(0xCB);
	ili9341_wr_data(0x39);
	ili9341_wr_data(0x2C);
	ili9341_wr_data(0x00);
	ili9341_wr_data(0x34);
	ili9341_wr_data(0x02);

	ili9341_wr_cmd(0xF7);
	ili9341_wr_data(0x20);

	ili9341_wr_cmd(0xEA);
	ili9341_wr_data(0x00);
	ili9341_wr_data(0x00);

	ili9341_wr_cmd(0xC1);  // Power control
	ili9341_wr_data(0x11); // SAP[2:0];BT[3:0]

	ili9341_wr_cmd(0xC5); // VCM control 1
	ili9341_wr_data(0x34);
	ili9341_wr_data(0x3D);

	ili9341_wr_cmd(0xC7); // VCM control 2
	ili9341_wr_data(0xC0);

	ili9341_wr_cmd(0x36); // Memory Access Control
	ili9341_wr_data(0x08);

	ili9341_wr_cmd(0x3A);  // Pixel format
	ili9341_wr_data(0x55); // 16bit

	ili9341_wr_cmd(0xB1); // Frame rate
	ili9341_wr_data(0x00);
	ili9341_wr_data(0x1D); // 65Hz

	ili9341_wr_cmd(0xB6); // Display Function Control
	ili9341_wr_data(0x0A);
	ili9341_wr_data(0xA2);
	ili9341_wr_data(0x27);
	ili9341_wr_data(0x00);

	ili9341_wr_cmd(0xb7); // Entry mode
	ili9341_wr_data(0x07);

	ili9341_wr_cmd(0xF2); // 3Gamma Function Disable
	ili9341_wr_data(0x08);

	ili9341_wr_cmd(0x26); // Gamma curve selected
	ili9341_wr_data(0x01);

	ili9341_wr_cmd(0xE0); // positive gamma correction
	ili9341_wr_data(0x1f);
	ili9341_wr_data(0x1a);
	ili9341_wr_data(0x18);
	ili9341_wr_data(0x0a);
	ili9341_wr_data(0x0f);
	ili9341_wr_data(0x06);
	ili9341_wr_data(0x45);
	ili9341_wr_data(0x87);
	ili9341_wr_data(0x32);
	ili9341_wr_data(0x0a);
	ili9341_wr_data(0x07);
	ili9341_wr_data(0x02);
	ili9341_wr_data(0x07);
	ili9341_wr_data(0x05);
	ili9341_wr_data(0x00);

	ili9341_wr_cmd(0xE1); // negamma correction
	ili9341_wr_data(0x00);
	ili9341_wr_data(0x25);
	ili9341_wr_data(0x27);
	ili9341_wr_data(0x05);
	ili9341_wr_data(0x10);
	ili9341_wr_data(0x09);
	ili9341_wr_data(0x3a);
	ili9341_wr_data(0x78);
	ili9341_wr_data(0x4d);
	ili9341_wr_data(0x05);
	ili9341_wr_data(0x18);
	ili9341_wr_data(0x0d);
	ili9341_wr_data(0x38);
	ili9341_wr_data(0x3a);
	ili9341_wr_data(0x1f);

	ili9341_wr_cmd(0x11); // Exit Sleep
	delay(120);
	ili9341_wr_cmd(0x29); // Display on
	delay(50);
}
