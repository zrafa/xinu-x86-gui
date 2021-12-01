#include <xinu.h>
#include <vga.h>

/*
void pixel(uint32 x, uint32 y, uint32 color) {
                //seek(VGA, (y*vga.pitch+(x*(vga.bpp/8))));
                seek(VGA, (y*vga.width+x));
                write(VGA, &color, (vga.bpp/8));
}
*/

//void pixel(int x,int y, int color) {
 //   uint8 * screen = vga.addr;
  //  unsigned int where = x*(vga.bpp/8) + y*vga.pitch;
/*
    screen[where+1] = 0x1;
    screen[where] = 0x1;
    screen[where+2] = 0x1;
*/
   // screen[where] = color & 255;              // BLUE
    //screen[where + 1] = (color >> 8) & 255;   // GREEN
   // screen[where + 2] = (color >> 16) & 255;  // RED
//}


/*
void pixel(int x,int y, int color) {
	uint8 * graphics_buffer = (uint8 *)vga.addr;
    unsigned where = x + y*vga.width;
    graphics_buffer[where] = 0x77;
}
*/


void pixel2(int x,int y, int color2)
//void pixel(uint32 x, uint32 y, uint32 color)
{
    //unsigned char* location = (unsigned char*)0xA0000 + (VGA_WIDTH * y + x)*4;
    //unsigned char* location = (unsigned char*)0xA0000 + (y*vga.pitch+(x*(vga.bpp/8)));
int i;
	int color = 0xffffffff;
	for (i=0; i<10000; i++) {
    //unsigned char* location = (unsigned char*)vga.addr + (y*vga.pitch+(x*(vga.bpp/8)));
    unsigned char* location = (unsigned char*) (vga.addr + i);

//    unsigned char* location = vga.addr + (y*vga.pitch+(x*(vga.bpp/8)));

//  unsigned char* location = (unsigned char*)0xA0000 + (VGA_WIDTH * y + x);
   // //*location = VGA_COLOR;
  //  *location = 0xff;
   // *(location+1) = 0xff;
    //*(location+2) = 0xff;
    *location = color & 255;              // BLUE
    *(location+1) = (color >> 8) & 255;   // GREEN
    *(location+2) = (color >> 16) & 255;  // RED
} 
while(1);
/*
    *(location+4) = (color >> 8) & 255;   // GREEN
    *(location+5) = (color >> 16) & 255;  // RED
    *(location+6) = (color >> 16) & 255;  // RED
    *(location+7) = (color >> 16) & 255;  // RED
    *(location+8) = (color >> 16) & 255;  // RED
    *(location+9) = (color >> 16) & 255;  // RED
*/
}





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






//multiboot_info_t *mbi;

void pixel(unsigned x, unsigned y, uint32 color)
{
      //uint32 color;
      void *fb = (void *) (unsigned long) mbi->framebuffer_addr;
//          color = 0x0000ffff;
                uint32 *pixel
                  = fb + mbi->framebuffer_pitch * y + 4 * x;
                *pixel = color;
//	while(1);
}


