/* vgaclose.c  -  vgaclose */

#include <xinu.h>

/*------------------------------------------------------------------------
 * vgaclose  -  Close vga 
 *------------------------------------------------------------------------
 */
devcall	vgaclose (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	return OK;
}
