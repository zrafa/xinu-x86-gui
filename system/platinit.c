/* platinit.c - platinit */

#include <xinu.h>


/*------------------------------------------------------------------
 * platinit - platform specific initialization
 *------------------------------------------------------------------
 */
status	platinit()
{
    status	retval;

	/* Initialize the uart */

	uartinit();

	/* initialize the PCI bus */

	retval = pci_init();

    return	retval;
}

