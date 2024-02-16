/* conf.h (GENERATED FILE; DO NOT EDIT) */

/* Device switch table declarations */

/* Device table entry */
struct	dentry	{
	int32   dvnum;
	int32   dvminor;
	char    *dvname;
	devcall (*dvinit) (struct dentry *);
	devcall (*dvopen) (struct dentry *, char *, char *);
	devcall (*dvclose)(struct dentry *);
	devcall (*dvread) (struct dentry *, void *, uint32);
	devcall (*dvwrite)(struct dentry *, void *, uint32);
	devcall (*dvseek) (struct dentry *, int32);
	devcall (*dvgetc) (struct dentry *);
	devcall (*dvputc) (struct dentry *, char);
	devcall (*dvcntl) (struct dentry *, int32, int32, int32);
	void    *dvcsr;
	void    (*dvintr)(void);
	byte    dvirq;
};

extern	struct	dentry	devtab[]; /* one entry per device */

/* Device name definitions */

#define CONSOLE              0	/* type tty      */
#define VTTY0                1	/* type tty      */
#define VTTY1                2	/* type tty      */
#define VTTY2                3	/* type tty      */
#define VTTY3                4	/* type tty      */
#define VTTY4                5	/* type tty      */
#define VTTY5                6	/* type tty      */
#define VTTY6                7	/* type tty      */
#define VTTY7                8	/* type tty      */
#define VTTY8                9	/* type tty      */
#define VTTY9               10	/* type tty      */
#define VGA                 11	/* type vga      */
#define MOUSE               12	/* type mice     */
#define KEYBOARD            13	/* type kbd      */
#define NULLDEV             14	/* type null     */
#define ETHER0              15	/* type eth      */
#define NAMESPACE           16	/* type nam      */
#define RDISK               17	/* type rds      */
#define RAM0                18	/* type ram      */
#define RFILESYS            19	/* type rfs      */
#define RFILE0              20	/* type rfl      */
#define RFILE1              21	/* type rfl      */
#define RFILE2              22	/* type rfl      */
#define RFILE3              23	/* type rfl      */
#define RFILE4              24	/* type rfl      */
#define RFILE5              25	/* type rfl      */
#define RFILE6              26	/* type rfl      */
#define RFILE7              27	/* type rfl      */
#define RFILE8              28	/* type rfl      */
#define RFILE9              29	/* type rfl      */
#define LFILESYS            30	/* type lfs      */
#define LFILE0              31	/* type lfl      */
#define LFILE1              32	/* type lfl      */
#define LFILE2              33	/* type lfl      */
#define LFILE3              34	/* type lfl      */
#define LFILE4              35	/* type lfl      */
#define LFILE5              36	/* type lfl      */

/* Control block sizes */

#define	Nnull	1
#define	Ntty	11
#define	Neth	1
#define	Nrds	1
#define	Nram	1
#define	Nrfs	1
#define	Nrfl	10
#define	Nlfs	1
#define	Nlfl	6
#define	Nvga	1
#define	Nmice	1
#define	Nkbd	1
#define	Nnam	1

#define NDEVS 37


/* Configuration and Size Constants */

#define	NPROC	     100	/* number of user processes		*/
#define	NSEM	     100	/* number of semaphores			*/
#define	IRQBASE	     32		/* base ivec for IRQ0			*/
#define	IRQ_TIMER    IRQ_HW5	/* timer IRQ is wired to hardware 5	*/
#define	IRQ_ATH_MISC IRQ_HW4	/* Misc. IRQ is wired to hardware 4	*/
#define CLKFREQ      200000000	/* 200 MHz clock			*/

#define	LF_DISK_DEV	RAM0

#define VGA_WIDTH	1024 /* IMPORTANT: YOU MUST MODIFY system/start.S AS WELL */
#define VGA_HEIGHT	768 /* IMPORTANT: YOU MUST MODIFY system/start.S AS WELL */
#define VGA_BPP		32 /* IMPORTANT: YOU MUST MODIFY system/start.S AS WELL */
#define TIMESERVER	"200.11.116.10"

#ifndef	ETHER0
#define	ETHER0	0
#endif

/* max numbers of virtual terminals available */
#define  MAX_N_VT 10   /* WARNING: keep this in sync with VTTY# max VTTY! */
