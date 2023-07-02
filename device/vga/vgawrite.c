/* vgawrite.c  -  vgawrite */

#include <xinu.h>

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
	/* Find the pixel position in memory */
	void *fb = ((void *) (uint32) (vga->addr + vga->pos));
	memcpy(fb, buffer, count);
	
	return OK;
}