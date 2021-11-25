/* sdmcDispatch.S - sdmcDispatch */

#include <icu.s>
		.text
		.globl	sdmcdispatch
		.globl	sdmcinterrupt
sdmcdispatch:
		pushal
		pushfl
		cli
		movb	$EOI,%al	# clear the interrupt
		outb	%al,$OCW1_2
		movb	$EOI,%al
		outb	%al,$OCW2_2

		call	sdmcinterrupt

		popfl
		popal
		iret
