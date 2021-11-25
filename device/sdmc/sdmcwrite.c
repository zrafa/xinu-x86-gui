/* sdmcwrite.c  -  sdmcwrite */

#include <xinu.h>
#include <sdmc.h>

/*------------------------------------------------------------------------
 * sdmcwrite - Write a buffer to an SD memory card
 *------------------------------------------------------------------------
 */
devcall	sdmcwrite (
	  struct dentry	*devptr,	/* entry in device switch table	*/
	  char	*buff,			/* buffer to hold disk block	*/
	  int32	blk			/* block number of disk block	*/
	)
{


	return OK;
}
