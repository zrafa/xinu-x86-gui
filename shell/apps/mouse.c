/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <gui.h>

#define YELLOW 0x00ffff00
#define BLACK 0x00303030
#define RED 0x00ff0000

/*
#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)
*/



//Mouse.inc by SANiK
//License: Use as you wish, except to cause damage
// byte mouse_cycle=0;     //unsigned char
// unsigned char mouse_byte[3];    //signed char

process mouse_p (
                did32   dev             /* ID of tty device from which  */
        )                               /*   to accept commands         */
{
	int mouse_x=0;         //signed char
	int mouse_y=0;         //signed char
	int old_x=0;
	int old_y=0;;
	unsigned char c;
	char * h = "Xinu x86 GUI experiments: VGA display, mouse and kbd";
	gui_print_text(10, 500, h, BLACK, YELLOW);
	char texto[10];
	int buf[3];
	
	open(MOUSE, NULL, 0);
	while(1) {
		read(MOUSE, buf, 3);
		mouse_x = buf[1];
		mouse_y = buf[2];

		if (buf[0] & 0x01)
		 	sprintf(texto, "mouse x: %d  y: %d    left click       ", mouse_x, mouse_y);
		else if (buf[0] & 0x02)
		 	sprintf(texto, "mouse x: %d  y: %d    right click       ", mouse_x, mouse_y);
		else if (buf[0] & 0x04)
		 	sprintf(texto, "mouse x: %d  y: %d    middle click       ", mouse_x, mouse_y);
		else
		 	sprintf(texto, "mouse x: %d  y: %d                      ", mouse_x, mouse_y);


		gui_pixel(mouse_x, mouse_y, RED);
		gui_pixel(mouse_x+1, mouse_y, RED);
		gui_pixel(mouse_x-1, mouse_y, RED);
		gui_pixel(mouse_x, mouse_y-1, RED);
		gui_pixel(mouse_x, mouse_y+1, RED);
		gui_print_text(10, 10, texto, BLACK, YELLOW);
		old_x = mouse_x; old_y = mouse_y;
		c = 0;
		sleepms(1);

	}
	close(MOUSE);

}

