/* vgacontrol.c */

#include <xinu.h>

/*------------------------------------------------------------------------
 * vgacontrol - implement control function for an vga device
 *------------------------------------------------------------------------
 */
devcall vgacontrol(
        struct  dentry  *devptr,        /* entry in device switch table */
        int32   func,                   /* control function             */
        int32   arg1,                   /* argument 1, if needed        */
        int32   arg2                    /* argument 2, if needed        */
        )
{

        switch (func) {

                /* Get vga pitch */

                case VGA_GET_PITCH:
			*(uint32 *)arg1 = vga->pitch;
			break;
                case VGA_GET_BPP:
			*(byte *)arg1 = vga->bpp;
			break;
                case VGA_GET_WIDTH:
			*(uint32 *)arg1 = vga->width;
			break;
                case VGA_GET_HEIGHT:
			*(uint32 *)arg1 = vga->height;
			break;
                case VGA_GET_POS:
			*(uint32 *)arg1 = vga->pos;
			break;
		default:
			return SYSERR;
	}

	return OK;
}
