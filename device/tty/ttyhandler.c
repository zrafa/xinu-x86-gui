/* ttyhandler.c - ttyhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyhandler  -  Handle an interrupt for a tty (serial) device
 *------------------------------------------------------------------------
 */
void ttyhandler(void) {
	struct	dentry	*devptr;	/* Address of device control blk*/
	struct	ttycblk	*typtr;		/* Pointer to ttytab entry	*/	
	struct	uart_csreg *csrptr;	/* Address of UART's CSR	*/
	byte	iir = 0;		/* Interrupt identification	*/

	/* Get CSR address of the device (assume console for now) */

	devptr = (struct dentry *) &devtab[CONSOLE];
	csrptr = (struct uart_csreg *) devptr->dvcsr;

	/* Obtain a pointer to the tty control block */

	typtr = &ttytab[ devptr->dvminor ];

	/* Decode hardware interrupt request from UART device */

        /* Check interrupt identification register */
	iir = io_inb(csrptr->iir);
        if (iir & UART_IIR_IRQ) {
		return;
        }

	/* Decode the interrupt cause based upon the value extracted	*/
	/* from the UART interrupt identification register.  Clear	*/
	/* the interrupt source and perform the appropriate handling	*/
	/* to coordinate with the upper half of the driver		*/

        /* Decode the interrupt cause */

        iir &= UART_IIR_IDMASK;		/* Mask off the interrupt ID */
        switch (iir) {

	    /* Receiver line status interrupt (error) */

	    case UART_IIR_RLSI:
		return;

	    /* Receiver data available or timed out */

	    case UART_IIR_RDA:
	    case UART_IIR_RTO:

		resched_cntl(DEFER_START);

		/* While chars avail. in UART buffer, call ttyhandle_in	*/

		while ( (io_inb(csrptr->lsr) & UART_LSR_DR) != 0) {
			ttyhandle_in(typtr, csrptr);
                }

		resched_cntl(DEFER_STOP);

		return;

            /* Transmitter output FIFO is empty (i.e., ready for more)	*/

	    case UART_IIR_THRE:
		ttyhandle_out(typtr, csrptr);
		return;

	    /* Modem status change (simply ignore) */

	    case UART_IIR_MSC:
		return;
	    }
}
