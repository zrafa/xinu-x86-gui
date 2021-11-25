/* io.h - io_inb, io_inw, io_inl, io_outb, io_outw, io_outl */

/* Define I/O macros used in device driver code to reference device	*/
/*	Control and Status Registers (CSRs).  The macros either		*/
/*	generate the memory-mapped I/O references needed for newer x86	*/
/*	platforms, such as the Galileo boards, or the port-mapped I/O	*/
/*	calls needed for traditional x86 machines and for VM platforms,	*/
/*	such as Vbox and VMWare.					*/

#ifdef	MEM_MAPPED

/* Memory-mapped input merely references an item in the device's CSRs;	*/
/*	memory-mapped output assigns a value to an item			*/

#define	io_inb(item)		(item)
#define	io_inw(item)		(item)
#define	io_inl(item)		(item)

#define	io_outb(item,val)	((item) = (val))
#define	io_outw(item,val)	((item) = (val))
#define	io_outl(item,val)	((item) = (val))

#else

/* Port-mapped input passes the address of an item in the CSRs to inb,	*/
/*	inw, or inl; port-mapped output calls outb, outw, or outl with	*/
/*	the address of an item (cast to in int32) and a value		*/

#define	io_inb(item)		inb( (int32)&(item))
#define	io_inw(item)		inw( (int32)&(item))
#define	io_inl(item)		inl( (int32)&(item))

#define	io_outb(item,val)	outb((int32)&(item),(val))
#define	io_outw(item,val)	outw((int32)&(item),(val))
#define	io_outl(item,val)	outl((int32)&(item),(val))

#endif
