/* icu.s */

/* Interrupt controller symbolic constants for assembly language	*/
/*	Note: icu.h defines equivalent constants for C functions	*/

	.set	ICU1,0x20	/* I/O port address, 8259A #1		*/
	.set	ICU2,0xA0	/* I/O port address, 8258A #2		*/

	.set	OCW1_2,ICU1	
	.set	OCW2_2,ICU2

	.set	IMR1,ICU1 + 1	/* Interrupt Mask Register for #1	*/
	.set	IMR2,ICU2 + 1	/* Interrupt Mask Register for #2	*/

	.set	EOI,0x20	/* non-specific end of interrupt	*/
