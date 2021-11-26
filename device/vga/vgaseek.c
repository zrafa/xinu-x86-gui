/* vgaseek.c - vgaseek */

#include <xinu.h>
#include <vga.h>

/*------------------------------------------------------------------------
 * vgaseek  -  Change the current position in the vga screen
 *------------------------------------------------------------------------
 */
devcall	vgaseek (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  uint32 pos			/* New file position		*/
	)
{
	vga.pos = pos;

	return OK;
}
