/* ttydispatch.S - ttydispatch */

#include <icu.s>
		.text
		.globl	ttydispatch
		.globl	ttyhandler

/*------------------------------------------------------------------------
 *  ttydispatch  -  Dispatch an interrupt to the tty handler function
 *------------------------------------------------------------------------
 */
ttydispatch:
		pushal			/* Save general-purpose regs.	*/
		pushfl			/* Save the flags register	*/
		cli			/* Prevent further interrupts	*/
		movb	$EOI,%al	/* Clear the interrupt		*/
		outb	%al,$OCW1_2
		call	ttyhandler	/* Call the handler		*/
		popfl			/* Restore the flags register	*/
		popal			/* Restore general-purpose regs.*/
		iret			/* Return from interrupt	*/
