/*
 * pci.h
 */

#define PCI_MAKE_ID(bus, dev, func)     ((bus) << 16) | ((dev) << 11) | ((func) << 8)
#define PCI_CONFIG_VENDOR_ID            0x00
#define PCI_CONFIG_DEVICE_ID            0x02
#define PCI_CONFIG_HEADER_TYPE          0x0e

#define PCI_CONFIG_PCI_IOBASE           0x10
#define PCI_CONFIG_PCI_IRQ              0x3C
#define PCI_CONFIG_PCI_COMMAND          0x04

#define PCI_CMD_MASK	                0x0005

// I/O Ports
#define PCI_CONFIG_ADDR                 0xcf8
#define PCI_CONFIG_DATA                 0xcfC

#define PCI_MAX_BUSES			256
#define PCI_DEVICES_PER_BUS		32
#define PCI_FUNCTIONS_PER_DEVICE	8
#define PCI_BUS_MASK			(PCI_MAX_BUSES - 1)
#define PCI_DEV_MASK			(PCI_DEVICES_PER_BUS - 1)
#define PCI_FUNC_MASK			(PCI_FUNCTIONS_PER_DEVICE - 1)
#define PCIE_BYTES_PER_FUNCTION		0x1000
#define PCIE_BYTES_PER_DEVICE		(PCIE_BYTES_PER_FUNCTION *	\
					 PCI_FUNCTIONS_PER_DEVICE)
#define PCIE_BYTES_PER_BUS		(PCIE_BYTES_PER_DEVICE *	\
					 PCI_DEVICES_PER_BUS)

#define PCI_BARS_PER_HEADER		6	/* PCI BARs per header */

#define PCI_HEADER_TYPE0_SIZE		0x40	    /* Size of type0 header */
#define	PCI_HDR_TYPE_MULTIFCN		(1 << 7)    /* Multifunction device */

#define PCI_BUS_SHIFT			    16	/* Bits to shift bus */
#define PCI_DEV_SHIFT			    3	/* Bits to shift dev */

/* Definitions of PCI base address register (bar) bits:*/
#define PCI_BAR_REGION_TYPE_IO_MASK	1	/* 0=mem space, 1=I/O */
#define PCI_BAR_LOCATABLE_1MB_MASK	2	/* 1=Must be < 1MB */
#define PCI_BAR_LOCATABLE_64B_MASK	4	/* 1=Any 64-bit phys */
#define PCI_BAR_LOCATABLE_PREFETCH	8	/* 1=Prefetchable */
#define PCI_BAR_BASE_ADDRESS_MASK	0xfffffff0/* mask: MMIO base */

#ifndef offsetof
#define offsetof(type, member) ((int)((unsigned int)&((type *)0)->member))
#endif

/* Define the layout of PCI config headers (type 0) */

struct __attribute__ ((__packed__)) pci_config_header {
	/*	      PCI header field			Byte offset */
	unsigned short	vendorID;				/* 0x0 */
	unsigned short	deviceID;				/*   2 */
	unsigned short	command;				/*   4 */
	unsigned short	status;					/*   6 */
	unsigned char	revID;					/*   8 */
	unsigned char	progIF;					/*   9 */
	unsigned char	subClass;				/*   a */
	unsigned char	classCode;				/*   b */
	unsigned char	clSize;					/*   c */
	unsigned char	latencyTimer;				/*   d */
	unsigned char	headerType;				/*   e */
	unsigned char	BIST;					/*   f */
	unsigned int	bars[PCI_BARS_PER_HEADER];		/*   10 */
	unsigned int	cardbusCISPtr;				/*   28 */
	unsigned short	subsystemVendorID;			/*   2c */
	unsigned short	subsystemID;				/*   2e */
	unsigned int	expansionROMAddr;			/*   30 */
	unsigned char	capabilitiesPtr;			/*   34 */
	unsigned char	reserved0;				/*   35 */
	unsigned char	reserved1;				/*   36 */
	unsigned char	reserved2;				/*   37 */
	unsigned int	reserved3;				/*   38 */
	unsigned char	interruptLine;				/*   3c */
	unsigned char	interruptPin;				/*   3d */
	unsigned char	minGrant;				/*   3e */
	unsigned char	maxLatency;				/*   3f */
};

