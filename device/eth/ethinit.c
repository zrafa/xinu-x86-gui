/* ethinit.c - ethinit */

#include <xinu.h>

#ifndef Neth
#define	Neth	1
#endif
struct	ethcblk	ethertab[Neth];		/* Ethernet control blocks 	*/

/*------------------------------------------------------------------------
 * ethinit - Initialize Ethernet device structures
 *------------------------------------------------------------------------
 */
devcall	ethinit (
	  struct dentry *devptr
	)
{
	struct	ethcblk *ethptr;
	int32	dinfo;			/* device information		*/

	/* Initialize structure pointers */

	ethptr = &ethertab[devptr->dvminor];
	
	memset(ethptr, '\0', sizeof(struct ethcblk));
	ethptr->dev = devptr;
	ethptr->csr = devptr->dvcsr;
	ethptr->state = ETH_STATE_DOWN;
	ethptr->mtu = ETH_MTU;
	ethptr->errors = 0;
	ethptr->addrLen = ETH_ADDR_LEN;
	ethptr->rxHead = ethptr->rxTail = 0;
	ethptr->txHead = ethptr->txTail = 0;

	dinfo = find_pci_device(INTEL_82545EM_DEVICE_ID,
				     INTEL_VENDOR_ID, 0);
	if (dinfo == SYSERR) {
		kprintf("Could not find an Intel 82545EM Ethernet\n");
		return SYSERR;
	}
	kprintf("Found Intel 82545EM Ethernet NIC\n");

	ethptr->type = NIC_TYPE_82545EM;
	ethptr->pcidev = dinfo;
	if (_82545EMInit(ethptr) == SYSERR) {
		kprintf("Could not initialize Intel 82545EM NIC\n");
		return SYSERR;
	}

	/* Mark Ethernet interface active */

	ethptr->state = ETH_STATE_UP;

	return OK;
}
