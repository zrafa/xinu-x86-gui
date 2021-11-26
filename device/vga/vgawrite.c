/* vgawrite.c  -  vgawrite */

#include <xinu.h>
#include <vga.h>

/*------------------------------------------------------------------------
 * vgawrite  -  Write a block to vga screen
 *------------------------------------------------------------------------
 */
devcall	vgawrite (
	struct dentry	*devptr,	/* Entry in device switch table	*/
	char *buffer,        /* Address of buffer            */
	uint32        count  /* Length of buffer             */
	)
{
 	memcpy((vga.addr + vga.pos), buffer, count);
	return OK;
}
