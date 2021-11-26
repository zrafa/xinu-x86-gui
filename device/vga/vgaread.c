/* vgaread.c  -  vgaread */

#include <xinu.h>
#include <vga.h>

/*------------------------------------------------------------------------
 * vgaread  -  Read a block from the vga screen
 *------------------------------------------------------------------------
 */
devcall	vgaread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char          *buffer,        /* Address of buffer            */
          uint32        count           /* Length of buffer             */
	)
{
	return OK;
}
