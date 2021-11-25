/* 82545EMInit.c - _82545EMInit */

#include <xinu.h>

local 	status 	_82545EM_init_hw(struct ethcblk *);
local 	void 	_82545EM_reset_hw(struct ethcblk *);
local 	void 	_82545EM_configure_rx(struct ethcblk *);
local 	void 	_82545EM_configure_tx(struct ethcblk *);
status 	_82545EM_read_phy_reg(struct ethcblk *, uint32, uint16 *);
status 	_82545EM_write_phy_reg(struct ethcblk *, uint32, uint16);


/*------------------------------------------------------------------------
 * _82545EMInit - initialize Intel 82545EM Ethernet NIC
 *------------------------------------------------------------------------
 */
status 	_82545EMInit(
	struct 	ethcblk *ethptr
	)
{
	struct	eth_tx_desc*	txRingPtr;
	struct	eth_rx_desc*	rxRingPtr;
	uint16  command;
	int32	i;
	uint32  rar_low, rar_high, bufptr;

	/* Read PCI configuration information */
	/* Read I/O base address */

	pci_bios_read_config_dword(ethptr->pcidev, E1000_PCI_IOBASE,
			(uint32 *)&ethptr->iobase);
	ethptr->iobase &= ~1;
	ethptr->iobase &= 0xffff; /* the low bit is set to indicate I/O */

	/* Read interrupt line number */

	pci_bios_read_config_byte (ethptr->pcidev, E1000_PCI_IRQ,
			(byte *)&(ethptr->dev->dvirq));

	/* Enable PCI bus master, I/O port access */

	pci_bios_read_config_word(ethptr->pcidev, E1000_PCI_COMMAND, 
			&command);
	command |= E1000_PCI_CMD_MASK;
	pci_bios_write_config_word(ethptr->pcidev, E1000_PCI_COMMAND, 
			command);

	/* Read the MAC address */
	
	rar_low = eth_io_readl(ethptr->iobase, E1000_RAL(0));
	rar_high = eth_io_readl(ethptr->iobase, E1000_RAH(0));

	for (i = 0; i < ETH_ADDR_LEN; i++) 
		ethptr->devAddress[i] = (byte)(rar_low >> (i*8));
	for (i = 0; i < ETH_ADDR_LEN; i++)
		ethptr->devAddress[i + 4] = (byte)(rar_high >> (i*8));

	kprintf("MAC address is %02x:%02x:%02x:%02x:%02x:%02x\n",
			0xff&ethptr->devAddress[0],
			0xff&ethptr->devAddress[1],
			0xff&ethptr->devAddress[2],
			0xff&ethptr->devAddress[3],
			0xff&ethptr->devAddress[4],
			0xff&ethptr->devAddress[5]);

	/* Initialize structure pointers */

	ethptr->rxRingSize = E1000_RX_RING_SIZE;
	ethptr->txRingSize = E1000_TX_RING_SIZE;
	ethptr->isem = semcreate(0);
	ethptr->osem = semcreate(ethptr->txRingSize);

	/* Rings must be aligned on a 16-byte boundary */
	
	ethptr->rxRing = (void *)getmem((ethptr->rxRingSize + 1)
			* E1000_RDSIZE);
	ethptr->txRing = (void *)getmem((ethptr->txRingSize + 1)
			* E1000_TDSIZE);
	ethptr->rxRing = (void *)(((uint32)ethptr->rxRing + 0xf) & ~0xf);
	ethptr->txRing = (void *)(((uint32)ethptr->txRing + 0xf) & ~0xf);
	
	/* Buffers are highly recommended to be allocated on cache-line */
	/* 	size (64-byte for E8400) 				*/
	
	ethptr->rxBufs = (void *)getmem((ethptr->rxRingSize + 1) 
			* ETH_BUF_SIZE);
	ethptr->txBufs = (void *)getmem((ethptr->txRingSize + 1) 
			* ETH_BUF_SIZE);
	ethptr->rxBufs = (void *)(((uint32)ethptr->rxBufs + 0x3f) 
			& ~0x3f);
	ethptr->txBufs = (void *)(((uint32)ethptr->txBufs + 0x3f) 
			& ~0x3f);

	printf("rxbuf: %x \n", (uint32)ethptr->rxBufs);
	printf("txbuf: %x \n", (uint32)ethptr->txBufs);
	if ( (SYSERR == (uint32)ethptr->rxBufs) || 
	     (SYSERR == (uint32)ethptr->txBufs) ) {
		return SYSERR;
	}

	/* Set buffer pointers and rings to zero */
	
	memset(ethptr->rxBufs, '\0', ethptr->rxRingSize * ETH_BUF_SIZE);
	memset(ethptr->txBufs, '\0', ethptr->txRingSize * ETH_BUF_SIZE);
	memset(ethptr->rxRing, '\0', E1000_RDSIZE * ethptr->rxRingSize);
	memset(ethptr->txRing, '\0', E1000_TDSIZE * ethptr->txRingSize);

	/* Insert the buffer into descriptor ring */
	
	rxRingPtr = (struct eth_rx_desc *)ethptr->rxRing;
	bufptr = (uint32)ethptr->rxBufs;
	for (i = 0; i < ethptr->rxRingSize; i++) {
		rxRingPtr->buffer_addr = (uint64)bufptr;
		rxRingPtr++;
		bufptr += ETH_BUF_SIZE;
	}

	txRingPtr = (struct eth_tx_desc *)ethptr->txRing;
	bufptr = (uint32)ethptr->txBufs;
	for (i = 0; i < ethptr->txRingSize; i++) {
		txRingPtr->buffer_addr = (uint64)bufptr;
		txRingPtr++;
		bufptr += ETH_BUF_SIZE;
	}

	/* Reset packet buffer allocation to default */

	eth_io_writel(ethptr->iobase, E1000_PBA, E1000_PBA_48K);

	/* Reset the NIC to bring it into a known state and initialize it */

	_82545EM_reset_hw(ethptr);

	/* Initialize the hardware */

	printf("ok1\n");
	if (_82545EM_init_hw(ethptr) != OK)
		return SYSERR;
	printf("ok2\n");

	/* Configure the NIC */

	eth_io_writel(ethptr->iobase, E1000_AIT, 0);

	/* Configure the RX */

	_82545EM_configure_rx(ethptr);
	
	/* Configure the TX */

	_82545EM_configure_tx(ethptr);

	/* Register the interrupt and enable interrupt */
	
	set_evec(ethptr->dev->dvirq + IRQBASE, (uint32)ethdispatch);
	ethIrqEnable(ethptr);

	return OK;
}

/*------------------------------------------------------------------------
 * _82545EM_reset_hw - Reset the hardware 
 *------------------------------------------------------------------------
 */
local void _82545EM_reset_hw(
	struct 	ethcblk *ethptr
	)
{
	uint32 ctrl;

	/* Masking off all interrupts */

	eth_io_writel(ethptr->iobase, E1000_IMC, 0xffffffff);

	/* Disable the Transmit and Receive units. */

	eth_io_writel(ethptr->iobase, E1000_RCTL, 0);
	eth_io_writel(ethptr->iobase, E1000_TCTL, E1000_TCTL_PSP);
	eth_io_flush(ethptr->iobase);
	MDELAY(10);
	
	ctrl = eth_io_readl(ethptr->iobase, E1000_CTRL);

	/* Issuing a global reset */

	eth_io_writel(ethptr->iobase, E1000_CTRL, ctrl | E1000_CTRL_RST);
	MDELAY(5);

	eth_io_writel(ethptr->iobase, E1000_IMC, 0xffffffff);
	eth_io_readl(ethptr->iobase, E1000_ICR);
}

/*------------------------------------------------------------------------
 * _82545EM_init_hw - Initialize the hardware
 *------------------------------------------------------------------------
 */
local status _82545EM_init_hw(
	struct	ethcblk *ethptr
	)
{
	uint16 i;
	uint32 ctrl;
	uint16 	mii_autoneg_adv_reg, mii_1000t_ctrl_reg;
	uint16 	phy_data, phy_ctrl, phy_status;

	/* Setup the receive address */
	/* Zero out the other receive addresses */

	for (i = 1; i < E1000_82545EM_RAR_ENTRIES; i++) {
		eth_io_writel(ethptr->iobase, E1000_RAL(i), 0);
	    	eth_io_flush(ethptr->iobase);
	    	eth_io_writel(ethptr->iobase, E1000_RAH(i), 0);
	    	eth_io_flush(ethptr->iobase);
	}

	/* Zero out the Multicast HASH table */

	for (i = 0; i < E1000_82545EM_MTA_ENTRIES; i++) {
		eth_io_writel(ethptr->iobase, E1000_MTA + (i << 2), 0);
	    	eth_io_flush(ethptr->iobase);
	}

	/* Configure copper link settings */

	ctrl = eth_io_readl(ethptr->iobase, E1000_CTRL);
	ctrl |= E1000_CTRL_SLU;
	ctrl &= ~(E1000_CTRL_FRCSPD | E1000_CTRL_FRCDPX);
	eth_io_writel(ethptr->iobase, E1000_CTRL, ctrl);

	printf("ok2\n");
    	if (_82545EM_read_phy_reg(ethptr, M88E1000_PHY_SPEC_CTRL, 
			       &phy_data) != OK)
		return SYSERR;
	printf("ok2\n");

	phy_data |= M88E1000_PSCR_AUTO_X_MODE;
	phy_data &= ~M88E1000_PSCR_POLARITY_REVERSAL;

	if (_82545EM_write_phy_reg(ethptr, M88E1000_PHY_SPEC_CTRL, 
				phy_data) != OK)
		return SYSERR;
	printf("ok2\n");

	/* Commit the changes. */

    	if (_82545EM_read_phy_reg(ethptr, E1000_PHY_CONTROL, 
				  &phy_ctrl) != OK)
		return SYSERR;
	printf("ok2\n");

	phy_ctrl |= E1000_MII_CR_RESET;
	
	if (_82545EM_write_phy_reg(ethptr, E1000_PHY_CONTROL, 
				   phy_ctrl) != OK)
		return SYSERR;
	printf("ok2\n");
	
	DELAY(1);

	/* Setup autoneg and flow control advertisement and perform 	*/
	/* 	autonegotiation. 					*/

	if (_82545EM_read_phy_reg(ethptr, E1000_PHY_AUTONEG_ADV, 
			       &mii_autoneg_adv_reg) != OK)
		return SYSERR;
	printf("ok2\n");

    	if (_82545EM_read_phy_reg(ethptr, E1000_PHY_1000T_CTRL, 
			       &mii_1000t_ctrl_reg) != OK)
		return SYSERR;
	printf("ok2\n");

	mii_autoneg_adv_reg |= (E1000_NWAY_AR_100TX_FD_CAPS |
				E1000_NWAY_AR_100TX_HD_CAPS |
				E1000_NWAY_AR_10T_FD_CAPS   |
				E1000_NWAY_AR_10T_HD_CAPS);
	mii_1000t_ctrl_reg &= ~E1000_CR_1000T_HD_CAPS;
	mii_1000t_ctrl_reg |= E1000_CR_1000T_FD_CAPS;

	mii_autoneg_adv_reg &= ~(E1000_NWAY_AR_ASM_DIR | 
				 E1000_NWAY_AR_PAUSE);

	if (_82545EM_write_phy_reg(ethptr, E1000_PHY_AUTONEG_ADV, 
				mii_autoneg_adv_reg) != OK)
		return SYSERR;
	printf("ok2\n");

	if (_82545EM_write_phy_reg(ethptr, E1000_PHY_1000T_CTRL, 
				mii_1000t_ctrl_reg) != OK)
		return SYSERR;
	printf("ok2\n");

	/* Restart auto-negotiation. */

	if (_82545EM_read_phy_reg(ethptr, E1000_PHY_CONTROL, 
				  &phy_ctrl) != OK)
		return SYSERR;
	printf("ok2\n");

    	phy_ctrl |= (E1000_MII_CR_AUTO_NEG_EN | 
		     E1000_MII_CR_RESTART_AUTO_NEG);

	if (_82545EM_write_phy_reg(ethptr, E1000_PHY_CONTROL, 
				   phy_ctrl) != OK)
		return SYSERR;
	printf("ok2\n");

	/* Wait for auto-negotiation to complete */

	for (;;) {
		if (_82545EM_read_phy_reg(ethptr, E1000_PHY_STATUS, 
				       &phy_status) != OK)
			DELAY(10);
		    
		if (_82545EM_read_phy_reg(ethptr, E1000_PHY_STATUS, 
				       &phy_status) != OK)
			return SYSERR;
	printf("ok2\n");
		
		if ((phy_status & E1000_MII_SR_LINK_STATUS) && 
		    (phy_status & E1000_MII_SR_AUTONEG_COMPLETE)) {
			break;
		}
		
		MDELAY(100);
	}

	ctrl = eth_io_readl(ethptr->iobase, E1000_CTRL);
	ctrl &= (~(E1000_CTRL_TFCE | E1000_CTRL_RFCE));
	eth_io_writel(ethptr->iobase, E1000_CTRL, ctrl);

	return OK;
}

/*------------------------------------------------------------------------
 * _82545EM_configure_rx - Configure Receive Unit after Reset
 *------------------------------------------------------------------------
 */
local void _82545EM_configure_rx(
	struct	ethcblk	*ethptr
	)
{
	uint32 rctl, rxcsum;

	rctl = eth_io_readl(ethptr->iobase, E1000_RCTL);

	/* Disable receiver while configuring. */

	eth_io_writel(ethptr->iobase, E1000_RCTL, rctl & ~E1000_RCTL_EN);

	/* Enable receiver, accept broadcast packets, no loopback, and 	*/
	/* 	free buffer threshold is set to 1/2 RDLEN. 		*/

	rctl &= ~(3 << E1000_RCTL_MO_SHIFT);
	rctl |= E1000_RCTL_EN 	  | 
		E1000_RCTL_BAM 	  |
		E1000_RCTL_LBM_NO |
		E1000_RCTL_RDMTS_HALF;

	/* Do not store bad packets, do not pass MAC control frame, 	*/
	/* 	disable long packet receive and CRC strip 		*/
	
	rctl &= ~(E1000_RCTL_SBP   |
		  E1000_RCTL_LPE   |
		  E1000_RCTL_SECRC |
		  E1000_RCTL_PMCF);
	
	/* Setup buffer sizes */

	rctl &= ~(E1000_RCTL_BSEX |
		  E1000_RCTL_SZ_4096);
	rctl |= E1000_RCTL_SZ_2048;

	/* Set the Receive Delay Timer Register, let driver be notified */
	/* 	immediately each time a new packet has been stored in 	*/
	/* 	memory 							*/

	eth_io_writel(ethptr->iobase, E1000_RDTR, E1000_RDTR_DEFAULT);
	eth_io_writel(ethptr->iobase, E1000_RADV, E1000_RADV_DEFAULT);

	/* IRQ moderation */

	eth_io_writel(ethptr->iobase, E1000_ITR, 
			1000000000 / (E1000_ITR_DEFAULT * 256));

	/* Setup the HW Rx Head and Tail Descriptor Pointers, the Base 	*/
	/* 	and Length of the Rx Descriptor Ring 			*/

	eth_io_writel(ethptr->iobase, E1000_RDBAL(0), 
			(uint32)ethptr->rxRing);
	eth_io_writel(ethptr->iobase, E1000_RDBAH(0), 0);
	eth_io_writel(ethptr->iobase, E1000_RDLEN(0), 
			E1000_RDSIZE * ethptr->rxRingSize);
	eth_io_writel(ethptr->iobase, E1000_RDH(0), 0);
	eth_io_writel(ethptr->iobase, E1000_RDT(0), 
			ethptr->rxRingSize - E1000_RING_BOUNDARY);

	/* Disable Receive Checksum Offload for IPv4, TCP and UDP. */

	rxcsum = eth_io_readl(ethptr->iobase, E1000_RXCSUM);
	rxcsum &= ~(E1000_RXCSUM_TUOFL | E1000_RXCSUM_IPOFL);
	eth_io_writel(ethptr->iobase, E1000_RXCSUM, rxcsum);

	/* Enable receiver. */

	eth_io_writel(ethptr->iobase, E1000_RCTL, rctl);
}

/*------------------------------------------------------------------------
 * _82545EM_configure_tx - Configure Transmit Unit after Reset
 *------------------------------------------------------------------------
 */
local void _82545EM_configure_tx(
	struct	ethcblk	*ethptr
	)
{
	uint32 	tctl, tipg, txdctl;
	uint32 	ipgr1, ipgr2;

	/* Set the transmit descriptor write-back policy for both queues */

	txdctl = eth_io_readl(ethptr->iobase, E1000_TXDCTL(0));
	txdctl &= ~E1000_TXDCTL_WTHRESH;
	txdctl |= E1000_TXDCTL_GRAN;
	eth_io_writel(ethptr->iobase, E1000_TXDCTL(0), txdctl);

	/* Program the Transmit Control Register */
	
	tctl = eth_io_readl(ethptr->iobase, E1000_TCTL);
	tctl &= ~E1000_TCTL_CT;
	tctl |= E1000_TCTL_RTLC |
		E1000_TCTL_EN 	|
		E1000_TCTL_PSP 	|
		(E1000_COLLISION_THRESHOLD << E1000_CT_SHIFT);
	tctl &= ~E1000_TCTL_COLD;
	tctl |= E1000_COLLISION_DISTANCE << E1000_COLD_SHIFT;

	/* Set the default values for the Tx Inter Packet Gap timer */
	
	tipg = E1000_TIPG_IPGT_COPPER_DEFAULT;
	ipgr1 = E1000_TIPG_IPGR1_DEFAULT;
	ipgr2 = E1000_TIPG_IPGR2_DEFAULT;
	tipg |= ipgr1 << E1000_TIPG_IPGR1_SHIFT;
	tipg |= ipgr2 << E1000_TIPG_IPGR2_SHIFT;
	eth_io_writel(ethptr->iobase, E1000_TIPG, tipg);

	/* Set the Tx Interrupt Delay register */
	
	eth_io_writel(ethptr->iobase, E1000_TIDV, E1000_TIDV_DEFAULT);
	eth_io_writel(ethptr->iobase, E1000_TADV, E1000_TADV_DEFAULT);

	/* Setup the HW Tx Head and Tail descriptor pointers */
	
	eth_io_writel(ethptr->iobase, E1000_TDBAL(0), 
			(uint32)ethptr->txRing);
	eth_io_writel(ethptr->iobase, E1000_TDBAH(0), 0);
	eth_io_writel(ethptr->iobase, E1000_TDLEN(0), 
			E1000_TDSIZE * ethptr->txRingSize);
	eth_io_writel(ethptr->iobase, E1000_TDH(0), 0);
	eth_io_writel(ethptr->iobase, E1000_TDT(0), 0);

	eth_io_writel(ethptr->iobase, E1000_TCTL, tctl);
}

/*------------------------------------------------------------------------
 * _82545EM_read_phy_reg - Read MDI control register
 *------------------------------------------------------------------------
 */
status _82545EM_read_phy_reg(
	struct	ethcblk	*ethptr,
	uint32 	offset,
	uint16 	*data
	)
{
	uint32 i, mdic = 0;

	if (offset > E1000_MAX_PHY_REG_ADDRESS) {
		return SYSERR;
	}

	mdic = ((offset << E1000_MDIC_REG_SHIFT) |
		(E1000_82545EM_MDIC_PHY_ADDR << E1000_MDIC_PHY_SHIFT) |
		(E1000_MDIC_OP_READ));

	eth_io_writel(ethptr->iobase, E1000_MDIC, mdic);

	for (i = 0; i < (E1000_GEN_POLL_TIMEOUT * 3); i++) {
		DELAY(50);
		mdic = eth_io_readl(ethptr->iobase, E1000_MDIC);
		if (mdic & E1000_MDIC_READY)
			break;
	}
	if (!(mdic & E1000_MDIC_READY)) {
		return SYSERR;
	}
	if (mdic & E1000_MDIC_ERROR) {
		return SYSERR;
	}
	*data = (uint16) mdic;

	return OK;
}

/*------------------------------------------------------------------------
 *  _82545EM_write_phy_reg - Write MDI control register
 *------------------------------------------------------------------------
 */
status _82545EM_write_phy_reg(
	struct	ethcblk	*ethptr,
	uint32 	offset,
	uint16 	data
	)
{
	uint32 i, mdic = 0;

	if (offset > E1000_MAX_PHY_REG_ADDRESS) {
		return SYSERR;
	}

	mdic = ( ((uint32)data) |
		 (offset << E1000_MDIC_REG_SHIFT) |
		 (E1000_82545EM_MDIC_PHY_ADDR << E1000_MDIC_PHY_SHIFT) |
		 (E1000_MDIC_OP_WRITE) );

	eth_io_writel(ethptr->iobase, E1000_MDIC, mdic);

	for (i = 0; i < (E1000_GEN_POLL_TIMEOUT * 3); i++) {
		DELAY(50);
		mdic = eth_io_readl(ethptr->iobase, E1000_MDIC);
		if (mdic & E1000_MDIC_READY)
			break;
	}
	if (!(mdic & E1000_MDIC_READY)) {
		return SYSERR;
	}
	if (mdic & E1000_MDIC_ERROR) {
		return SYSERR;
	}

	return OK;
}
