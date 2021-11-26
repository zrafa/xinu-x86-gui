/* micedispatch.S - micedispatch */

#include <icu.s>
		.text
		.globl	micehandlerirq
		.globl	micehandler

/*------------------------------------------------------------------------
 *  micedispatch  -  Dispatch an interrupt to the mouse handler function
 *------------------------------------------------------------------------
 */
micehandlerirq:
		pushal			/* Save general-purpose regs.	*/
		pushfl			/* Save the flags register	*/
		cli			/* Prevent further interrupts	*/
		movb	$EOI,%al	/* Clear the interrupt		*/
		outb	%al,$OCW1_2
		call	micehandler	/* Call the handler		*/
		popfl			/* Restore the flags register	*/
		popal			/* Restore general-purpose regs.*/
		iret			/* Return from interrupt	*/
