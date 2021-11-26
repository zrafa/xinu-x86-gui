/* micewrite.c  -  micewrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 * micewrite  -  Write a block to mice?
 *------------------------------------------------------------------------
 */
devcall	micewrite (
	struct dentry	*devptr,	/* Entry in device switch table	*/
	char *buffer,        /* Address of buffer            */
	uint32        count  /* Length of buffer             */
	)
{
	return OK;
}
