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
#define VGA                  1	/* type vga      */
#define MOUSE                2	/* type mice     */
#define KEYBOARD             3	/* type kbd      */
#define NULLDEV              4	/* type null     */
#define ETHER0               5	/* type eth      */
#define NAMESPACE            6	/* type nam      */
#define RDISK                7	/* type rds      */
#define RAM0                 8	/* type ram      */
#define RFILESYS             9	/* type rfs      */
#define RFILE0              10	/* type rfl      */
#define RFILE1              11	/* type rfl      */
#define RFILE2              12	/* type rfl      */
#define RFILE3              13	/* type rfl      */
#define RFILE4              14	/* type rfl      */
#define RFILE5              15	/* type rfl      */
#define RFILE6              16	/* type rfl      */
#define RFILE7              17	/* type rfl      */
#define RFILE8              18	/* type rfl      */
#define RFILE9              19	/* type rfl      */
#define LFILESYS            20	/* type lfs      */
#define LFILE0              21	/* type lfl      */
#define LFILE1              22	/* type lfl      */
#define LFILE2              23	/* type lfl      */
#define LFILE3              24	/* type lfl      */
#define LFILE4              25	/* type lfl      */
#define LFILE5              26	/* type lfl      */

/* Control block sizes */

#define	Nnull	1
#define	Ntty	1
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

#define NDEVS 27


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

#ifndef	ETHER0
#define	ETHER0	0
#endif

