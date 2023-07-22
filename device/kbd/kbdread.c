/* kbdread.c  -  kbdread */

#include <xinu.h>

/*------------------------------------------------------------------------
 * kbdread  -  Read the status of the keyboard driver
 *------------------------------------------------------------------------
 */
devcall	kbdread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char          *buffer,        /* Address of buffer            */
          uint32        count           /* Length of buffer             */
	)
{
	//wait(kbdc.kbdsem);
	memcpy(buffer, &(kbdc.key), sizeof(kbdc.key));
	return OK;
}
