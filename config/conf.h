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
#define NULLDEV              1	/* type null     */
#define ETHER0               2	/* type eth      */
#define NAMESPACE            3	/* type nam      */
#define RDISK                4	/* type rds      */
#define RAM0                 5	/* type ram      */
#define RFILESYS             6	/* type rfs      */
#define RFILE0               7	/* type rfl      */
#define RFILE1               8	/* type rfl      */
#define RFILE2               9	/* type rfl      */
#define RFILE3              10	/* type rfl      */
#define RFILE4              11	/* type rfl      */
#define RFILE5              12	/* type rfl      */
#define RFILE6              13	/* type rfl      */
#define RFILE7              14	/* type rfl      */
#define RFILE8              15	/* type rfl      */
#define RFILE9              16	/* type rfl      */
#define LFILESYS            17	/* type lfs      */
#define LFILE0              18	/* type lfl      */
#define LFILE1              19	/* type lfl      */
#define LFILE2              20	/* type lfl      */
#define LFILE3              21	/* type lfl      */
#define LFILE4              22	/* type lfl      */
#define LFILE5              23	/* type lfl      */

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
#define	Nnam	1

#define NDEVS 24


/* Configuration and Size Constants */

#define	NPROC	     100	/* number of user processes		*/
#define	NSEM	     100	/* number of semaphores			*/
#define	IRQBASE	     32		/* base ivec for IRQ0			*/
#define	IRQ_TIMER    IRQ_HW5	/* timer IRQ is wired to hardware 5	*/
#define	IRQ_ATH_MISC IRQ_HW4	/* Misc. IRQ is wired to hardware 4	*/
#define CLKFREQ      200000000	/* 200 MHz clock			*/

#define	LF_DISK_DEV	RAM0

#ifndef	ETHER0
#define	ETHER0	0
#endif

