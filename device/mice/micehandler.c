/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

/* micehandler.c - micehandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  micehandler  -  Handle an interrupt for the mouse device
 *------------------------------------------------------------------------
 */

extern int mouse_mode;

int mouse_cycle = 0;
unsigned char mouse_byte[4];

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


#define MOUSE_V_BIT  0x08
void micehandler(uint8 data)
{

	switch(mouse_cycle) {
	case 0:
		//mouse_byte[0]=inportb(MOUSE_DATA_PORT);
		mouse_byte[0]=data;
		if (mouse_byte[0] == 0xAA) {		/* hot plug: re init mouse */
			return;
		}
		if (!(data & MOUSE_V_BIT)) {mouse_cycle=0; return;}
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

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_WRITE  0xD4
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08

void micehandler2(uint8 data) {
	uint8 status = inportb(MOUSE_STATUS);
	mouse_byte[0] = data;
	++mouse_cycle;
	if (!(data & MOUSE_V_BIT)) {mouse_cycle=0; return;}

	while (status & MOUSE_BBIT) {
		uint8 mouse_in = inportb(MOUSE_PORT);
		if (status & MOUSE_F_BIT) {
			switch (mouse_cycle) {
			case 0:
				mouse_byte[0] = mouse_in;
				if (!(mouse_in & MOUSE_V_BIT)) {mouse_cycle=0; return;}
				++mouse_cycle;
				break;
			case 1:
				mouse_byte[1] = mouse_in;
				++mouse_cycle;
				break;
			case 2:
				mouse_byte[2] = mouse_in;
				break;
			case 3:
				mouse_byte[3]=mouse_in;
				break;
			}
		}
		status = inportb(MOUSE_STATUS);
	}
	finish_mice();
}

