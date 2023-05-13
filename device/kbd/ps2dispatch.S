/* ps2dispatch.S - ps2dispatch */

#include <icu.s>
		.text
		.globl	ps2handlerirq
		.globl	ps2_shared_handler

/*------------------------------------------------------------------------
 *  ps2dispatch  -  Dispatch an interrupt to the kbd/mice handler function
 *------------------------------------------------------------------------
 */
ps2handlerirq:
		pushal			/* Save general-purpose regs.	*/
		pushfl			/* Save the flags register	*/
		cli			/* Prevent further interrupts	*/
		movb	$EOI,%al	/* Clear the interrupt		*/
		outb	%al,$OCW1_2
		call	ps2_shared_handler    /* Call the handler	*/
		popfl			/* Restore the flags register	*/
		popal			/* Restore general-purpose regs.*/
		iret			/* Return from interrupt	*/

