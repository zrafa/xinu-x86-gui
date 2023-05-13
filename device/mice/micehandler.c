/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

/* micehandler.c - micehandler */

#include <xinu.h>
#include <vga.h>

/*------------------------------------------------------------------------
 *  micehandler  -  Handle an interrupt for the mouse device
 *------------------------------------------------------------------------
 */

extern int mouse_mode;

int mouse_cycle = 0;
unsigned char mouse_byte[3];

void finish_mice(void)
{
	uint8 d, state;
	int rel_x, rel_y;

		mouse_cycle=0;

		if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40) {
			/* x/y overflow? bad packet! */
			return;
		}

		state = mouse_byte[0];

		mouse.buttons = state;

		d = mouse_byte[1];
		rel_x = d - ((state << 4) & 0x100);
		mouse.x = mouse.x + rel_x;

		d = mouse_byte[2];
		rel_y = d - ((state << 3) & 0x100);
		mouse.y = mouse.y + (-1)*rel_y;

		mouse.y = (mouse.y < 0) ? 0 : mouse.y;
		mouse.y = (mouse.y >= VGA_HEIGHT) ? VGA_HEIGHT : mouse.y;
		mouse.x = (mouse.x < 0) ? 0 : mouse.x;
		mouse.x = (mouse.x >= VGA_WIDTH) ? VGA_WIDTH : mouse.x;
}


void micehandler(uint8 data)
{

	switch(mouse_cycle) {
	case 0:
		//mouse_byte[0]=inportb(MOUSE_DATA_PORT);
		mouse_byte[0]=data;
		if (mouse_byte[0] == 0xAA) {		/* hot plug: re init mouse */
			return;
		}
		mouse_cycle++;
		break;
	case 1:
		mouse_byte[1]=data;
		mouse_cycle++;
		break;
	case 2:
		mouse_byte[2]=data;
		finish_mice();
		break;
	case 3:
		mouse_byte[3]=data;
		finish_mice();
		break;
	}
}

