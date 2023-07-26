/* ethwrite.c - ethwrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 * ethwrite - write a packet to an E1000E device
 *------------------------------------------------------------------------
 */
devcall	ethwrite(
	struct	dentry	*devptr, 	/* entry in device switch table	*/
	void	*buf,			/* buffer that holds a packet	*/
	uint32	len			/* length of buffer		*/
	)
{
	struct	ethcblk	*ethptr; 	/* ptr to entry in ethertab 	*/
	struct 	eth_tx_desc *descptr;/* ptr to ring descriptor 	*/
	char 	*pktptr; 		/* ptr used during packet copy  */
	uint32	tail;			/* index of ring buffer for pkt	*/
	uint32 	tdt;

	ethptr = &ethertab[devptr->dvminor];

	/* Verify Ethernet interface is up and arguments are valid */

	if ((ETH_STATE_UP != ethptr->state)
			|| (len < ETH_HDR_LEN)
			|| (len > ETH_MAX_PKT_LEN) ) {
		return SYSERR;
	}

	/* If padding of short packet is enabled, the value in TX 	*/
	/* 	descriptor length feild should be not less than 17 	*/
	/* 	bytes */

	if (len < 17)
		return SYSERR;

	/* Wait for a free ring slot */

	wait(ethptr->osem);

	/* Find the tail of the ring to insert packet */
	
	tail = ethptr->txTail;
	descptr = (struct eth_tx_desc *)ethptr->txRing + tail;

	/* Copy packet to transmit ring buffer */
	
	pktptr = (char *)((uint32)descptr->buffer_addr & ADDR_BIT_MASK);
	memcpy(pktptr, buf, len);

	/* Insert transmitting command and length */
	
	descptr->lower.data &= E1000_TXD_CMD_DEXT; 
	descptr->lower.data = E1000_TXD_CMD_IDE |
			      E1000_TXD_CMD_RS | 
			      E1000_TXD_CMD_IFCS |
			      E1000_TXD_CMD_EOP |
			      len;
	descptr->upper.data = 0;

	/* Add descriptor by advancing the tail pointer */
	
	tdt = eth_dev_readl(ethptr->iobase, E1000_TDT(0));
	tdt = (tdt + 1) % ethptr->txRingSize;
	eth_dev_writel(ethptr->iobase, E1000_TDT(0), tdt);

	/* Advance the ring tail pointing to the next available ring 	*/
	/* 	descriptor 						*/
	
	ethptr->txTail = (ethptr->txTail + 1) % ethptr->txRingSize;

	return len;
}
