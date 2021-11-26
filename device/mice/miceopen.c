/* miceopen.c  -  miceopen */

#include <xinu.h>

/*------------------------------------------------------------------------
 * miceopen  -  Open the ps/2 mouse device
 *------------------------------------------------------------------------
 */

devcall	miceopen (
	 struct	dentry	*devptr,	/* Entry in device switch table	*/
	 char	*name,			/* Unused for a mice */
	 char	*mode			/* Unused for a mice */
	)
{
	/* No action -- just return the device descriptor */

	return devptr->dvnum;
}
