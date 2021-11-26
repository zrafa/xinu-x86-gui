/* miceinit.c  -  miceinit */

#include <xinu.h>
#include <mouse.h>

struct	mouse_t	mouse;

/*------------------------------------------------------------------------
 *  miceinit  -  Initialize the ps/2 mouse
 *------------------------------------------------------------------------
 */
devcall	miceinit (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	mouse.buttons = 0;
	mouse.x = 0;
	mouse.y = 0;

	unsigned char _status;  //unsigned char

	//Enable the auxiliary mouse device
	mouse_wait(1);
	outportb(MOUSE_CTRL_STATUS_PORT, 0xA8);
 
	//Enable the interrupts
	mouse_wait(1);
	outportb(MOUSE_CTRL_STATUS_PORT, 0x20);
	mouse_wait(0);
	_status=(inportb(MOUSE_DATA_PORT) | 2);
	mouse_wait(1);
	outportb(MOUSE_CTRL_STATUS_PORT, 0x60);
	mouse_wait(1);
	outportb(MOUSE_DATA_PORT, _status);
 
	//Tell the mouse to use default settings
	mouse_write(0xF6);
	mouse_read();  //Acknowledge
 
	//Enable the mouse
	mouse_write(0xF4);
	mouse_read();  //Acknowledge

	//Setup the mouse handler: IRQ 12 for mouse
        set_evec(12 + IRQBASE, (uint32)micehandlerirq);

	return OK;
}
