/* micehandler.c - micehandler */

#include <xinu.h>

#define MOUSE_V_BIT  0x08

int mouse_cycle = 0;
unsigned char mouse_byte[4];

void finish_mice(void)
{
	uint8 d, state;
	int rel_x, rel_y;
	int n;

    mouse_t *mouse_pt = &mousec.mouse;


	mouse_cycle = 0;

	if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40) {
		/* x/y overflow? bad packet! */
		return;
	}

	state = mouse_byte[0];

	mouse_pt->buttons = state;

	d = mouse_byte[1];
	rel_x = d - ((state << 4) & 0x100);
	mouse_pt->x = mouse_pt->x + rel_x;

	d = mouse_byte[2];
	rel_y = d - ((state << 3) & 0x100);
	mouse_pt->y = mouse_pt->y + (-1)*rel_y;

	mouse_pt->y = (mouse_pt->y < 0) ? 0 : mouse_pt->y;
	mouse_pt->y = (mouse_pt->y >= VGA_HEIGHT) ? VGA_HEIGHT : mouse_pt->y;
	mouse_pt->x = (mouse_pt->x < 0) ? 0 : mouse_pt->x;
	mouse_pt->x = (mouse_pt->x >= VGA_WIDTH) ? VGA_WIDTH : mouse_pt->x;

	n = semcount(mousec.mousesem);
	if (n < 1)
		signal(mousec.mousesem);	

}

/*------------------------------------------------------------------------
 *  micehandler  -  Handle an interrupt for the mouse device
 *------------------------------------------------------------------------
 */

void micehandler()
{

	switch(mouse_cycle) {
	    case 0:
		    mouse_byte[0] = inb(PS2_DATA_PORT);
		    if (mouse_byte[0] == 0xAA) {		/* hot plug: re init mouse */
			    return;
		    }
		    if (!(mouse_byte[0] & MOUSE_V_BIT)) {
                mouse_cycle=0;
                return;
            }
		    mouse_cycle++;
		    break;
	    case 1:
		    mouse_byte[1] = inb(PS2_DATA_PORT);
		    mouse_cycle++;
		    break;
	    case 2:
		    mouse_byte[2] = inb(PS2_DATA_PORT);
		    finish_mice();
		    break;
//	    case 3:
//		    mouse_byte[3] = inb(PS2_DATA_PORT);
//		    finish_mice();
//		    break;
	}
}

