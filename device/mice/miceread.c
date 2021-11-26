/* miceread.c  -  miceread */

#include <xinu.h>
#include <mouse.h>

/*------------------------------------------------------------------------
 * miceread  -  Read the status of the mouse driver
 *------------------------------------------------------------------------
 */
devcall	miceread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char          *buffer,        /* Address of buffer            */
          uint32        count           /* Length of buffer             */
	)
{
	memcpy(buffer, &mouse, sizeof(mouse));

	//mouse.buttons = 0;
	//mouse.x = 0;
	//mouse.y = 0;
	return OK;
}
