/* e1000.h - information for Intel 82545EM NIC (a form of E1000E) */

#define INTEL_VENDOR_ID		0x8086
#define INTEL_82545EM_DEVICE_ID	0x100F

/* PCI Configuration Registers */

#define E1000_PCI_COMMAND 	0x04
#define E1000_PCI_STATUS 	0x06
#define E1000_PCI_MEMBASE 	0x10
#define E1000_PCI_FLASHBASE 	0x14
#define E1000_PCI_IOBASE 	0x18
#define E1000_PCI_IRQ 		0x3C

#define E1000_PCI_CMD_MASK	0x0005

#define E1000_IO_IOADDR		0x0000
#define E1000_IO_IODATA		0x0004

#define E1000_RX_RING_SIZE 	32
#define E1000_TX_RING_SIZE 	16
#define E1000_RING_BOUNDARY 	16

#ifdef X86_QEMU
#define eth_dev_writel(b,r,v) eth_mmio_writel(b,r,v)
#define eth_dev_readl(b,r) eth_mmio_readl(b,r)
#define eth_dev_flush(b) eth_mmio_flush(b)
#else
#define eth_dev_writel(b,r,v) eth_io_writel(b,r,v)
#define eth_dev_readl(b,r) eth_io_readl(b,r)
#define eth_dev_flush(b) eth_io_flush(b)
#endif

/* Receive Descriptor */

struct	eth_rx_desc {
	uint64  buffer_addr;		/* Address of the descriptor's 	*/
					/*  data buffer 		*/
	uint16  length;			/* Length of data DMAed into 	*/
					/*  data buffer 		*/
	uint16  csum;		   	/* Packet checksum 		*/
	uint8   status;		 	/* Descriptor status 		*/
	uint8   errors;		 	/* Descriptor Errors 		*/
	uint16  special;
};

/* Transmit Descriptor */

struct	eth_tx_desc {
	uint64  buffer_addr;		/* Address of the descriptor's  */
					/*  data buffer 		*/
	union {
	 uint32 data;
	 struct {
	  uint16 	length; 	/* Data buffer length 		*/
	  uint8 	cso;		/* Checksum offset 		*/
	  uint8 	cmd;		/* Descriptor control 		*/
	 } flags;
	} lower;
	union {
	 uint32 data;
	 struct {
	  uint8 	status; 	/* Descriptor status 		*/
	  uint8 	css;		/* Checksum start 		*/
	  uint16 	special;
	 } fields;
	} upper;
};

#define E1000_RDSIZE 		sizeof(struct 	eth_rx_desc)
#define E1000_TDSIZE 		sizeof(struct 	eth_tx_desc)

static 	inline 	void 	eth_mmio_writel(
	uint32 	iobase,
	uint32 	reg,
	uint32 	val
	)
{
	*(int32*)(iobase+reg) = (val);
}

static 	inline 	uint32 	eth_mmio_readl(
	uint32 	iobase,
	uint32 	reg
	)
{
	return *(int32*)(iobase+reg);
}

static 	inline 	void 	eth_io_writel(
	uint32 	iobase,
	uint32 	reg,
	uint32 	val
	)
{
	outl(iobase + E1000_IO_IOADDR, reg);
	outl(iobase + E1000_IO_IODATA, val);
}

static 	inline 	uint32 	eth_io_readl(
	uint32 	iobase,
	uint32 	reg
	)
{
	outl(iobase + E1000_IO_IOADDR, reg);
	return inl(iobase + E1000_IO_IODATA);
}

#define eth_mmio_flush(iobase)                                          \
	eth_mmio_readl((iobase), E1000_STATUS);

#define eth_io_flush(iobase)  						\
	eth_io_readl((iobase), E1000_STATUS);

#define ADDR_BIT_MASK   		0xFFFFFF	

/* Register Defination */

#define E1000_CTRL 		0x00000 /* Device Control 		*/
#define E1000_STATUS 		0x00008 /* Device Status 		*/
#define E1000_CTRL_EXT 		0x00018 /* Extended Device Control 	*/
#define E1000_MDIC 		0x00020 /* MDI Control 			*/
#define E1000_ICR 		0x000C0 /* Interrupt Cause Read 	*/
#define E1000_ITR 		0x000C4 /* Interrupt Throttling Rate 	*/
#define E1000_ICS 		0x000C8 /* Interrupt Cause Set 		*/
#define E1000_IMS 		0x000D0 /* Interrupt Mask Set 		*/
#define E1000_IMC 		0x000D8 /* Interrupt Mask Clear 	*/
#define E1000_RCTL 		0x00100 /* Rx Control 			*/
#define E1000_TCTL 		0x00400 /* Tx Control 			*/
#define E1000_TIPG 		0x00410 /* Tx Inter-packet gap 		*/
#define E1000_EXTCNF_CTRL 	0x00F00 /* Extended Configuration 	*/
					/* 	Control 		*/
#define E1000_TBT 		0x00448 /* Tx Burst Timer 		*/
#define E1000_AIT 		0x00458 /* Adaptive Interframe Spacing 	*/
					/* 	Throttle 		*/
#define E1000_PBA 		0x01000 /* Packet Buffer Allocation 	*/
#define E1000_RXCSUM 		0x05000 /* Rx Checksum Control 		*/
#define E1000_RFCTL 		0x05008 /* Receive Filter Control 	*/
#define E1000_RDTR 		0x02820 /* Rx Delay Timer 		*/
#define E1000_RADV 		0x0282C /* Rx Interrupt Absolute Delay  */
					/* 	Timer 			*/
#define E1000_TIDV 		0x03820 /* Tx Interrupt Delay Value 	*/
#define E1000_TADV 		0x0382C /* Tx Interrupt Absolute Delay  */
					/* 	Val 			*/
#define E1000_MTA 		0x05200 /* Multicast Table Array 	*/
#define E1000_KMRNCTRLSTA 	0x00034 /* MAC-PHY interface 		*/
#define E1000_GCR 		0x05B00 /* PCI-Ex Control		*/
#define E1000_FWSM 		0x05B54 /* FW Semaphore 		*/
#define E1000_KABGTXD 		0x03004 /* AFE Band Gap Transmit Ref 	*/
					/* 	Data */

#define E1000_RDBAL(_n) 	((_n) < 4 ? (0x02800 + ((_n) * 0x100)) : \
					(0x0C000 + ((_n) * 0x40)))
#define E1000_RDBAH(_n) 	((_n) < 4 ? (0x02804 + ((_n) * 0x100)) : \
					(0x0C004 + ((_n) * 0x40)))
#define E1000_RDLEN(_n) 	((_n) < 4 ? (0x02808 + ((_n) * 0x100)) : \
					(0x0C008 + ((_n) * 0x40)))
#define E1000_RDH(_n) 		((_n) < 4 ? (0x02810 + ((_n) * 0x100)) : \
					(0x0C010 + ((_n) * 0x40)))
#define E1000_RDT(_n) 		((_n) < 4 ? (0x02818 + ((_n) * 0x100)) : \
					(0x0C018 + ((_n) * 0x40)))
#define E1000_TDBAL(_n) 	((_n) < 4 ? (0x03800 + ((_n) * 0x100)) : \
					(0x0E000 + ((_n) * 0x40)))
#define E1000_TDBAH(_n) 	((_n) < 4 ? (0x03804 + ((_n) * 0x100)) : \
					(0x0E004 + ((_n) * 0x40)))
#define E1000_TDLEN(_n) 	((_n) < 4 ? (0x03808 + ((_n) * 0x100)) : \
					(0x0E008 + ((_n) * 0x40)))
#define E1000_TDH(_n) 		((_n) < 4 ? (0x03810 + ((_n) * 0x100)) : \
					(0x0E010 + ((_n) * 0x40)))
#define E1000_TDT(_n) 		((_n) < 4 ? (0x03818 + ((_n) * 0x100)) : \
					(0x0E018 + ((_n) * 0x40)))
#define E1000_TXDCTL(_n) 	((_n) < 4 ? (0x03828 + ((_n) * 0x100)) : \
					(0x0E028 + ((_n) * 0x40)))
#define E1000_TARC(_n)		(0x03840 + ((_n) * 0x100))
#define E1000_RAL(_i) 		(((_i) <= 15) ? (0x05400 + ((_i) * 8)) : \
					(0x054E0 + ((_i - 16) * 8)))
#define E1000_RAH(_i) 		(((_i) <= 15) ? (0x05404 + ((_i) * 8)) : \
					(0x054E4 + ((_i - 16) * 8)))

/* Receive Descriptor bit definitions */

#define E1000_RXD_STAT_DD 	0x01    /* Descriptor Done */
#define E1000_RXD_STAT_EOP 	0x02    /* End of Packet */

/* Receive Control */

#define E1000_RCTL_RST 		0x00000001 	/* software reset 	*/
#define E1000_RCTL_EN 		0x00000002 	/* enable 		*/
#define E1000_RCTL_SBP 		0x00000004 	/* store bad packet 	*/
#define E1000_RCTL_UPE 		0x00000008 	/* unicast promisc 	*/
						/* 	enable 		*/
#define E1000_RCTL_MPE 		0x00000010 	/* multicast promisc 	*/
						/* 	enable 		*/
#define E1000_RCTL_LPE 		0x00000020 	/* long packet enable 	*/
#define E1000_RCTL_LBM_NO 	0x00000000 	/* no loopback mode 	*/
#define E1000_RCTL_DTYP_MASK 	0x00000C00 	/* descriptor type mask */
#define E1000_RCTL_RDMTS_HALF 	0x00000000 	/* rx desc min thresh 	*/
						/* 	size 		*/
#define E1000_RCTL_MO_SHIFT 	12 		/* multicast offset shift*/
#define E1000_RCTL_MO_0 	0x00000000 	/* multicast offset 11:0*/
#define E1000_RCTL_MO_1 	0x00001000 	/* multicast offset 12:1*/
#define E1000_RCTL_MO_2 	0x00002000 	/* multicast offset 13:2*/
#define E1000_RCTL_MO_3 	0x00003000 	/* multicast offset 15:4*/
#define E1000_RCTL_MDR 		0x00004000 	/* multicast desc ring 0*/
#define E1000_RCTL_BAM 		0x00008000 	/* broadcast enable 	*/
#define E1000_RCTL_SZ_2048 	0x00000000 	/* rx buffer size 2048 	*/
#define E1000_RCTL_SZ_4096 	0x00030000 	/* rx buffer size 4096 	*/
#define E1000_RCTL_DPF 		0x00400000 	/* discard pause frames */
#define E1000_RCTL_PMCF 	0x00800000 	/* pass MAC control 	*/
						/* 	frames 		*/
#define E1000_RCTL_BSEX 	0x02000000 	/* buffer size extension*/
#define E1000_RCTL_SECRC 	0x04000000 	/* strip Ethernet CRC 	*/
#define E1000_RCTL_FLXBUF_MASK 0x78000000 	/* Flexible buffer size */


/* Device Control */

#define E1000_CTRL_GIO_MASTER_DISABLE 0x00000004
						/*Blocks new Master reqs */
#define E1000_CTRL_SLU		0x00000040	/* set link up 		*/
#define E1000_CTRL_FRCSPD	0x00000800	/* force speed 		*/
#define E1000_CTRL_FRCDPX	0x00001000	/* force duplex 	*/
#define E1000_CTRL_RST		0x04000000	/* global reset 	*/
#define E1000_CTRL_RFCE 	0x08000000	/* receive flow control */
						/* 	enable 		*/
#define E1000_CTRL_TFCE 	0x10000000	/* transmit flow control*/
						/* 	enable 		*/
#define E1000_CTRL_PHY_RST 	0x80000000 	/* PHY reset 		*/

/* Device Status */

#define E1000_STATUS_LAN_INIT_DONE 	0x00000200 
					/* Lan Init Completion by NVM 	*/
#define E1000_STATUS_PHYRA 		0x00000400      
					/* PHY Reset Asserted 		*/
#define E1000_STATUS_GIO_MASTER_ENABLE 	0x00080000 
					/* Master request status 	*/

/* Transmit Descriptor bit definitions */

#define E1000_TXD_CMD_EOP 	0x01000000 	/* end of packet 	*/
#define E1000_TXD_CMD_IFCS 	0x02000000 	/* insert FCS 		*/
#define E1000_TXD_CMD_IC 	0x04000000 	/* insert checksum 	*/
#define E1000_TXD_CMD_RS 	0x08000000 	/* report status 	*/
#define E1000_TXD_CMD_DEXT 	0x20000000 	/* descriptor extension */
#define E1000_TXD_CMD_IDE 	0x80000000 	/* enable Tidv register */
#define E1000_TXD_STAT_DD 	0x00000001 	/* descriptor done 	*/

/* Transmit Control */

#define E1000_TCTL_RST 		0x00000001 	/* software reset 	*/
#define E1000_TCTL_EN 		0x00000002 	/* enable Tx 		*/
#define E1000_TCTL_PSP 		0x00000008 	/* pad short packets 	*/
#define E1000_TCTL_CT 		0x00000ff0 	/* collision threshold 	*/
#define E1000_TCTL_COLD 	0x003ff000 	/* collision distance 	*/
#define E1000_TCTL_RTLC 	0x01000000 	/* re-transmit on late 	*/

/* Receive Checksum Control */

#define E1000_RXCSUM_IPOFL 	0x00000100 	/* IPv4 checksum offload*/
#define E1000_RXCSUM_TUOFL 	0x00000200 	/* TCP/UDP checksum 	*/
						/* 	offload 	*/

/* Header split receive */

#define E1000_RFCTL_NFSW_DIS 	0x00000040
#define E1000_RFCTL_NFSR_DIS 	0x00000080

/* Extended Configuration Control and Size */

#define E1000_EXTCNF_CTRL_SWFLAG 0x00000020
#define E1000_KABGTXD_BGSQLBIAS 0x00050000

/* Collision related configuration parameters */

#define E1000_COLLISION_THRESHOLD 	15
#define E1000_CT_SHIFT 			4
#define E1000_COLLISION_DISTANCE 	63
#define E1000_COLD_SHIFT 		12

/* Default values for the transmit IPG register */

#define E1000_TIPG_IPGT_COPPER_DEFAULT	8
#define E1000_TIPG_IPGR1_DEFAULT	8
#define E1000_TIPG_IPGR1_SHIFT		10
#define E1000_TIPG_IPGR2_DEFAULT 	6
#define E1000_TIPG_IPGR2_SHIFT		20

#define E1000_PBA_10K 		0x000A 		/* PBA constants - 10KB */
#define E1000_PBA_48K 		0x0030    	/* PBA constants - 48KB */

/* Interrupt Cause Read */

#define E1000_ICR_TXDW 		0x00000001 	/* transmit desc written*/
						/* 	back 		*/
#define E1000_ICR_LSC 		0x00000004 	/* Link status change 	*/
#define E1000_ICR_RXSEQ 	0x00000008 	/* rx sequence error 	*/
#define E1000_ICR_RXO 		0x00000040 	/* rx overrun 		*/
#define E1000_ICR_RXT0 		0x00000080 	/* rx timer intr 	*/

/* Interrupt Mask Set */

#define E1000_IMS_TXDW 		E1000_ICR_TXDW 	/* Tx desc written back */
#define E1000_IMS_LSC 		E1000_ICR_LSC 	/* Link Status Change 	*/
#define E1000_IMS_RXSEQ 	E1000_ICR_RXSEQ /* Rx sequence error 	*/
#define E1000_IMS_RXO 		E1000_ICR_RXO 	/* Rx overrun 		*/
#define E1000_IMS_RXT0 		E1000_ICR_RXT0 	/* Rx timer intr 	*/

#define E1000_IMS_ENABLE_MASK ( E1000_IMS_TXDW |	\
				E1000_IMS_LSC 	|	\
				E1000_IMS_RXSEQ|	\
				E1000_IMS_RXT0 |	\
				E1000_IMS_RXO)

/* Transmit Descriptor Control */

#define E1000_TXDCTL_WTHRESH 	0x003F0000 	/* TXDCTL writeback 	*/
						/* 	threshold 	*/
#define E1000_TXDCTL_GRAN 	0x01000000 	/* TXDCTL granularity 	*/
#define E1000_TXDCTL_MAX_TX_DESC_PREFETCH 0x0100001F /*GRAN=1,PTHRESH=31*/

#define E1000_RAL_MAC_ADDR_LEN 		4
#define E1000_RAH_MAC_ADDR_LEN 		2
#define E1000_RAH_AV 		0x80000000 	/* receive descriptor 	*/
						/* 	valid 		*/

/* PCI Express Control */

#define E1000_GCR_RXD_NO_SNOOP		0x00000001
#define E1000_GCR_RXDSCW_NO_SNOOP	0x00000002
#define E1000_GCR_RXDSCR_NO_SNOOP	0x00000004
#define E1000_GCR_TXD_NO_SNOOP		0x00000008
#define E1000_GCR_TXDSCW_NO_SNOOP	0x00000010
#define E1000_GCR_TXDSCR_NO_SNOOP	0x00000020

#define E1000_PCIE_NO_SNOOP_ALL (					\
				 E1000_GCR_RXD_NO_SNOOP 	|	\
				 E1000_GCR_RXDSCW_NO_SNOOP 	| 	\
				 E1000_GCR_RXDSCR_NO_SNOOP 	| 	\
				 E1000_GCR_TXD_NO_SNOOP 	| 	\
				 E1000_GCR_TXDSCW_NO_SNOOP 	| 	\
				 E1000_GCR_TXDSCR_NO_SNOOP)

/* PHY Control Register */
#define E1000_MII_CR_RESTART_AUTO_NEG 	0x0200	/* restart auto 	*/
						/* 	negotiation 	*/
#define E1000_MII_CR_AUTO_NEG_EN 	0x1000 	/* auto neg enable 	*/
#define E1000_MII_CR_RESET 		0x8000	/* 0=normal, 1=PHY reset*/

/* PHY Status Register */

#define E1000_MII_SR_LINK_STATUS	0x0004	/* link status 1 = link	*/
#define E1000_MII_SR_AUTONEG_COMPLETE	0x0020	/* Auto Neg Complete 	*/

/* Autoneg Advertisement Register */

#define E1000_NWAY_AR_10T_HD_CAPS 	0x0020 	/* 10T   Half Duplex 	*/
#define E1000_NWAY_AR_10T_FD_CAPS 	0x0040 	/* 10T   Full Duplex 	*/
#define E1000_NWAY_AR_100TX_HD_CAPS 	0x0080 	/* 100TX Half Duplex 	*/
#define E1000_NWAY_AR_100TX_FD_CAPS 	0x0100 	/* 100TX Full Duplex 	*/
#define E1000_NWAY_AR_PAUSE 		0x0400 	/* pause operation 	*/
						/* 	desired 	*/
#define E1000_NWAY_AR_ASM_DIR 		0x0800 	/* asymmetric pause 	*/
						/* 	direction bit 	*/
#define E1000_CR_1000T_HD_CAPS 		0x0100 	/* advertise 1000T HD 	*/
#define E1000_CR_1000T_FD_CAPS 		0x0200 	/* advertise 1000T FD 	*/


/* PHY 1000 MII Register/Bit Definitions */

#define E1000_PHY_CONTROL 		0x00	/* control register 	*/
#define E1000_PHY_STATUS 		0x01	/* status register 	*/
#define E1000_PHY_AUTONEG_ADV 		0x04	/* autoneg advertisement*/
#define E1000_PHY_1000T_CTRL 		0x09	/*1000Base-T control reg*/
#define E1000_PHY_PAGE_SELECT 		0x15 	/* Page Address Reg 	*/

#define E1000_PHY_PAGE_SHIFT 		5
#define E1000_MAX_PHY_REG_ADDRESS 	0x1F 	/* 5 bit address bus 	*/
#define E1000_MAX_PHY_MULTI_PAGE_REG 	0xF

/* Timeout definitions */

#define E1000_PHY_CFG_TIMEOUT           100
#define E1000_MASTER_DISABLE_TIMEOUT 	800
#define E1000_SW_FLAG_TIMEOUT 		1000 

/* M88E1000 Specific Registers */

#define M88E1000_PHY_SPEC_CTRL 		0x10 	/* PHY specific control */
						/* 	register 	*/
#define M88E1000_PHY_SPEC_STATUS 	0x11 	/* PHY specific status 	*/
						/* 	register 	*/
#define M88E1000_PSCR_POLARITY_REVERSAL 0x0002 	/* 1=polarity reverse 	*/
						/* 	enabled 	*/
#define M88E1000_PSCR_AUTO_X_MODE 	0x0060

/* BME1000 PHY Specific Control Register */

#define E1000_BM_PSCR_ENABLE_DOWNSHIFT 	0x0800  /* 1 = enable downshift */
#define E1000_BM_PHY_PAGE_SELECT 	22 	/* Page Select for BM 	*/


/* MDI Control */

#define E1000_MDIC_REG_SHIFT 	16
#define E1000_MDIC_PHY_MASK 	0x03E00000
#define E1000_MDIC_PHY_SHIFT 	21
#define E1000_MDIC_OP_WRITE 	0x04000000
#define E1000_MDIC_OP_READ 	0x08000000
#define E1000_MDIC_READY 	0x10000000
#define E1000_MDIC_ERROR 	0x40000000

/* SerDes Control */

#define E1000_GEN_POLL_TIMEOUT 640

#define E1000_RDTR_DEFAULT	0       /* receive interrupt delay 	*/
#define E1000_RADV_DEFAULT	8       /* receive Absolute interrupt 	*/
					/* 	delay 			*/
#define E1000_TIDV_DEFAULT	8       /* transmit interrupt delay 	*/
#define E1000_TADV_DEFAULT	32 	/* transmit absolute interrupt 	*/
					/* 	delay 			*/
#define E1000_ITR_DEFAULT	20000   /* interrupt throttling rate 	*/

#define E1000_ICH_FWSM_RSPCIPHY 	0x00000040 
#define E1000_ICH_LAN_INIT_TIMEOUT 	1500

#define E1000_KMRNCTRLSTA_OFFSET 	0x001F0000
#define E1000_KMRNCTRLSTA_OFFSET_SHIFT 16
#define E1000_KMRNCTRLSTA_REN          0x00200000
#define E1000_KMRNCTRLSTA_TIMEOUTS 	0x4 /* Kumeran Timeouts 	*/
#define E1000_KMRNCTRLSTA_INBAND_PARAM 0x9  /* Kumeran InBand Parameters*/


/* 82545EM specific definitions */

#define E1000_82545EM_MDIC_PHY_ADDR 	1
#define E1000_82545EM_RAR_ENTRIES     	15
#define E1000_82545EM_MTA_ENTRIES 	128

/* 82567LM specific definitions */

#define E1000_82567LM_MDIC_PHY_ADDR  	2
#define E1000_82567LM_RAR_ENTRIES 	7
#define E1000_82567LM_MTA_ENTRIES 	32
