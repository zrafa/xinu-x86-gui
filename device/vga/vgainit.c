/* vgainit.c  -  vgainit */

#include <xinu.h>
#include <vga.h>

struct	vga_t	vga;

/*------------------------------------------------------------------------
 *  vgainit  -  Initialize the vga screen
 *------------------------------------------------------------------------
 */
devcall	vgainit (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	vga.addr = (unsigned char *) (unsigned long) mbi->framebuffer_addr;
	vga.pos = 0;

	return OK;
}
