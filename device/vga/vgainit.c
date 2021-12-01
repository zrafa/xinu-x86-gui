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
	vga.addr = (unsigned char *) (mbi->framebuffer_addr);
	vga.width = mbi->framebuffer_width;
	vga.height = mbi->framebuffer_height;
	vga.pitch = mbi->framebuffer_pitch;
	vga.bpp = mbi->framebuffer_bpp;
	vga.pos = 0;

	return OK;
}
