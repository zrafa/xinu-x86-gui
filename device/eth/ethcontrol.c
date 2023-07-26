/* ethcontrol.c - ethcontrol, ethIrqEnable, ethIrqDisable */

#include <xinu.h>

/*------------------------------------------------------------------------
 * ethcontrol - implement control function for an eth device
 *------------------------------------------------------------------------
 */
devcall	ethcontrol(
	struct	dentry	*devptr,	/* entry in device switch table	*/
	int32	func,			/* control function		*/
	int32	arg1,			/* argument 1, if needed 	*/
	int32	arg2			/* argument 2, if needed	*/
	)
{
	struct	ethcblk	*ethptr; 	/* pointer to control block	*/
	uint32 rar_low, rar_high;
	uint8 *addr;


	ethptr = &ethertab[devptr->dvminor];

	switch (func) {

		/* Get MAC address */

		case ETH_CTRL_GET_MAC:
			memcpy((byte *)arg1, ethptr->devAddress, 
					ETH_ADDR_LEN);
			break;

		case ETH_CTRL_SET_MAC:
			addr = (uint8 *)arg1;
			rar_low = ((uint32) addr[0] |
				  ((uint32) addr[1] << 8) |
				  ((uint32) addr[2] << 16) | 
				  ((uint32) addr[3] << 24));

			rar_high = ((uint32) addr[4] | 
				   ((uint32) addr[5] << 8));

			if (rar_low || rar_high)
				rar_high |= E1000_RAH_AV;

			eth_dev_writel(ethptr->iobase, E1000_RAL(arg2), 
					rar_low);
			eth_dev_flush(ethptr->iobase);
			eth_dev_writel(ethptr->iobase, E1000_RAH(arg2), 
					rar_high);
			eth_dev_flush(ethptr->iobase);
			
			break;

		default:
			return SYSERR;
	}

	return OK;
}

/*------------------------------------------------------------------------
 * ethIrqDisable - Mask off interrupt generation on the NIC
 *------------------------------------------------------------------------
 */
void ethIrqDisable(
	struct 	ethcblk	*ethptr
	)
{
	eth_dev_writel(ethptr->iobase, E1000_IMC, ~0);

	eth_dev_flush(ethptr->iobase);
}

/*------------------------------------------------------------------------
 * eth_irq_enable - Enable default interrupt generation settings
 *------------------------------------------------------------------------
 */
void ethIrqEnable(
	struct 	ethcblk	*ethptr
	)
{
	eth_dev_writel(ethptr->iobase, E1000_IMS, E1000_IMS_ENABLE_MASK);

	eth_dev_flush(ethptr->iobase);
}
