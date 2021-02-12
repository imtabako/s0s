.set	TDIV, 11931
.set	THZ, 100
.section .bss
time_cnt:
.skip	4

# .extern	set_idt_entry

.section .text
.global	set_timer
.type	set_timer, @function
set_timer:
	push	%ax
	movl	$THZ, (time_cnt)

	mov	$0x36, %al	# square wave mode, 16-bit value, IRQ0 timer
	outb	%al, $0x43	# CMD port
	outb	%al, $0x80	# io wait

	mov	$TDIV, %ax	# IRQ0 timer DATA/DIV port
	outb	%al, $0x40	# First timer port accepts least signif 8bits
	outb	%al, $0x80	# io wait
	shr	$8, %ax
	outb	%al, $0x40	# ...then most significat 8bits

	pushw	$0x8E00		# Interrupt gate, CS, index 32
	pushw	%cs
	pushl	$irq0_handler
	pushl	$0x20
	call	set_idt_entry
	add	$12, %esp

	pop	%ax
	ret
.size	set_timer, . - set_timer

fmt_tim:
.ascii	"S \0"
.type	irq0, @function
irq0_handler:
	push	%eax

	mov	(time_cnt), %eax
	or	%eax, %eax
	jz	irq0_echo
	decl	(time_cnt)
	jmp	irq0_end

irq0_echo:
	movl	$THZ, (time_cnt)
	pushl	$79
	call	_vga_timer_inc
	add	$4, %esp

irq0_end:

	mov	$0x20, %al	# EOI to PIC master
	out	%al, $0x20

	pop	%eax
	iret
.size	irq0_handler, . - irq0_handler
