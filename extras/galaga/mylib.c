#include <xinu.h>

typedef unsigned short u16;
// u16 *videoBuffer = (u16 *)0x6000000;
// #define scanlinecounter *(volatile u16 *)0x4000006
// extern void setPixel(int x, int y, u16 color);
void drawRect(int x, int y, int width, int height, u16 color);
void drawHollowRect(int x, int y, int width, int height, u16 color);
void drawImage3(int x, int y, int width, int height, const u16* image);
// void delay();
void waitForVBlank();

extern uint32 *galaga_buf;

uint32 rgb16_to_32(uint16 color)
{
        /* 1. Extract the red, green and blue values */

        // From rrrr rggg gggb bbbb, get r5, g6 and b5
        uint32 r = (color >> 11) & 0x1F;
        uint32 g = (color >> 5) & 0x3F;
        uint32 b = (color & 0x1F);

        /* 2. Convert them to r8, g8 and b8 (0-255 value), applying rule of three */
        r = (r * 255) / 31;
        g = (g * 255) / 63;
        b = (b * 255) / 31;

        /* 3. Construct 32-bit format, we get 0x00RRGGBB by shifting bits */
        uint32 color32 = 0x00ffffff & ((r << 16) | (g << 8) | b);
        return color32;
}

void gui_buf_init()
{
	galaga_buf = (uint32 *)getmem(240*160*4);
}

void gui_buf_free()
{
	freemem((char *)galaga_buf, 240*160*4);
}

void setPixel(int x, int y, u16 color) {
	if ((x > 240) || (y > 160))
		return;

        uint32 color32 = rgb16_to_32(color);
	galaga_buf[x + 240 * y] = color32;
	// galaga_buf[x + 240 * y] = color;
}

void drawRect(int x, int y, int width, int height, u16 color) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			setPixel(x + i, y + j, color);
		}
	}
}

void drawHollowRect(int x, int y, int width, int height, u16 color) {
	for (int i = 0; i < width; i++) {
		setPixel(x + i, y, color);
		setPixel(x + i, y + (height - 1), color);
	}
	for (int j = 0; j < height; j++) {
		setPixel(x, y + j, color);
		setPixel(x + (width - 1), y + j, color);
	}
}

void drawImage3(int x, int y, int width, int height, const u16* image) {
	int colorPos = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			u16 color = 0xffff & ((image[colorPos] << 8) | image[colorPos + 1]);
			setPixel(x + j, y + i, color);
			colorPos += 2;
		}
	}
}

void delay_galaga() {
	for (int a = 0; a < 10; a++) {
		a++;
	}
}

void waitForVBlank() {
	sleepms(20);
	// while(scanlinecounter > 160);
	// while(scanlinecounter < 160);
}
