/* vgaopen.c  -  vgaopen */

#include <xinu.h>

/*------------------------------------------------------------------------
 * vgaopen  -  Open the vga screen
 *------------------------------------------------------------------------
 */

devcall	vgaopen (
	 struct	dentry	*devptr,	/* Entry in device switch table	*/
	 char	*name,			/* Unused for a vga */
	 char	*mode			/* Unused for a vga */
	)
{
	/* No action -- just return the device descriptor */

	return devptr->dvnum;
}
