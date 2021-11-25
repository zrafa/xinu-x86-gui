/* pci.h
 *
 * Benjamin A. Kuperman
 * 10 July 2001
 * 
 * Various structures and such used in pci.c
 */

typedef struct bios32 {
    unsigned long   signature;          /* "_32_"                            */
    unsigned long   bios32_entry;       /* 32 bit physical address for
                                           entry into the BIOS32 service
                                           directory                         */
    unsigned char   revision;           /* should be 0x00                    */
    unsigned char   length;             /* length of data in 16 byte units   */
    unsigned char   checksum;           /* sum of all bytes should be 0      */
    unsigned char   padding[5];         /* all zeros                         */
} bios32;

/* the bios can only exist in a certain memory range */
#define BIOS32_MIN_ADDR    0x0e0000
#define BIOS32_MAX_ADDR 0x0ffff0
#define BIOS32_SIGNATURE (('_' << 0) + ('3' << 8) + ('2' << 16) + ('_' << 24))
#define BIOS32_UNITS    16    /* used with bios32->length */

/* The string "$PCI" is used to interface with BIOS32 for PCI service */
#define PCI_SERVICE (('$' << 0) + ('P' << 8) + ('C' << 16) + ('I' << 24))

/* stuff snagged from linux to make this work */
#define save_flags(x) \
__asm__ __volatile__("pushfl ; popl %0":"=g" (x): /* no input */ :"memory")

#define restore_flags(x) \
__asm__ __volatile__("pushl %0 ; popfl": /* no output */ :"g" (x):"memory")

#define cli() __asm__ __volatile__ ("cli": : :"memory")

#define PCIBIOS_PCI_BIOS_PRESENT            0xb101
#define PCIBIOS_FIND_PCI_DEVICE             0xb102
#define PCIBIOS_FIND_PCI_CLASS_CODE         0xb103
#define PCIBIOS_GENERATE_SPECIAL_CYCLE      0xb106
#define PCIBIOS_READ_CONFIG_BYTE            0xb108
#define PCIBIOS_READ_CONFIG_WORD            0xb109
#define PCIBIOS_READ_CONFIG_DWORD           0xb10a
#define PCIBIOS_WRITE_CONFIG_BYTE           0xb10b
#define PCIBIOS_WRITE_CONFIG_WORD           0xb10c
#define PCIBIOS_WRITE_CONFIG_DWORD          0xb10d
#define PCIBIOS_GET_IRQ_ROUTING_OPTIONS     0xb10e
#define PCIBIOS_SET_PCI_IRQ                 0xb10f

#define PCIBIOS_SUCCESSFUL                  0x00
#define PCIBIOS_FUNC_NOT_SUPPORTED          0x81
#define PCIBIOS_BAD_VENDOR_ID               0x83
#define PCIBIOS_DEVICE_NOT_FOUND            0x86
#define PCIBIOS_BAD_REGISTER_NUMBER         0x87
#define PCIBIOS_SET_FAILED                  0x88
#define PCIBIOS_BUFFER_TOO_SMALL            0x89

#define PCIBIOS_ETHERNET_CLASS              ((0x02<<16) + (0x00<<8) + 0)

#define PCIBIOS_MAX_DEVICE                  65535
#define PCIBIOS_MAX_VENDOR                  65534
#define PCIBIOS_MAX_INDEX                   255 /* arbitrary */


/* PCI signature: "PCI " */
#define PCI_SIGNATURE    (('P' << 0) + ('C' << 8) + ('I' << 16) + (' ' << 24))

extern int pci_init(void);
extern int find_pci_device(int32, int32, int32);
extern struct ether     dev_eth;
extern struct eth_pd*   eth_txring;
extern struct eth_pd*   eth_rxring;

extern    int pci_bios_read_config_byte(uint32, int, unsigned char *);
extern    int pci_bios_read_config_word(uint32, int, uint16 *);
extern    int pci_bios_read_config_dword(uint32, int, uint32 *);
extern    int pci_bios_write_config_byte(uint32, int, unsigned char);
extern    int pci_bios_write_config_word(uint32, int, uint16);
extern    int pci_bios_write_config_dword(uint32, int, uint32);
