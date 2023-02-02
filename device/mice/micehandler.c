/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

/* micehandler.c - micehandler */

#include <xinu.h>
#include <mouse.h>
#include <vga.h>

/*------------------------------------------------------------------------
 *  micehandler  -  Handle an interrupt for the mouse device
 *------------------------------------------------------------------------
 */
void micehandler(void)
{
	uint8 d, state;
	int i, rel_x, rel_y;
	unsigned char mouse_byte[3];

	int mouse_cycle = 0;

	for (i=0; i<3; i++)
	switch(mouse_cycle) {
	case 0:
		mouse_byte[0]=inportb(MOUSE_DATA_PORT);
		if (mouse_byte[0] == 0xAA) {		/* hot plug: re init mouse */
			print_text_on_vga(10, 100, "RE INIT");
			/*
			mouse_byte[0]=inportb(MOUSE_DATA_PORT);
			//Tell the mouse to use default settings
        		mouse_write(0xF6);
        		mouse_read();  //Acknowledge
 
        		//Enable the mouse
        		mouse_write(0xF4);
        		mouse_read();  //Acknowledge
			*/
			return;
		}
		mouse_cycle++;
		break;
	case 1:
		mouse_byte[1]=inportb(MOUSE_DATA_PORT);
		mouse_cycle++;
		break;
	case 2:
		mouse_byte[2]=inportb(MOUSE_DATA_PORT);
		mouse_cycle=0;

		state = mouse_byte[0];
		mouse.buttons = state;
		d = mouse_byte[1];
		//mouse.x = d - ((state << 4) & 0x100);
		rel_x = d - ((state << 4) & 0x100);
		d = mouse_byte[2];
		//mouse.y = d - ((state << 3) & 0x100);
		rel_y = d - ((state << 3) & 0x100);
		//mouse.x = rel_x;
		//mouse.y = rel_y;
		mouse.x = mouse.x + rel_x;
		mouse.y = mouse.y + (-1)*rel_y;

		mouse.y = (mouse.y < 0) ? 0 : mouse.y;
		mouse.y = (mouse.y >= VGA_HEIGHT) ? VGA_HEIGHT : mouse.y;
		mouse.x = (mouse.x < 0) ? 0 : mouse.x;
		mouse.x = (mouse.x >= VGA_WIDTH) ? VGA_WIDTH : mouse.x;
		break;
	}
}

