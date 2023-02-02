#include <xinu.h>
#include <vga.h>



typedef struct {
	unsigned int height;
	unsigned int width;
	unsigned int size;
	unsigned int pitch;
	unsigned char bpp;
	unsigned char ypp;
	unsigned char* buffer;
	unsigned char* device;
} vgaframebuffer_t;

vgaframebuffer_t vga2;

uint32 rgb16_to_rgb32(u16 a)
{
/* 1. Extract the red, green and blue values */

/* from rrrr rggg gggb bbbb */
uint32 r = (a & 0xF800) >>11;
uint32 g = (a & 0x07E0) >>5;
uint32 b = (a & 0x001F);

/* 2. Convert them to 0-255 range:
There is more than one way. You can just shift them left:
to 00000000 rrrrr000 gggggg00 bbbbb000
r <<= 3;
g <<= 2;
b <<= 3;
But that means your image will be slightly dark and
off-colour as white 0xFFFF will convert to F8,FC,F8
So instead you can scale by multiply and divide: */
r = r * 255 / 31;
g = g * 255 / 63;
b = b * 255 / 31;
/* This ensures 31/31 converts to 255/255 */

/* 3. Construct your 32-bit format (this is 0RGB): */
//return (r << 16) | (g << 8) | b;
return (b << 16) | (g << 8) | r;


/* Or for BGR0: */
//return (r << 8) | (g << 16) | (b << 24);
}



void setPixel(int x, int y, u16 color) {
	if ((x >= 240) || (y >= 160))
		return;

      void *fb = (void *) (unsigned long) mbi->framebuffer_addr;
                uint32 *pixel
                  = fb + mbi->framebuffer_pitch * y + 4 * x;
//	 *pixel = color;
		*pixel = rgb16_to_rgb32(color);

}

void pixeld(int x, int y, int color) {
	if (x > vga2.width || y > vga2.height) return;
    int b=color>>16,g=color>>8&255,r=color&255;
	vga2.device[(y*vga2.width+x)*vga2.ypp] = r;
	vga2.device[(y*vga2.width+x)*vga2.ypp+1] = g;
	vga2.device[(y*vga2.width+x)*vga2.ypp+2] = b;
}

void linea()
{
	pixeld(50, 50, 0xAFAFAFAF);
	while(1);
}




void pixel(unsigned x, unsigned y, uint32 color)
{
      void *fb = (void *) (unsigned long) mbi->framebuffer_addr;
                uint32 *pixel
                  = fb + mbi->framebuffer_pitch * y + 4 * x;
                *pixel = color;
}


void paint_screen(){

        uint32 color = 0x00ffff00;
        int i,j,x,y;
        open(VGA, NULL, 0);
        for (y=0; y<VGA_HEIGHT; y++) {
        	for (x=0; x<VGA_WIDTH; x++) {
                	pixel(x, y, color);
        	}
                sleepms(1);
        }
        close(VGA);

}




