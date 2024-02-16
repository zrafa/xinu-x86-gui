/* uart.c - uartinit */

#include <xinu.h>

/*------------------------------------------------------------------------
 * uartinit  -  Initialize the uart
 *------------------------------------------------------------------------
 */
void	uartinit()
{
	volatile struct uart_csreg *csrptr = (struct uart_csreg *)0x3f8;

	/* Initialize the uart */

	/* Set baud rate */
	outb((int32)&csrptr->lcr, UART_LCR_DLAB);
	outb((int32)&csrptr->dlm, 0x00);            /* 9600 */
	outb((int32)&csrptr->dll, 0x0c);

	outb((int32)&csrptr->lcr,UART_LCR_8N1);     /* 8 bit char, No Parity, 1 Stop */
	outb((int32)&csrptr->fcr, 0x00);	        /* Disable FIFO for now		*/
}

