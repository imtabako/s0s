.section .bss
.align	16
.global	idt
idt:
.skip	0x180	# (32+16) * 8-bytes
idt_len:

.section .text
fmt:
.ascii	"Interrupt \0"

.type	ihandler_0, @function
ihandler_0:
	pushl	$fmt
	call	_puts
	mov	$0, %eax
	add	$48, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend0:	hlt
	jmp	.Lend0

	iret
.size	ihandler_0, . - ihandler_0

.type	ihandler_1, @function
ihandler_1:
	pushl	$fmt
	call	_puts
	mov	$1, %eax
	add	$48, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend1:	hlt
	jmp	.Lend1

	iret
.size	ihandler_1, . - ihandler_1

.type	ihandler_2, @function
ihandler_2:
	pushl	$fmt
	call	_puts
	mov	$2, %eax
	add	$48, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend2:	hlt
	jmp	.Lend2

	iret
.size	ihandler_2, . - ihandler_2

.type	ihandler_3, @function
ihandler_3:
	pushl	$fmt
	call	_puts
	mov	$3, %eax
	add	$48, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend3:	hlt
	jmp	.Lend3

	iret
.size	ihandler_3, . - ihandler_3

.type	ihandler_4, @function
ihandler_4:
	pushl	$fmt
	call	_puts
	mov	$4, %eax
	add	$48, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend4:	hlt
	jmp	.Lend4

	iret
.size	ihandler_4, . - ihandler_4

.type	ihandler_5, @function
ihandler_5:
	pushl	$fmt
	call	_puts
	mov	$5, %eax
	add	$48, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend5:	hlt
	jmp	.Lend5

	iret
.size	ihandler_5, . - ihandler_5

.type	ihandler_6, @function
ihandler_6:
	pushl	$fmt
	call	_puts
	mov	$6, %eax
	add	$48, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend6:	hlt
	jmp	.Lend6

	iret
.size	ihandler_6, . - ihandler_6

.type	ihandler_7, @function
ihandler_7:
	pushl	$fmt
	call	_puts
	mov	$7, %eax
	add	$48, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend7:	hlt
	jmp	.Lend7

	iret
.size	ihandler_7, . - ihandler_7

.type	ihandler_8, @function
ihandler_8:
	pushl	$fmt
	call	_puts
	mov	$8, %eax
	add	$48, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend8:	hlt
	jmp	.Lend8

	iret
.size	ihandler_8, . - ihandler_8

.type	ihandler_9, @function
ihandler_9:
	pushl	$fmt
	call	_puts
	mov	$9, %eax
	add	$48, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend9:	hlt
	jmp	.Lend9

	iret
.size	ihandler_9, . - ihandler_9

.type	ihandler_10, @function
ihandler_10:
	pushl	$fmt
	call	_puts
	mov	$0, %eax
	add	$65, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend10:	hlt
	jmp	.Lend10

	iret
.size	ihandler_10, . - ihandler_10

.type	ihandler_11, @function
ihandler_11:
	pushl	$fmt
	call	_puts
	mov	$1, %eax
	add	$65, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend11:	hlt
	jmp	.Lend11

	iret
.size	ihandler_11, . - ihandler_11

.type	ihandler_12, @function
ihandler_12:
	pushl	$fmt
	call	_puts
	mov	$2, %eax
	add	$65, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend12:	hlt
	jmp	.Lend12

	iret
.size	ihandler_12, . - ihandler_12

.type	ihandler_13, @function
ihandler_13:
	pushl	$fmt
	call	_puts
	mov	$3, %eax
	add	$65, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend13:	hlt
	jmp	.Lend13

	iret
.size	ihandler_13, . - ihandler_13

.type	ihandler_14, @function
ihandler_14:
	pushl	$fmt
	call	_puts
	mov	$4, %eax
	add	$65, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend14:	hlt
	jmp	.Lend14

	iret
.size	ihandler_14, . - ihandler_14

.type	ihandler_15, @function
ihandler_15:
	pushl	$fmt
	call	_puts
	mov	$5, %eax
	add	$65, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend15:	hlt
	jmp	.Lend15

	iret
.size	ihandler_15, . - ihandler_15

.type	ihandler_16, @function
ihandler_16:
	pushl	$fmt
	call	_puts
	mov	$6, %eax
	add	$65, %eax
	push	%eax
	call	_putc
	add	$8, %esp

.Lend16:	hlt
	jmp	.Lend16

	iret
.size	ihandler_16, . - ihandler_16

fmtq:
.ascii	"IRQ\12\0"
.type	irq1, @function
irq1:
	push	%ax
	in	$0x60, %al

	push	%ax
	mov	$0x20, %al
	out	%al, $0x20
	pop	%ax

	push	%ax
	call	kbd_handler
	add	$2, %esp

	pop	%ax
	iret
.size	irq1, . - irq1

# set_idt_entry(int ind, void *ihandler(), short selector, short type)
# Sets IDT[ind] with given arguments
# 
# ESP		EIP
# ESP+4		dw ind
# ESP+8		dw ptr
# ESP+12	w  sel
# ESP+14	w  type
# 
# Format of IDT_entry is following (assuming interrupt gate):
#       31               15               0
# 	+----------------+----------------+
#	| OFFSET 31..16  | TYPE           | 4
# 	+----------------+----------------+
#	| SELECTOR       | OFFSET 15..0   | 0
# 	+----------------+----------------+
.global	set_idt_entry
.type	set_idt_entry, @function
set_idt_entry:
	push	%eax
	push	%ebx
	push	%ecx

	mov	$idt, %ebx
	mov	16(%esp), %ecx			# index

	mov	20(%esp), %eax			# ptr
	mov	%ax, (%ebx, %ecx, 8)		# offset 15..0
	shr	$16, %eax
	mov	%ax, 6(%ebx, %ecx, 8)		# offset 31..16

	mov	24(%esp), %eax			# type:cel
	mov	%ax, 2(%ebx, %ecx, 8)		# selector (CS) in GDT
	shr	$16, %eax
	mov	%ax, 4(%ebx, %ecx, 8)		# flags, interrupt gate
	
	pop	%ecx
	pop	%ebx
	pop	%eax
	ret
.size	set_idt_entry, . - set_idt_entry

.global	set_idt
.type	set_idt, @function
set_idt:
	push	%ecx

	mov	$17, %ecx
	pushw	$0x8E00
	pushw	%cs
	pushl	$ihandler_16
	push	%ecx
	decl	(%esp)

.Lsidt0:
	call	set_idt_entry
	subl	$0x1F, 4(%esp)
	decl	(%esp)
	loop	.Lsidt0

	movl	$0x21, (%esp)
	movl	$irq1, 4(%esp)
	call	set_idt_entry
	add	$12, %esp

	call	set_timer
	call	pic_init

	pushl	$idt
	pushw	$(idt_len-idt-1)
	lidt	(%esp)
	sti
	add	$6, %esp

	pop	%ecx
	ret
.size	set_idt, . - set_idt
