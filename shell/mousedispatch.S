/* ttydispatch.S - ttydispatch */

#include <icu.s>
		.text
		.globl	mouse_handler_irq
		.globl	mouse_handler

/*------------------------------------------------------------------------
 *  ttydispatch  -  Dispatch an interrupt to the tty handler function
 *------------------------------------------------------------------------
 */
mouse_handler_irq:
		pushal			/* Save general-purpose regs.	*/
		pushfl			/* Save the flags register	*/
		cli			/* Prevent further interrupts	*/
		movb	$EOI,%al	/* Clear the interrupt		*/
		outb	%al,$OCW1_2
		call	mouse_handler	/* Call the handler		*/
		popfl			/* Restore the flags register	*/
		popal			/* Restore general-purpose regs.*/
		iret			/* Return from interrupt	*/
