/* pci.c - pci_init */

#include <xinu.h>

/*------------------------------------------------------------------------
 * pci_init - initialize the PCI bios structures and such
 *------------------------------------------------------------------------
 */
int32	pci_init(void)
{
	return OK;
}

/*------------------------------------------------------------------------
 * pci_read_config_byte  -  Read a byte from device's configuration space
 *------------------------------------------------------------------------
 */
int32 pci_read_config_byte(
		uint32	encodedDev,	/* Encoded PCI device		*/
		int	offset,		/* Offset in config space	*/
		byte	*value		/* Pointer to store value	*/
		)
{
    uint32 addr;

    addr = 0x80000000 | encodedDev | (offset & 0xfc);
    outl(PCI_CONFIG_ADDR, addr);
	*value = inb(PCI_CONFIG_DATA + (offset & 0x03));
	return OK;
}

/*------------------------------------------------------------------------
 * pci_read_config_word  -  Read a word from device's configuration space
 *------------------------------------------------------------------------
 */
int pci_read_config_word(
		uint32	encodedDev,	/* Encoded PCI device		*/
		int32	offset,		/* Offset in config space	*/
		uint16	*value		/* Pointer to store value	*/
		)
{
    uint32 addr;

    addr = 0x80000000 | encodedDev | (offset & 0xfc);
    outl(PCI_CONFIG_ADDR, addr);
	*value = inw(PCI_CONFIG_DATA + (offset & 0x02));
	return OK;
}

/*------------------------------------------------------------------------
 * pci_read_config_dword  -  Read a dword from device's config space
 *------------------------------------------------------------------------
 */
int pci_read_config_dword(
		uint32	encodedDev,	/* Encoded PCI device		*/
		int32	offset,		/* Offset in config space	*/
		uint32	*value		/* Pointer to store value	*/
		)
{
    uint32 addr;

    addr = 0x80000000 | encodedDev | (offset & 0xfc);
    outl(PCI_CONFIG_ADDR, addr);
	*value = inl(PCI_CONFIG_DATA);
	return OK;
}

/*------------------------------------------------------------------------
 * pci_write_config_byte  -  Write a byte to device's config space
 *------------------------------------------------------------------------
 */
int pci_write_config_byte(
		uint32	encodedDev,	/* Encoded PCI device		*/
		int32	offset,		/* Offset in config space	*/
		byte	value		/* Value to be written		*/
		)
{
    uint32 addr;

    addr = 0x80000000 | encodedDev | (offset & 0xfc);
    outl(PCI_CONFIG_ADDR, addr);
    outb(PCI_CONFIG_DATA + (offset & 0x03), value);

	return OK;
}

/*------------------------------------------------------------------------
 * pci_write_config_word  -  Write a word to device's config space
 *------------------------------------------------------------------------
 */
int pci_write_config_word(
		uint32	encodedDev,	/* Encoded PCI device		*/
		int32	offset,		/* Offset in config space	*/
		uint16	value		/* Value to be written		*/
		)
{
    uint32 addr;

    addr = 0x80000000 | encodedDev | (offset & 0xfc);
    outl(PCI_CONFIG_ADDR, addr);
    outw(PCI_CONFIG_DATA + (offset & 0x02), value);

	return OK;
}

/*------------------------------------------------------------------------
 * pci_write_config_dword  -  Write a dword to device's config space
 *------------------------------------------------------------------------
 */
int pci_write_config_dword(
		uint32	encodedDev,	/* Encoded PCI device		*/
		int32	offset,		/* Offset in config space	*/
		uint32	value		/* Value to be written		*/
		)
{
    uint32 addr;

    addr = 0x80000000 | encodedDev | (offset & 0xfc);
    outl(PCI_CONFIG_ADDR, addr);
    outl(PCI_CONFIG_DATA, value);

	return OK;
}


/*------------------------------------------------------------------------
 * find_pci_device - find a specified PCI device
 *------------------------------------------------------------------------
 */
int find_pci_device(int32 deviceID, int32 vendorID, int32 index)
{
	int32	count;		    /* Count of the device		*/
	int32	multifunction;  /* Is device multifinction?	*/
	uint32	info;		    /* Encoded PCI device		*/
	uint32	bus, dev, func;	/* Bus, device, function nos.	*/
	struct pci_config_header devfuncHdr;

	/* Traverse PCI configuration space looking for
	   a match for the target device */
	count = 0;
	for (bus = 0; bus < PCI_MAX_BUSES; bus++) {
		for (dev = 0; dev < PCI_DEVICES_PER_BUS; dev++) {
			for (func = 0; func < PCI_FUNCTIONS_PER_DEVICE;
			     func++) {
                pci_read_config_word(PCI_MAKE_ID(bus, dev, func), PCI_CONFIG_VENDOR_ID,
			(uint16 *)&devfuncHdr.vendorID);
                pci_read_config_word(PCI_MAKE_ID(bus, dev, func), PCI_CONFIG_DEVICE_ID,
			(uint16 *)&devfuncHdr.deviceID);

				if ((devfuncHdr.vendorID == vendorID) &&
				    (devfuncHdr.deviceID == deviceID) &&
				    (index == count++)) {
					/* Match: return the matching PCI
					   Bus:Device:Function encoded
					   into an uint32. */
					info = PCI_MAKE_ID(bus, dev, func);

					return info;
				}
				/* Evaluate sub-functions only if the
				   device is multi-function */
				if (func == 0) {
					multifunction =
						devfuncHdr.headerType &
						PCI_HDR_TYPE_MULTIFCN;
					if (!multifunction) {
						break; /* Inner for */
					}
				}
			}
		}
	}
	/* The target device was not found. */
	return SYSERR;
}

