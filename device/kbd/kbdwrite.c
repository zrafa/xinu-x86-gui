/* kbdwrite.c  -  kbdwrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 * kbdwrite  -  Write a block to kbd?
 *------------------------------------------------------------------------
 */
devcall	kbdwrite (
	struct dentry	*devptr,	/* Entry in device switch table	*/
	char *buffer,        /* Address of buffer            */
	uint32        count  /* Length of buffer             */
	)
{
	return OK;
}
