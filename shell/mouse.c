/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <vga.h>

/*
#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)
*/



//Mouse.inc by SANiK
//License: Use as you wish, except to cause damage
// byte mouse_cycle=0;     //unsigned char
// unsigned char mouse_byte[3];    //signed char
int mouse_x=0;         //signed char
int mouse_y=0;         //signed char

extern void print_text_on_buffer(unsigned char x, unsigned char y, char *text);
extern void borrar(int x1, int y1, int x2, int y2);

process mouse_p (
                did32   dev             /* ID of tty device from which  */
        )                               /*   to accept commands         */
{
	int old_x=0;
	int old_y=0;;
	unsigned char c;
	char * h = "hola mundo";
	print_text_on_buffer(10, 10, h);
	char texto[10];
	int buf[3];
	
	open(MOUSE, NULL, 0);
	while(1) {
		//borrar(0,0,300,300);
		read(MOUSE, buf, 3);
/*
                mouse_x = mouse_x + buf[1]; /* rel_x; */
/*
                mouse_y = mouse_y + (-1)*buf[2]; /* rel_y; */

/*
                mouse_y = (mouse_y < 0) ? 0 : mouse_y;
                mouse_y = (mouse_y >= VGA_HEIGHT) ? VGA_HEIGHT : mouse_y;
                mouse_x = (mouse_x < 0) ? 0 : mouse_x;
                mouse_x = (mouse_x >= VGA_WIDHT) ? VGA_WIDHT : mouse_x;
*/
		mouse_x = buf[1];
		mouse_y = buf[2];

		if (buf[0] && 0x01)
		 	sprintf(texto, "x: %d  y: %d    left : 1\n", mouse_x, mouse_y);
		else
		 	sprintf(texto, "x: %d  y: %d    left : 0\n", mouse_x, mouse_y);
		pixel(old_x, old_y, 0x00ffff00);
		pixel(mouse_x, mouse_y, 0x00ff0000);
		print_text_on_buffer(10, 10, texto);
		old_x = mouse_x; old_y = mouse_y;
		// printf("mouse: 0x%x\n", c);
		c = 0;
		// mouse_write(c);
		sleepms(10);

	}
	close(MOUSE);

}

