/* ethdispatch.S - ethdispatch */

#include <icu.s>
		.text
		.globl	ethdispatch
		.globl	ethhandler
ethdispatch:
		pushal
		pushfl
		cli
		movb	$EOI,%al	# clear the interrupt
		outb	%al,$OCW1_2
		movb	$EOI,%al
		outb	%al,$OCW2_2

		call	ethhandler

		popfl
		popal
		iret
