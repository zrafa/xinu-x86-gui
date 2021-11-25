/* uart.h - definintions for the NS16550 uart serial hardware */

#define UART_BAUD	115200	/* Default console baud rate.		*/
#define	UART_OUT_IDLE	0x0016	/* determine if transmit idle		*/
#define	UART_FIFO_SIZE	16	/* chars in UART onboard output FIFO	*/
				/* (16 for later UART chips)		*/
#define INTEL_UART_PCI_DID	0x0936	/* UART PCI Device ID		*/
#define INTEL_UART_PCI_VID	0x8086	/* UART PCI Vendor ID		*/
/*
 * Control and Status Register (CSR) definintions for the 16550 UART.
 * The code maps the structure structure directly onto the base address
 * CSR address for the device.
 */
struct	uart_csreg
{
	volatile byte	buffer;	/* receive buffer (when read)		*/
				/*   OR transmit hold (when written)	*/
	volatile byte	ier;	/* interrupt enable			*/
	volatile byte	iir;	/* interrupt identification (when read)	*/
				/*   OR FIFO control (when written)	*/
	volatile byte	lcr;	/* line control register		*/
	volatile byte	mcr;	/* modem control register		*/
	volatile byte	lsr;	/* line status register			*/
	volatile byte	msr;	/* modem status register		*/
	volatile byte	scr;	/* scratch register			*/
};

/* Alternative names for control and status registers */

#define	rbr	buffer		/* receive buffer (when read)		*/
#define	thr	buffer		/* transmit hold (when written)		*/
#define	fcr	iir		/* FIFO control (when written)		*/
#define	dll	buffer		/* divisor latch (low byte)		*/
#define	dlm	ier		/* divisor latch (high byte)		*/

/* Definintion of individual bits in control and status registers	*/

/* Port offsets from the base */

#define	UART_DLL	0x00	/* value for low byte of divisor latch	*/
				/*	DLAB=0				*/
#define UART_DLM	0x01	/* value for high byte of divisor latch	*/
				/*	DLAB=1				*/

/* Line control bits */

#define	UART_LCR_DLAB	0x80	/* Divisor latch access bit		*/
#define	UART_LCR_8N1	0x03	/* 8 bits, no parity, 1 stop		*/

/* Interrupt enable bits */

#define UART_IER_ERBFI	0x01	/* Received data interrupt mask		*/
#define UART_IER_ETBEI	0x02	/* Transmitter buffer empty interrupt	*/
#define UART_IER_ELSI	0x04	/* Recv line status interrupt mask	*/
#define UART_IER_EMSI	0x08	/* Modem status interrupt mask		*/

/* Interrupt identification masks */

#define UART_IIR_IRQ	0x01	/* Interrupt pending bit		*/
#define UART_IIR_IDMASK 0x0E	/* 3-bit field for interrupt ID		*/
#define UART_IIR_MSC	0x00	/* Modem status change			*/
#define UART_IIR_THRE	0x02	/* Transmitter holding register empty	*/
#define UART_IIR_RDA	0x04	/* Receiver data available		*/
#define UART_IIR_RLSI	0x06	/* Receiver line status interrupt	*/
#define UART_IIR_RTO	0x0C	/* Receiver timed out			*/

/* FIFO control bits */

#define UART_FCR_EFIFO	0x01	/* Enable in and out hardware FIFOs	*/
#define UART_FCR_RRESET 0x02	/* Reset receiver FIFO			*/
#define UART_FCR_TRESET 0x04	/* Reset transmit FIFO			*/
#define UART_FCR_TRIG0	0x00	/* RCVR FIFO trigger level one char	*/
#define UART_FCR_TRIG1	0x40	/* RCVR FIFO trigger level 1/4		*/
#define UART_FCR_TRIG2	0x80	/* RCVR FIFO trigger level 2/4		*/
#define UART_FCR_TRIG3	0xC0	/* RCVR FIFO trigger level 3/4		*/

/* Modem control bits */

#define UART_MCR_OUT2	0x08	/* User-defined OUT2			*/
#define UART_MCR_RTS    0x02    /* RTS complement 			*/
#define UART_MCR_DTR    0x01    /* DTR complement 			*/
#define UART_MCR_LOOP	0x10	/* Enable loopback test mode		*/

/* Line status bits */

#define UART_LSR_DR	0x01	/* Data ready				*/
#define	UART_LSR_BI	0x10	/* Break interrupt indicator		*/
#define UART_LSR_THRE	0x20	/* Transmit-hold-register empty		*/
#define UART_LSR_TEMT	0x40	/* Transmitter empty			*/
