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
	/* Find the pixel position in memory */
	void *fb = ((void *) (uint32) (vga->addr + vga->pos));

	for(uint32 i = 0; i < count; i += sizeof(uint32)){
		uint32 *pixel = fb;

		/* Take 4 bytes from the buffer to create the color */
		uint32 color = *((uint32 *)(buffer + i));
		*pixel = color;
		
		fb = fb + (vga->bpp/8);
	}

	return OK;
}