/* intr.s - disable, restore, enable, pause, halt, spurious_irq7,	*/
/*			spurious_irq15					*/

#include <icu.s>

	.text
	.globl	disable
	.globl	restore
	.globl	enable
	.globl	pause
	.globl	halt
	.globl	spurious_irq7
	.globl	spurious_irq15

/*------------------------------------------------------------------------
 * disable  -  Disable interrupts and return the previous state
 *------------------------------------------------------------------------
 */
disable:
	pushfl
	cli
	popl	%eax
	andl	$0x00000200,%eax
	ret

/*------------------------------------------------------------------------
 * restore  -  Restore interrupts to value given by mask argument
 *------------------------------------------------------------------------
 */
restore:
        cli
        movw    4(%esp),%ax
	andl	$0x00000200,%eax
	pushl	%eax
	popfl
        ret

/*------------------------------------------------------------------------
 * enable  -  Enable all interrupts
 *------------------------------------------------------------------------
 */
enable:
	sti
	ret

/*------------------------------------------------------------------------
 * pause  -  Halt the processor until an interrupt occurs
 *------------------------------------------------------------------------
 */
pause:
	call enable
	hlt
	ret


/*------------------------------------------------------------------------
 * halt  -  Do nothing forever
 *------------------------------------------------------------------------
 */
halt:
	jmp halt
	ret

/*------------------------------------------------------------------------
 * spurious_irq7  -  Handle an unexpected interrupt for irq7
 *------------------------------------------------------------------------
 */
spurious_irq7:
	mov	$EOI,%al
	outb	%al,$OCW1_2
	iret

/*------------------------------------------------------------------------
 * spurious_irq15  -  Handle an unexpected interrupt for irq15
 *------------------------------------------------------------------------
 */
spurious_irq15:
	mov	$EOI,%al
	outb	%al,$OCW1_2
	mov	$EOI,%al
	outb	%al,$OCW2_2
	iret

/*------------------------------------------------------------------------
 * Xtrap  -  Entry point when no interrupt/exception handler in place
 *------------------------------------------------------------------------
 */
Xtrap:
	call	trap			/* Call C trap handler		*/
	iret				/* Should never occur		*/

/************************************************************************/
/*									*/
/* Definitions of _Xint0 through _Xint47 entry points for interrupts	*/
/*		and exceptions (referenced as initial entries in the	*/
/*		interrupt vector array below)				*/
/*									*/
/************************************************************************/

	.globl	_Xint0			/* Divide error			*/
_Xint0:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$0
	jmp	Xtrap

	.globl  _Xint1			/* Debug exception		*/
_Xint1:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$1
	jmp	Xtrap

	.globl  _Xint2			/* Nonmaskable interrupt	*/
_Xint2:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$2
	jmp	Xtrap

	.globl  _Xint3			/* Breakpoint (one-byte INT 3	*/
_Xint3:
	pushl	%ebp			/*    instruction)		*/
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$3
	jmp	Xtrap

	.globl  _Xint4			/* Overflow (INTO instruction)	*/
_Xint4:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$4
	jmp	Xtrap

	.globl  _Xint5			/* Bounds check (BOUND instr)	*/
_Xint5:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$5
	jmp	Xtrap

	.globl  _Xint6			/* Invalid opcode		*/
_Xint6:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$6
	jmp	Xtrap

	.globl  _Xint7			/* Coprocessor not available	*/
_Xint7:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$7
	jmp	Xtrap

	.globl  _Xint8			/* Double fault			*/
_Xint8:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$8
	jmp	Xtrap
	
	.globl  _Xint9			/* (reserved)			*/
_Xint9:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$9
	jmp	Xtrap

	.globl  _Xint10			/* Invalid TSS			*/
_Xint10:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$10
	jmp	Xtrap

	.globl  _Xint11			/* Segment not present		*/
_Xint11:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$11
	jmp	Xtrap

	.globl  _Xint12			/* Stack exception		*/
_Xint12:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$12
	jmp	Xtrap

	.globl  _Xint13			/* General protection		*/
_Xint13:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$13
	jmp	Xtrap

	.globl  _Xint14			/* Page fault			*/
_Xint14:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$14
	jmp	Xtrap

	.globl  _Xint15			/* (reserved)			*/
_Xint15:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$15
	jmp	Xtrap

	.globl  _Xint16			/* Coprocessor error		*/
_Xint16:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$16
	jmp	Xtrap

	.globl  _Xint17			/* (reserved)			*/
_Xint17:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$17
	jmp	Xtrap

	.globl  _Xint18			/* (reserved)			*/
_Xint18:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$18
	jmp	Xtrap

	.globl  _Xint19			/* (reserved)			*/
_Xint19:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$19
	jmp	Xtrap

	.globl  _Xint20			/* (reserved)			*/
_Xint20:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$20
	jmp	Xtrap

	.globl  _Xint21			/* (reserved)			*/
_Xint21:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$21
	jmp	Xtrap

	.globl  _Xint22			/* (reserved)			*/
_Xint22:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$22
	jmp	Xtrap

	.globl  _Xint23			/* (reserved)			*/
_Xint23:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$23
	jmp	Xtrap

	.globl  _Xint24			/* (reserved)			*/
_Xint24:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$24
	jmp	Xtrap

	.globl  _Xint25			/* (reserved)			*/
_Xint25:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$25
	jmp	Xtrap

	.globl  _Xint26			/* (reserved)			*/
_Xint26:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$26
	jmp	Xtrap

	.globl  _Xint27			/* (reserved)			*/
_Xint27:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$27
	jmp	Xtrap

	.globl  _Xint28			/* (reserved)			*/
_Xint28:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$28
	jmp	Xtrap

	.globl  _Xint29			/* (reserved)			*/
_Xint29:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$29
	jmp	Xtrap

	.globl  _Xint30			/* (reserved)			*/
_Xint30:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$30
	jmp	Xtrap

	.globl  _Xint31			/* (reserved)			*/
_Xint31:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$31
	jmp	Xtrap

	.globl  _Xint32			/* available for ext. interrupt	*/
_Xint32:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$32
	jmp	Xtrap

	.globl  _Xint33			/* available for ext. interrupt	*/
_Xint33:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$33
	jmp	Xtrap
	
	.globl  _Xint34			/* available for ext. interrupt	*/
_Xint34:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$34
	jmp	Xtrap

	.globl  _Xint35			/* available for ext. interrupt	*/
_Xint35:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$35
	jmp	Xtrap

	.globl  _Xint36			/* available for ext. interrupt	*/
_Xint36:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$36
	jmp	Xtrap

	.globl  _Xint37			/* available for ext. interrupt	*/
_Xint37:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$37
	jmp	Xtrap

	.globl  _Xint38			/* available for ext. interrupt	*/
_Xint38:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$38
	jmp	Xtrap

	.globl  _Xint39			/* available for ext. interrupt	*/
_Xint39:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$39
	jmp	Xtrap

	.globl  _Xint40			/* available for ext. interrupt	*/
_Xint40:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$40
	jmp	Xtrap

	.globl  _Xint41			/* available for ext. interrupt	*/
_Xint41:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$41
	jmp	Xtrap

	.globl  _Xint42			/* available for ext. interrupt	*/
_Xint42:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$42
	jmp	Xtrap

	.globl  _Xint43			/* available for ext. interrupt	*/
_Xint43:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$43
	jmp	Xtrap

	.globl  _Xint44			/* available for ext. interrupt	*/
_Xint44:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$44
	jmp	Xtrap

	.globl  _Xint45			/* available for ext. interrupt	*/
_Xint45:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$45
	jmp	Xtrap

	.globl  _Xint46			/* available for ext. interrupt	*/
_Xint46:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$46
	jmp	Xtrap

/*------------------------------------------------------------------------
 * _Xint? - array of exception/interrupt entry points that are
 *		initialized to point to default handlers
 *------------------------------------------------------------------------
 */
	.globl	defevec
defevec:
	.long	_Xint0			/* Divide error			*/
	.long	_Xint1			/* Debug exceptions		*/
	.long	_Xint2			/* Nonmaskable interrupt	*/
	.long	_Xint3			/* Breakpoint (one-byte INT 3	*/
					/*    instruction)		*/	
	.long	_Xint4			/* Overflow (INTO instruction)	*/
	.long	_Xint5			/* Bounds check (BOUND instr)	*/
	.long	_Xint6			/* Invalid opcode		*/
	.long	_Xint7			/* Coprocessor not available	*/
	.long	_Xint8			/* Double fault			*/
	.long	_Xint9			/* (reserved)			*/
	.long	_Xint10			/* Invalid TSS			*/
	.long	_Xint11			/* Segment not present		*/
	.long	_Xint12			/* Stack exception		*/
	.long	_Xint13			/* General protection		*/
	.long	_Xint14			/* Page fault			*/
	.long	_Xint15			/* (reserved)			*/
	.long	_Xint16			/* Coprocessor error		*/
	.long	_Xint17			/* (reserved)			*/
	.long	_Xint18			/* (reserved)			*/
	.long	_Xint19			/* (reserved)			*/
	.long	_Xint20			/* (reserved)			*/
	.long	_Xint21			/* (reserved)			*/
	.long	_Xint22			/* (reserved)			*/
	.long	_Xint23			/* (reserved)			*/
	.long	_Xint24			/* (reserved)			*/
	.long	_Xint25			/* (reserved)			*/
	.long	_Xint26			/* (reserved)			*/
	.long	_Xint27			/* (reserved)			*/
	.long	_Xint28			/* (reserved)			*/
	.long	_Xint29			/* (reserved)			*/
	.long	_Xint30			/* (reserved)			*/
	.long	_Xint31			/* (reserved)			*/
	.long	_Xint32			/* available for ext. interrupt	*/
	.long	_Xint33			/* available for ext. interrupt	*/
	.long	_Xint34			/* available for ext. interrupt	*/
	.long	_Xint35			/* available for ext. interrupt	*/
	.long	_Xint36			/* available for ext. interrupt	*/
	.long	_Xint37			/* available for ext. interrupt	*/
	.long	_Xint38			/* available for ext. interrupt	*/
	.long	spurious_irq7
	.long	_Xint40			/* available for ext. interrupt	*/
	.long	_Xint41			/* available for ext. interrupt	*/
	.long	_Xint42			/* available for ext. interrupt	*/
	.long	_Xint43			/* available for ext. interrupt	*/
	.long	_Xint44			/* available for ext. interrupt	*/
	.long	_Xint45			/* available for ext. interrupt	*/
	.long	_Xint46			/* available for ext. interrupt	*/
	.long	spurious_irq15
