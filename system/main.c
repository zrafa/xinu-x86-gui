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
word *my_clock = (word *)0x046C;      /* this points to the 18.2hz system
                                         clock. */

/**************************************************************************
 *  set_mode                                                              *
 *     Sets the video mode.                                               *
 **************************************************************************/

void paint()
{
__asm__ (
	"mov vga3, %eax"
	);

// Keep the color in EDX
__asm__ (
	"xor	%edx, %edx"
        );

__asm__ (
"main_loop:"
	);
	// Reset the pixel counter in ECX
__asm__ (
	"xor	%ecx, %ecx"
        );
	
	// Increase the color
__asm__ (
	"inc	%dl"
        );
	
__asm__ (
"pixel_loop:"
	);
	// Draw current pixel
__asm__ (
	"mov	%edx, 0(%eax,%ecx,4)"
        );
	
	// Jump to next pixel
__asm__ (
	"inc	%ecx"
        );
	
	// Have all the pixels in this frame been drawn?
__asm__ (
	"cmpl	$786432, %ecx"
        );
	
	// Yes, draw next pixel
__asm__ (
	"jl	pixel_loop"
        );
	
	// No, jump to next frame
__asm__ (
	"jmp	main_loop"
        );

}

extern unsigned char * vga2;

void pixel(int x, int y, uint32 color) {
	*(vga2+(((y*1024)+x) *4)) = color;
	*(vga2+(((y*1024)+x) *4)+1) = (color >> 8);
	*(vga2+(((y*1024)+x) *4)+2) = (color >> 16);
}

void paint2(){

	int color = 0x00ffff00;
	int i,j,x,y;
	for (y=0; y<768; y++)
	for (x=0; x<1024; x++)
	//for (i=0; i<(1024*768); i++)
	//for (i=0; i<(1024*5); i++)
		//*(vga2+(i*4)) = color;
		//*(vga2+(i*4)+2) = color;
		pixel(x,y,color);

}


//extern void * vga2;

void putpixel2(int x, int y, unsigned char color)
{

	uint32 * pixel;
	pixel = (uint32) vga2 + (y*1024+x);
	*(pixel) = color;
}

/* example for 320x200 VGA */
void putpixel(int pos_x, int pos_y, unsigned char VGA_COLOR)
{
    //unsigned char* location = (unsigned char*)0xA0000 + 320 * pos_y + pos_x;
    //*location = VGA_COLOR;
    unsigned char* location = vga2 + ((1024 * pos_y + pos_x) * 4);
    *location = 0xff;
}

/**************************************************************************
 *  plot_pixel                                                            *
 *    Plot a pixel by directly writing to video memory, with no           *
 *    multiplication.                                                     *
 **************************************************************************/

uint16 *VGA = (uint16 *)0xA0000;          /* this points to video memory. */
void plot_pixel(int x,int y,byte color)
{
  /*  y*320 = y*256 + y*64 = y*2^8 + y*2^6   */
  VGA[(y<<8)+(y<<6)+x]=color;
}

// define our structure
typedef struct __attribute__ ((packed)) {
    unsigned short di, si, bp, sp, bx, dx, cx, ax;
    unsigned short gs, fs, es, ds, eflags;
} regs16_t;
 
// tell compiler our int32 function is external
extern void inter32(unsigned char intnum, regs16_t *regs);
extern void mouse_init();
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

	/* Run the Xinu shell */
	printf("\n HOLA MUNDO \n");
	paint2();

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	mouse_init();
	keyboard_init();
	resume(create(mouse, 1024, 50, "mouse", 1, CONSOLE));
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
