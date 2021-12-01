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
 	//memcpy((vga.addr + vga.pos + 1), buffer, count);
	//*(vga.addr + vga.pos + 3) = *buffer;
	*(uint8 *)(vga.addr + vga.pos+1) = 0x1f;
	//*buffer;
	//*(vga.addr + vga.pos+2) = *buffer;
	//*(vga.addr + vga.pos+3) = *buffer;
	//*(vga.addr + vga.pos+4) = *buffer;
	return OK;
}
