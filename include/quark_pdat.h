/* quark_pdat.h - Functions for reading the platform data */

/* Structures and Values used to read the platform data from the memory mapped SPI flash */
#define QUARK_PDAT_MAGIC       "PDAT"      /* Platform header signature */
#define QUARK_PDAT_MEMORY_LOC  0xFFF10000  /* Memory mapped location of the platform data */

/* Platform Type Identifiers */
#define QUARK_PLAT_TYPE_ID     1
#define QUARK_MAC1_ID          3
#define QUARK_MAC2_ID          4
#define QUARK_MRCPARAMS_ID     6

/* Platform entry descriptions */
#define QUARK_PLAT_TYPE_DESC   "PlatformID"
#define QUARK_MRCPARAMS_DESC   "MrcParams"
#define QUARK_MAC1_DESC        "1st MAC"
#define QUARK_MAC2_DESC        "2nd MAC"

#pragma pack(2)
struct quark_pdat_header {
	char 	quark_pdat_magic[4];  /* Platform header signature */
	uint32 	quark_pdat_len;       /* Size in bytes of the platform data (not including the header */
	uint32	quark_pdat_crc;       /* CRC value for the platform data (not including the header) */
	
	/* Platform data entries follow here */
};
#pragma pack()

#pragma pack(2)
struct quark_pdat_entry {
	uint16	pdat_entry_id;        /* Type identifier for the platform data entry */
	uint16	pdat_entry_len;       /* Length in bytes of the platform entry (not including the entry header) */
	char	pdat_entry_desc[10];  /* Description string of the platform entry */
	uint16	pdat_entry_ver;       /* Version of the platform entry */
	
	/* variable length data for the entry */
};
#pragma pack()

int get_quark_pdat_entry_data_by_id(uint16 type_id, char* buf, uint32 size);
int get_quark_pdat_entry_data_by_desc(const char* entry_desc, char* buf, uint32 size);
