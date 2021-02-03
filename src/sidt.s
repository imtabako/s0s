.section .bss
.align	16
.global	idt
idt:
.skip	0x80	# 16 * 8-bytes
idt_len:

.section .text
fmt:
.ascii	"Interrupt\12\0"

.type	ihandler_0, @function
ihandler_0:
#	mov	%eip, %eax
#	sub	$ihandler_0, %eax
#	sub	$0, %eax

	mov	$fmt, %eax
	push	%eax
	call	vga_puts
	add	$4, %esp

.Lend:	hlt
	jmp	.Lend

	iret
.size	ihandler_0, . - ihandler_0

.type	set_idt_entry, @function
set_idt_entry:
	push	%eax
	push	%ebx
	push	%ecx
	mov	16(%esp), %ecx

	mov	$ihandler_0, %eax
	mov	$idt, %ebx
	mov	%ax, (%ebx, %ecx, 8)		# offset 15..0
	shr	$16, %eax
	mov	%ax, 6(%ebx, %ecx, 8)		# offset 31..16
	mov	%cs, 2(%ebx, %ecx, 8)		# selector (CS) in GDT
	movw	$0x8E00, 4(%ebx, %ecx, 8)	# flags, interrupt gate
	
	pop	%ecx
	pop	%ebx
	pop	%eax
	ret
.size	set_idt_entry, . - set_idt_entry

.global	set_idt
.type	set_idt, @function
set_idt:
	push	%ecx

	xor	%ecx, %ecx
	push	%ecx
	call	set_idt_entry
	add	$4, %esp

	mov	$14, %ecx
	push	%ecx
	call	set_idt_entry
	add	$4, %esp

	pushl	$idt
	pushw	$(idt_len-idt-1)
	lidt	(%esp)
	sti
	add	$6, %esp

	pop	%ecx
	ret
.size	set_idt, . - set_idt
