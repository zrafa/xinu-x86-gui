/* micedispatch.S - micedispatch */

#include <icu.s>
		.text
		.globl	micedispatch

/*------------------------------------------------------------------------
 *  micedispatch  -  Dispatch an interrupt to the kbd/mice handler function
 *------------------------------------------------------------------------
 */
micedispatch:
		pushal              /* Save general-purpose regs.	*/
		pushfl			    /* Save the flags register	*/
		cli			        /* Prevent further interrupts	*/
		movb	$EOI,%al	/* Clear the interrupt		*/
		outb	%al,$OCW1_2
		outb	%al,$OCW2_2
		call	micehandler  /* Call the handler	*/
		popfl			    /* Restore the flags register	*/
		popal			    /* Restore general-purpose regs.*/
		iret			    /* Return from interrupt	*/

