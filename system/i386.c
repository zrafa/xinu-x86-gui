/* i386.c - setsegs */

#include <xinu.h>
#include <icu.h>
#include <stdio.h>

#define BOOTP_CODE
#define	KCODE	1
#define	KSTACK	2
#define	KDATA	3

struct sd gdt_copy[NGD] = {
		/* 0th entry NULL */
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
		/* 1st, Kernel Code Segment */
	{ 0xffff, 0, 0, 6, 1, 1, 0, 1, 0xf, 0, 0, 1, 1, 0 },
		/* 2nd, Kernel Data Segment */
	{ 0xffff, 0, 0, 2, 0, 1, 0, 1, 0xf, 0, 0, 1, 1, 0 },
		/* 3rd, Kernel Stack Segment */
	{ 0xffff, 0, 0, 2, 0, 1, 0, 1, 0xf, 0, 0, 1, 1, 0 },
		/* 4st, Bootp Code Segment */
	{ 0xffff, 0, 0, 6, 1, 1, 0, 1, 0xf, 0, 0, 1, 1, 0 },
		/* 5th, Code Segment for BIOS32 request */
	{ 0xffff, 0, 0, 6, 1, 1, 0, 1, 0xf, 0, 0, 1, 1, 0 },
		/* 6th, Data Segment for BIOS32 request */
	{ 0xffff, 0, 0, 2, 0, 1, 0, 1, 0xf, 0, 0, 1, 1, 0 },
};

extern	void	setirmask(void);
extern	struct	sd gdt[];
extern	struct	segtr gdtr;

long	initsp;		/* initial SP for nulluser() */

extern	void	*maxheap;

/*------------------------------------------------------------------------
 * setsegs - initialize the 386 processor
 *------------------------------------------------------------------------
 */
void	setsegs()
{
	extern int	etext;
	struct sd	*psd;
	uint32		np, npages;

	npages = 4096;		/* 16 Meg for now */
	maxheap = (char *)(npages * NBPG - 1);

	psd = &gdt_copy[1];	/* kernel code segment */
	np = ((int)&etext + NBPG-1) / NBPG;	/* # code pages */
	psd->sd_lolimit = np;
	psd->sd_hilimit = np >> 16;

	psd = &gdt_copy[2];	/* kernel data segment */
	psd->sd_lolimit = npages;
	psd->sd_hilimit = npages >> 16;

	psd = &gdt_copy[3];	/* kernel stack segment */
	psd->sd_lolimit = npages;
	psd->sd_hilimit = npages >> 16;

	psd = &gdt_copy[4];	/* bootp code segment */
	psd->sd_lolimit = npages;   /* Allows execution of 0x100000 CODE */
	psd->sd_hilimit = npages >> 16;

	memcpy(gdt, gdt_copy, sizeof(gdt_copy));
	initsp = npages*NBPG  - 4;
}

extern	int outb(int, int);

/*------------------------------------------------------------------------
 * init8259 - initialize the 8259A interrupt controllers
 *------------------------------------------------------------------------
 */
void	init8259()
{

	/* Master device */
	outb(ICU1, 0x11);	/* ICW1: icw4 needed		*/
	outb(ICU1+1, 0x20);	/* ICW2: base ivec 32		*/
	outb(ICU1+1, 0x4);	/* ICW3: cascade on irq2	*/
	outb(ICU1+1, 0x1);	/* ICW4: buf. master, 808x mode */
	outb(ICU1, 0xb);	/* OCW3: set ISR on read	*/

	/* Slave device */
	outb(ICU2, 0x11);	/* ICW1: icw4 needed		*/
	outb(ICU2+1, 0x28);	/* ICW2: base ivec 40		*/
	outb(ICU2+1, 0x2);	/* ICW3: slave on irq2		*/
	outb(ICU2+1, 0xb);	/* ICW4: buf. slave, 808x mode	*/
	outb(ICU2, 0xb);	/* OCW3: set ISR on read	*/

	setirmask();
}

void	pseg(struct sd *psd)
{
	int		i;
	unsigned char	*pb = (unsigned char *)psd;
	unsigned int	base;
	unsigned int	limit;

	kprintf("pseg (%X)\n", pb);
	kprintf("hex:   %02x", (unsigned int)*pb++);
	for(i=1; i<8; ++i) {
		kprintf(" (%X)", pb);
		kprintf(" %02x", (unsigned int)*pb++);
	}
	limit = ((int)psd->sd_hilimit) << 16;
	limit |= psd->sd_lolimit;
	base = ((int)psd->sd_hibase) << 24;
	base |= ((int)psd->sd_midbase)<< 16;
	base |= psd->sd_lobase;
	
	kprintf("\nby field: base %X limit %d perm %d\n", base, limit,
		psd->sd_perm);
	kprintf("iscode %d isapp %d dpl %d present %d avl %d\n",
		psd->sd_iscode, psd->sd_isapp, psd->sd_dpl, psd->sd_present,
		psd->sd_avl);
	kprintf("mbz %d 32b %d gran %d\n", psd->sd_mbz, psd->sd_32b,
		psd->sd_gran);
}
#define	IMR1	0x21
#define	IMR2	0xA1

/*------------------------------------------------------------------------
 * setirmask - set the interrupt mask in the controller
 *------------------------------------------------------------------------
 */
void	setirmask(void)
{
	if (girmask == 0) {	/* skip until girmask initialized */
		return;
	}
	outb(IMR1, girmask&0xff);
	outb(IMR2, (girmask>>8)&0xff);
	return;
}
