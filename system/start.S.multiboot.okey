# 0 "boot.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "boot.S"
# 19 "boot.S"
# 1 "multiboot.h" 1
# 20 "boot.S" 2
# 39 "boot.S"
        .text

        .globl start, _start
start:
_start:
        jmp multiboot_entry


        .align 4


multiboot_header:

        .long 0x1BADB002

        .long 0x00000001 | 0x00000002 | 0x00000004 | 0

        .long -(0x1BADB002 + 0x00000001 | 0x00000002 | 0x00000004 | 0)
# 69 "boot.S"
        .long 0
        .long 0
        .long 0
        .long 0
        .long 0

        .long 0
        .long 1024
        .long 768
        .long 32

multiboot_entry:

        movl $(stack + 0x4000), %esp


        pushl $0
        popf


        pushl %ebx

        pushl %eax


        call nulluser


        pushl $halt_message
        call printf

loop: hlt
        jmp loop

halt_message:
        .asciz "Halted."


        .comm stack, 0x4000


 .data

        .align  16
        .globl  gdt
        .globl  gdtr
        .globl  idt
        .globl  idtr

gdt:    .space  64      # must equal NGD*8 (64 = 8 segments)
gdtr:   .word   63      # sizeof _gdt -1 (in bytes)
        .long   gdt     # global pointer to the gdt
idt:    .space  1024    # must equal NID*8 (1024 == 256 entries * 8 bytes per entry)
idtr:   .word   1023    # size of _idt -1 (in bytes)
        .long   idt

        .globl  cpudelay
cpudelay:       .long   1





	.text
	.globl delay

	.globl	inb
inb:	movl	4(%esp), %edx
	xorl	%eax, %eax	# clr eax
	inb	%dx, %al
	ret

	.globl	inw
inw:	movl	4(%esp), %edx
	xorl	%eax, %eax	# clr eax
	inw	%dx, %ax
	ret

	.globl  inl
inl:	movl	4(%esp), %edx
	xorl	%eax, %eax
	inl	%dx, %eax
	ret

	.globl	outb
outb:	movl	4(%esp), %edx
	movl	8(%esp), %eax
	outb	%al, %dx
	ret

	.globl	outw
outw:	movl	4(%esp), %edx
	movl	8(%esp), %eax
	outw	%ax, %dx
	ret

	.globl	outl
outl:	movl	4(%esp), %edx
	movl	8(%esp), %eax
	outl	%eax, %dx
	ret

#ifndef SMALL
	.globl	_rtcin
_rtcin:	movl	4(%esp), %eax
	outb	%al, $0x70
	subl	%eax, %eax	# clr eax
	inb	$0x71, %al
	ret
#endif

	.globl ___udivsi3
___udivsi3:
	movl 4(%esp), %eax
	xorl %edx, %edx
	divl 8(%esp)
	ret

	.globl ___divsi3
___divsi3:
	movl 4(%esp), %eax
	xorl %edx, %edx
	cltd
	idivl 8(%esp)
	ret

	#
	# bzero (base,cnt)
	#

	.globl sbzero
sbzero:
	pushl	%edi
	movl	8(%esp), %edi
	movl	12(%esp), %ecx
	movb	$0x00, %al
	cld
	rep
	stosb
	popl	%edi
	ret

	#
	# bcopy(src, dst, count)
	#

	.globl	bcopy
bcopy:
	pushl	%esi
	pushl	%edi
	movl	12(%esp),%esi
	movl	16(%esp),%edi
L1:
	movl	20(%esp),%ecx
	cld
	rep
	movsb
	popl	%edi
	popl	%esi
	ret

	# insw(port, addr, cnt)
	.globl	insw
insw:
	pushl	%edi
	movw	8(%esp), %dx
	movl	12(%esp), %edi
	movl	16(%esp), %ecx
	cld
	.byte 0x66, 0xf2, 0x6d	# rep insw
	movl	%edi, %eax
	popl	%edi
	ret

	# outsw(port, addr, cnt)
	.globl	outsw
outsw:
	pushl	%esi
	movw	8(%esp), %dx
	movl	12(%esp), %esi
	movl	16(%esp), %ecx
	cld
	.byte 0x66, 0xf2, 0x6f	# rep outsw
	movl	%esi, %eax
	popl	%esi
	ret

	#
	# lidt() - load interrupt descriptor table from idtr
	#
	.globl	lidt
lidt:
	lidt	idtr
	ret

jumptocode:
	ljmp	$0x20,$0x100000	/* CS descriptor 1 */

jumptobootcode:
        ljmp    $0x20,$BOOTPLOC /* CS descriptor 1 */
