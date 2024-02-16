/* miceread.c  -  miceread */

#include <xinu.h>

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
 	wait(mousec.mousesem);
 	memcpy(buffer, &mousec.mouse, sizeof(mousec.mouse));

	return OK;
}
