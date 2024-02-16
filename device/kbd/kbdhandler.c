/* kbdhandler.c - kbdhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  kbdhandler  -  Handle an interrupt for the keyboard device
 *------------------------------------------------------------------------
 */
void kbdhandler()
{
	int n;
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    kbdc.key = inb(PS2_DATA_PORT);
	n = semcount(kbdc.kbdsem);
	if (n < 1)
		signal(kbdc.kbdsem);	
}

