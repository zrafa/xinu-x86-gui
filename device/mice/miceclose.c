/* miceclose.c  -  miceclose */

#include <xinu.h>

/*------------------------------------------------------------------------
 * miceclose  -  Close the mouse device
 *------------------------------------------------------------------------
 */
devcall	miceclose (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	return OK;
}
