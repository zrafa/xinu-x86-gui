/* sdmcclose.c  -  sdmcclose */

#include <xinu.h>
#include <sdmc.h>

/*------------------------------------------------------------------------
 * sdmcclose - Close a SD memory card device
 *------------------------------------------------------------------------
 */
devcall	sdmcclose (
	  struct dentry	*devptr		/* entry in device switch table	*/
	)
{
	return OK;
}
