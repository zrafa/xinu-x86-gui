/* kbddispatch.S - kbddispatch */

#include <icu.s>
		.text
		.globl	kbdhandlerirq
		// .globl	kbdhandler
		.globl	ps2_shared_handler

/*------------------------------------------------------------------------
 *  kbddispatch  -  Dispatch an interrupt to the keyboard handler function
 *------------------------------------------------------------------------
 */
kbdhandlerirq:
		pushal			/* Save general-purpose regs.	*/
		pushfl			/* Save the flags register	*/
		cli			/* Prevent further interrupts	*/
		movb	$EOI,%al	/* Clear the interrupt		*/
		outb	%al,$OCW1_2
		// call	kbdhandler	/* Call the handler		*/
		call	ps2_shared_handler	/* Call the handler		*/
		popfl			/* Restore the flags register	*/
		popal			/* Restore general-purpose regs.*/
		iret			/* Return from interrupt	*/

