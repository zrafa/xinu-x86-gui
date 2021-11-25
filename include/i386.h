/* i386.h - DELAY */

#define	NBPG		4096

#define	NID		48
#define	NGD		 8

#define	IRQBASE		32	/* base ivec for IRQ0               */

struct idt {
    unsigned short	igd_loffset;
    unsigned short	igd_segsel;
    unsigned int	igd_rsvd : 5;
    unsigned int	igd_mbz : 3;
    unsigned int	igd_type : 5;
    unsigned int	igd_dpl : 2;
    unsigned int	igd_present : 1;
    unsigned short	igd_hoffset;
};

#define	IGDT_TASK	 5	/* task gate IDT descriptor	        */
#define	IGDT_INTR	14	/* interrupt gate IDT descriptor    */
#define	IGDT_TRAPG	15	/* Trap Gate                        */


/* Segment Descriptor */

struct sd {
    unsigned short	sd_lolimit;
    unsigned short	sd_lobase;
    unsigned char	sd_midbase;
    unsigned int	sd_perm : 3;
    unsigned int	sd_iscode : 1;
    unsigned int	sd_isapp : 1;
    unsigned int	sd_dpl : 2;
    unsigned int	sd_present : 1;
    unsigned int	sd_hilimit : 4;
    unsigned int	sd_avl : 1;
    unsigned int	sd_mbz : 1;		/* must be '0' */
    unsigned int	sd_32b : 1;
    unsigned int	sd_gran : 1;
    unsigned char	sd_hibase;
};

extern	struct sd	gdt_copy[];

#define	sd_type		sd_perm

/* System Descriptor Types */

#define	SDT_INTG	14	/* Interrupt Gate	*/

/* Segment Table Register */
struct segtr {
    unsigned int	len : 16;
    unsigned int	addr : 32;
};
