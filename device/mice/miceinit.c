/* miceinit.c  -  miceinit */

#include <xinu.h>

struct  mousecblk mousec;

/*------------------------------------------------------------------------
 *  miceinit  -  Initialize the ps/2 mouse
 *------------------------------------------------------------------------
 */
devcall	miceinit (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	unsigned char _status;

    /* Initialize values in the mouse control block */
    mousec.mousesem = semcreate(0);
	mousec.mouse.buttons = 0;
	mousec.mouse.x = 0;
	mousec.mouse.y = 0;

	// Enable the auxiliary mouse device
    ps2_wait(1);
	outb(PS2_CTRL_STATUS_PORT, ENABLE_AUX_INTERFACE);

	// Enable the interrupts
	ps2_wait(1);
	outb(PS2_CTRL_STATUS_PORT, READ_COMMAND);
	ps2_wait(0);
	_status = (inb(PS2_DATA_PORT) | INT2);
	ps2_wait(1);
	outb(PS2_CTRL_STATUS_PORT, WRITE_COMMAND);
	ps2_wait(1);
	outb(PS2_DATA_PORT, _status);
 
	// Tell the mouse to use default settings
	ps2_write(SET_DEFAULTS);
	ps2_read();  // Acknowledge
 
	// Enable the mouse
	ps2_write(ENABLE_PACKET_STREAMING);
	ps2_read();  // Acknowledge

	// Setup the mouse handler: IRQ 12 for mouse
    set_evec(12 + IRQBASE, (uint32)micedispatch);

	return OK;
}
