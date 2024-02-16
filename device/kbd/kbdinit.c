/* kbdinit.c  -  kbdinit */

#include <xinu.h>

struct  kbdcblk kbdc;

/*------------------------------------------------------------------------
 *  kbdinit  -  Initialize the ps/2 keyboard
 *------------------------------------------------------------------------
 */
devcall	kbdinit (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	byte _status;

    /* Initialize values in the kbd control block */
    kbdc.kbdsem = semcreate(0);
    kbdc.key = 0;

	// Enable the keyboard device
	ps2_wait(1);
	outb(PS2_CTRL_STATUS_PORT, ENABLE_KBD_INTERFACE);

	// Enable the interrupts
	ps2_wait(1);
	outb(PS2_CTRL_STATUS_PORT, READ_COMMAND);
	ps2_wait(0);
	_status = (inb(PS2_DATA_PORT) | INT);
	ps2_wait(1);
	outb(PS2_CTRL_STATUS_PORT, PS2_DATA_PORT);
	ps2_wait(1);
	outb(PS2_DATA_PORT, _status);
 
	// Enable the keyboard
	while ((inb(PS2_CTRL_STATUS_PORT) & IBF) != 0) {
		inb(PS2_DATA_PORT);
	}; 

	set_evec(1 + IRQBASE, (uint32)kbddispatch);

	return OK;
}
