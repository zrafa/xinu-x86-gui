/*  main.c  - main */

#include <xinu.h>



#include <stdio.h>
#include <stdlib.h>

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define NUM_COLORS          256       /* number of colors in mode 0x13 */

#define sgn(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a
                                         number */
typedef unsigned char  byte;
typedef unsigned short word;

// volatile byte *VGA = (byte *)0xA0000;          /* this points to video memory. */
//uint16 *VGA = (uint16 *)0xA0000;          /* this points to video memory. */
//word *my_clock = (word *)0x046C;      /* this points to the 18.2hz system
           //                              clock. */


extern unsigned char * vga;


void linea2()
{

void *fb = (void *) (uint64) mbi->framebuffer_addr;

          uint32 color = 0x0000ffff;
	int i,j=0;
      for (i = mbi->framebuffer_width; i > 0; i--){


                uint32 *pixel = fb + mbi->framebuffer_pitch * j + 3 * i;
               *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
		j++;

        }



}

//extern void pixel(uint32 x, uint32 y, uint32 color);
//void pixel(int x, int y, uint32 color) {
////	*(vga+(((y*1024)+x) *4)) = color;
////	*(vga+(((y*1024)+x) *4)+1) = (color >> 8);
////	*(vga+(((y*1024)+x) *4)+2) = (color >> 16);
//		seek(VGA, ((y*VGA_WIDTH+x) *1));
//		write(VGA, &color, 1);
//}

void paint2(){

	//int color = 0x00ffff00;
	uint32 color = 0x00ffff00;
	int i,j,x,y;
	open(VGA, NULL, 0);
	for (y=0; y<VGA_HEIGHT; y++) {
	for (x=0; x<VGA_WIDTH; x++) {
		pixel(x, y, color);
//		color++;
		//seek(VGA, ((y*VGA_WIDTH+x) * (VGA_BPP/8)));
		//write(VGA, &color, (VGA_BPP/8));
	}
		sleepms(1);
	}
	close(VGA);

}


//extern void * vga2;

void putpixel2(int x, int y, unsigned char color)
{
	uint8 *vga = (uint8 *)0xA0000;          /* this points to video memory. */

	uint32 * pixel;
	pixel = (uint32) vga + (y*VGA_WIDTH+x);
	*(pixel) = color;
}

/* example for 320x200 VGA */
void putpixel(int pos_x, int pos_y, unsigned char VGA_COLOR)
{
    //unsigned char* location = (unsigned char*)0xA0000 + (VGA_WIDTH * pos_y + pos_x)*3;
    unsigned char* location = (unsigned char*)0xA0000 + (VGA_WIDTH * pos_y + pos_x);
    //*location = VGA_COLOR;
    *location = 0xff;
    *(location+1) = 0xff;
    *(location+2) = 0xff;
}


// define our structure
typedef struct __attribute__ ((packed)) {
    unsigned short di, si, bp, sp, bx, dx, cx, ax;
    unsigned short gs, fs, es, ds, eflags;
} regs16_t;
 
// tell compiler our int32 function is external
extern void inter32(unsigned char intnum, regs16_t *regs);
// extern void mouse_init();
extern void keyboard_init();

 
process	main(void)
{

/* Print message */
    uint16 *base;
    char *msg = "Congraturations!  Welcome to the 64-bit world!";
    int offset;
//
    base = (uint16 *)0xb8000;
    offset = 0;
    while ( msg[offset] ) {
        *(base + offset) = 0x0700 | msg[offset];
        offset++;
    }

	int x,y;
	// for (y=0; y<VGA_HEIGHT; y++)
	//for (x=0; x<300; x++)
	//	pixel(x,x,0xffffffff);
		//putpixel(x,y,0xff);
	//while(1);

	/* Run the Xinu shell */
	printf("\n HOLA MUNDO \n");
//	while(1);
	paint2();
	linea2();
//	pixel(1,1, 0xffffffff );
//	pixel(10,1, 0xffffffff);
//	pixel(10,100, 0xffffffff);
//	while(1);

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	// mouse_init();
	keyboard_init();
	resume(create(mouse_p, 1024, 50, "mouse", 1, CONSOLE));
	resume(create(keyboard, 1024, 50, "keyboard", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
