/* multiboot.h - definitions for the multiboot header */

struct mbootinfo {
	uint32	flags;			/* Multiboot information flags	*/
	uint32	mem_lower;		/* Memory in KB below 1M	*/
	uint32	mem_upper;		/* Memory in KB above 1M	*/
	uint32	boot_device;		/* Boot device containing image	*/
	uint32	cmdline;		/* Address of command line	*/
	uint32	mods_count;		/* No. of modules loaded	*/
	uint32	mods_addr;		/* Address to the module info	*/
	uint32	syms[4];		/* Info regarding ELF sections	*/
	uint32	mmap_length;		/* Length(bytes) of mmap region	*/
	uint32	mmap_addr;		/* Address of the mmap regios	*/
	uint32	drives_length;		/* Size of drives structures	*/
	uint32	drives_addr;		/* Address of Drive structures	*/
	uint32	config_table;		/* Address of ROM config table	*/
	uint32	boot_loader_name;	/* Address of boot loader name	*/
	uint32	apm_table;		/* Address of APM table		*/
	uint32	vbe_control_info;	/* VBE control info		*/
	uint32	vbe_mode_info;		/* VBE mode info		*/
	uint16	vbe_mode;		/* Current Video mode		*/
	uint16	vbe_interface_seg;	/* VBE related information	*/
	uint16	vbe_interface_off;	/* VBE related information	*/
	// RAFA uint32	vbe_interface_len;	/* VBE related informarion	*/
	uint16	vbe_interface_len;	/* VBE related informarion	*/

	uint64 framebuffer_addr;
	uint32 framebuffer_pitch;
	uint32 framebuffer_width;
	uint32 framebuffer_height;
	uint8 framebuffer_bpp;
};

extern struct mbootinfo *mbi;

struct mbmregion {
	uint32	size;			/* Entry Size			*/
	uint64	base_addr;		/* Memory region base address	*/
	uint64	length;			/* Memory region length		*/
	uint32	type;			/* Memory region type		*/
};

/* Multiboot header magic number	*/
#define	MULTIBOOT_HEADER_MAGIC  	0x1BADB002

/* Flags for required multiboot info	*/
#define	MULTIBOOT_HEADER_FLAGS  	0x00010003

/* Multiboot signature verification	*/
#define MULTIBOOT_SIGNATURE		0x2BADB002

/* mem_upper mem_lower fields valid	*/
#define MULTIBOOT_BOOFINFO_MEM		0x00000001

/* boot device field valid		*/
#define MULTIBOOT_BOOTINFO_BOOTDEV	0x00000002

/* cmdline field valid			*/
#define MULTIBOOT_BOOTINFO_CMDLINE	0x00000004

/* mods_count mods_addr valid		*/
#define MULTIBOOT_BOOTINFO_MODS		0x00000008

/* mmap_length mmap_addr valid		*/
#define MULTIBOOT_BOOTINFO_MMAP		0x00000040

/* Usable memory region 		*/
#define	MULTIBOOT_MMAP_TYPE_USABLE	0x00000001
