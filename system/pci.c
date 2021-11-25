/* pci.c - pci_init */

#include <xinu.h>

static	int	pcibios_init(void);
static	int	check_pcibios(void);
static	unsigned long bios32_service(unsigned long service);
static	bios32	 *bios32ptr;	/* pointer to the BIOS 32 structure */

/* from linux */
static unsigned long bios32_entry = 0;
static struct {
	unsigned long address;
	unsigned short segment;
} bios32_indirect = { 0, 0 + 5*0x08 /* 0x10  KERNEL_CS */ };

static long pcibios_entry = 0;
static struct {
    unsigned long address;
    unsigned short segment;
} pci_indirect = { 0, 0+5*0x08 /* 0x10  KERNEL_CS */ };

/*------------------------------------------------------------------------
 * pci_init - initialize the PCI bios structures and such
 *------------------------------------------------------------------------
 */
int32	pci_init(void)
{
    if (pcibios_init() == OK) {
	return OK;
    } else {
	kprintf("pci_init: PCI BIOS initialization failed\n");
	return SYSERR;
    }
}

/*------------------------------------------------------------------------
 * pcibios_init
 *------------------------------------------------------------------------
 */
int pcibios_init(void)
{
    unsigned char   *ptr;
    bios32	    *bptr;
    int		    i,length;
    unsigned char   cksum;

    /* search for it */
    for (bptr = (bios32 *)BIOS32_MIN_ADDR; 
	 bptr <= (bios32 *)BIOS32_MAX_ADDR; bptr++) {
	/* check for signature first */
	if (bptr->signature != BIOS32_SIGNATURE)
	    continue;
	/* now check the checksum */
	cksum=0;
	length = bptr->length * BIOS32_UNITS;
	ptr=(unsigned char *)bptr;
	for (i=0; i<length; i++) {
	    cksum += ptr[i];
	}
	if (cksum != 0) {
	    kprintf("pcibios_init: invalid checksum at 0x%08lx\n",bptr);
	    continue;
	}
	/* verify the revision */
	if (bptr->revision != 0) {
	    kprintf("pcibios_init: invalid revision %d at 0x%08lx\n",
		    bptr->revision,bptr);
	    continue;
	}
	/* If I am here, then all tests were valid */
	bios32ptr = bptr;		/* set the global reference */

	bios32_entry = bios32ptr->bios32_entry;
	bios32_indirect.address = bios32_entry;

	if (check_pcibios()) {
	    return OK;
	} else {
	    return SYSERR;
	}
    }

    /* If this point is reached, nothing was found */
    kprintf("pcibios_init: no BIOS32 Service Directory was found\n");
    return SYSERR;

}


/*------------------------------------------------------------------------
 * check_pcibios - Check for a PCI BIOS
 *------------------------------------------------------------------------
 */
int check_pcibios(void)
{
    unsigned long signature;                        
    unsigned char present_status;
#ifdef	DEBUG
    unsigned char major_revision;
    unsigned char minor_revision;
#endif
    unsigned long flags;
    int pack;

    if ((pcibios_entry = bios32_service(PCI_SERVICE))) {
	pci_indirect.address = pcibios_entry;
	save_flags(flags);
	cli();
	__asm__(  "lcall *(%%edi)\n\t"
		  "jc 1f\n\t"
		  "xorb %%ah, %%ah\n"
		  "1:\tshl $8, %%eax\n\t"
		  "movw %%bx, %%ax"
		: "=d" (signature),
		  "=a" (pack)
		: "1" (PCIBIOS_PCI_BIOS_PRESENT),
		  "D" (&pci_indirect)
		: "bx", "cx");
	restore_flags(flags);

	present_status = (pack >> 16) & 0xff;
#ifdef	DEBUG
	major_revision = (pack >> 8) & 0xff;
	minor_revision = pack & 0xff;
#endif
	if (present_status || (signature != PCI_SIGNATURE)) {
	    kprintf ("pcibios_init : %s : BIOS32 Service Directory says PCI BIOS is present,\n"
		    "       but PCI_BIOS_PRESENT subfunction fails with present status of 0x%x\n"
		    "       and signature of 0x%08lx (%c%c%c%c).\n",
		    (signature == PCI_SIGNATURE) ?  "WARNING" : "ERROR",
		    present_status, signature,
		    (char) (signature >>  0), (char) (signature >>  8),
		    (char) (signature >> 16), (char) (signature >> 24));

	    if (signature != PCI_SIGNATURE)
		pcibios_entry = 0;
	}
	if (pcibios_entry != 0) {
#ifdef DEBUG
	    kprintf ("pcibios_init : PCI BIOS revision %x.%02x entry at 0x%x\n",
		    major_revision, minor_revision, pcibios_entry);
#endif
	    return 1;
	}
    }
    return 0;
}

static unsigned long bios32_service(unsigned long service)
{
    unsigned char return_code;      /* %al */
    unsigned long address;          /* %ebx */
    unsigned long length;           /* %ecx */
    unsigned long entry;            /* %edx */
    unsigned long flags;

    /* According to the PCI BIOS specification, the following needs to
     * take place before the "CALL FAR":
     *
     * 1. The CS code segment selector and the DS data segment
     * selector must be set up to encompass the physical page holding
     * the entry point *as well as* the immediately following physical
     * page.
     *
     * 2. Them must also have the same base.
     *
     * 3. Platform BIOS writers must assume that CS is execute-only
     * and DS is read-only.
     *
     * 4. The SS stack segment selectore must provide at least 1K of
     * stack space.
     */

    save_flags(flags); 
    cli();
    asm(    "nop\n"
	    "nop\n"
	    "lcall *(%%edi)\n"
	    : "=a" (return_code),
	    "=b" (address),
	    "=c" (length),
	    "=d" (entry)
	    : "0" (service),
	    "1" (0),
	    "D" (&bios32_indirect));
    restore_flags(flags);

    switch (return_code) {
	case 0:
	    return address + entry;
	case 0x80:      /* Not present */
	    kprintf("bios32_service(0x%lx) : not present\n", service);
	    return 0;
	default: /* Shouldn't happen */
	    kprintf("bios32_service(0x%lx) : returned unexpected value 0x%x\n",
		    service, return_code);
	    return 0;
    }
}


/*------------------------------------------------------------------------
 * find_pci_device - find a specified PCI device
 *------------------------------------------------------------------------
 */
int find_pci_device(int32 deviceID, int32 vendorID, int32 index)
{
    unsigned long retval, info, error;
#ifdef	DEBUG
    int busNum, deviceNum, functionNum;
#endif

    /*
     * Returns the location of a PCI device based on the Device ID
     * and Vendor ID.
     */
#ifdef DEBUG
    kprintf("find_pci_device: vendor=0x%x device=0x%x index=%d\n",
	    vendorID, deviceID, index);
#endif
    /*
     * Entry
     * -----
     * AH  - PCI_FUNCTION_CODE
     * AL  - FIND_PCI_DEVICE
     * CX  - Device ID
     * DX  - Vendor ID
     * SI  - Index (0..N)
     *
     * Exit
     * ----
     * BH  - Bus Number (0-255)
     * BL  - Dev Num (upper 5 bits), Function Number (lower 3 bits)
     * AH  - Return Code
     * CF  - Completion status: set=error, cleared=success
     */
/*
    save_flags(flags);
    cli();
*/
    asm(    "lcall *(%%edi)\n"	    /* call the function */
#if 0
	    /* Really need to check the carry flag here */
#else
	    "xor %%ecx, %%ecx\n"    /* placeholding */
#endif
	   :"=a" (retval),
	    "=b" (info),
	    "=c" (error)
	   :"0" (PCIBIOS_FIND_PCI_DEVICE),
	    "2" (deviceID),
	    "d" (vendorID),
	    "S" (index),
	    "D" (&pci_indirect));
/*
    restore_flags(flags);
*/
#ifdef	DEBUG
    /* set up the return values */
    busNum	    = 0xff & (info >> 8);
    deviceNum	    = 0x07 & info;	    /* lower 3 bits */
    functionNum	    = 0x1f & (info >> 3);   /* next 5 bits */
    kprintf("find_pci_device: error=0x%x bus=%d dev=0x%x fun=0x%x (info=0x%x)\n",
	    error, busNum, deviceNum, functionNum, info);
    kprintf("retval = 0x%lx\n",0xff & (retval>>8));
#endif
    if (PCIBIOS_SUCCESSFUL == (0xff & (retval>>8))) {
	return info;
    } else
	return SYSERR;
}


int pci_bios_read_config_byte(uint32 dev, int where, unsigned char *value)
{
	unsigned long ret;

	__asm__("lcall *(%%esi); cld\n\t"
		"jc 1f\n\t"
		"xorb %%ah, %%ah\n"
		"1:"
		: "=c" (*value),
		  "=a" (ret)
		: "1" (PCIBIOS_READ_CONFIG_BYTE),
		  "b" (dev),
		  "D" ((long) where),
		  "S" (&pci_indirect));
	return (int) (ret & 0xff00) >> 8;
}

int pci_bios_read_config_word(uint32 dev, int where, unsigned short *value)
{
	unsigned long ret;

	__asm__("lcall *(%%esi); cld\n\t"
		"jc 1f\n\t"
		"xorb %%ah, %%ah\n"
		"1:"
		: "=c" (*value),
		  "=a" (ret)
		: "1" (PCIBIOS_READ_CONFIG_WORD),
		  "b" (dev),
		  "D" ((long) where),
		  "S" (&pci_indirect));
	return (int) (ret & 0xff00) >> 8;
}

int pci_bios_read_config_dword(uint32 dev, int where, uint32 *value)
{
	unsigned long ret;

	__asm__("lcall *(%%esi); cld\n\t"
		"jc 1f\n\t"
		"xorb %%ah, %%ah\n"
		"1:"
		: "=c" (*value),
		  "=a" (ret)
		: "1" (PCIBIOS_READ_CONFIG_DWORD),
		  "b" (dev),
		  "D" ((long) where),
		  "S" (&pci_indirect));
	return (int) (ret & 0xff00) >> 8;
}

int pci_bios_write_config_byte(uint32 dev, int where, unsigned char value)
{
	unsigned long ret;

	__asm__("lcall *(%%esi); cld\n\t"
		"jc 1f\n\t"
		"xorb %%ah, %%ah\n"
		"1:"
		: "=a" (ret)
		: "0" (PCIBIOS_WRITE_CONFIG_BYTE),
		  "c" (value),
		  "b" (dev),
		  "D" ((long) where),
		  "S" (&pci_indirect));
	return (int) (ret & 0xff00) >> 8;
}

int pci_bios_write_config_word(uint32 dev, int where, unsigned short value)
{
	unsigned long ret;

	__asm__("lcall *(%%esi); cld\n\t"
		"jc 1f\n\t"
		"xorb %%ah, %%ah\n"
		"1:"
		: "=a" (ret)
		: "0" (PCIBIOS_WRITE_CONFIG_WORD),
		  "c" (value),
		  "b" (dev),
		  "D" ((long) where),
		  "S" (&pci_indirect));
	return (int) (ret & 0xff00) >> 8;
}

int pci_bios_write_config_dword(uint32 dev, int where, uint32 value)
{
	unsigned long ret;

	__asm__("lcall *(%%esi); cld\n\t"
		"jc 1f\n\t"
		"xorb %%ah, %%ah\n"
		"1:"
		: "=a" (ret)
		: "0" (PCIBIOS_WRITE_CONFIG_DWORD),
		  "c" (value),
		  "b" (dev),
		  "D" ((long) where),
		  "S" (&pci_indirect));
	return (int) (ret & 0xff00) >> 8;
}
