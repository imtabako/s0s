.section .data
.align	16
.global	gdt
gdt:
.long	0x0		# NULL desc
.long	0x0
.long	0x0000FFFF	# Code Kernel
.long	0x00CF9A00
.long	0x0000FFFF	# Data Kernel
.long	0x00CF9200
.long	0x0000FFFF	# Code User
.long	0x00CFFA00
.long	0x0000FFFF	# Data User
.long	0x00CFF200
gdt_tss:
.long	0x0		# TSS desc (set later in `set_gdt')
.long	0x0
gdt_len:


.section .text
.global	set_gdt
.type	set_gdt, @function
set_gdt:
	push	%eax
	push	%ebx
	push	%ecx

# Set GDT entry for TSS
# TODO: know why to do it
# TSS now is actually 103-dword empty space in RAM
	mov	$gdt_tss, %ebx	# move GDT[TSS] addr to EBX

	mov	$tss, %eax	# move TSS addr to EAX
	shl	$16, %eax
	mov	$103, %ax	# EAX = | TSS 0..15 | 103 |
	mov	%eax, (%ebx)

	mov	$tss, %ecx
	shr	$16, %ecx	# CX = TSS 16..31

	xor	%eax, %eax
	mov	%ch, %ah
	shl	$16, %eax
	mov	$0xE9, %ah	# flags: Present, DPL=11b, Not-Busy, Type of TSS
	mov	%cl, %al	# EAX = | TSS 23..31 | 00 | E9h | TSS 16..22 |
	mov	%eax, 4(%ebx)

# Load GDT into GDTR
	# Selector format: 0 -- Priv Lvl, 1 -- GDT/LDT, 2..15 -- Index
	mov	$gdt, %ebx
	push	%ebx		# ESP: GDT_Addr 0..31

	mov	$gdt_len, %eax
	sub	%ebx, %eax
	dec	%ax		# AX: number of GDT in bytes - 1
	push	%ax		# ESP: GDT_Len 0..15; GDT_Addr 16..47

	lgdt	(%esp)		# Load GDT from stack
	add	$6, %esp

	# Setup segmet registers for new GDT
	mov	$0x10, %ax	# mov AX 1; shl AX 8
	mov	%ax, %es	# Use 2nd index of GDT for all Data segments
	mov	%ax, %ss
	mov	%ax, %ds
	mov	%ax, %fs
	mov	%ax, %gs

	ljmp	$0x8, $far_jump	# CS set to 0x8 (1st ind GDT),  jump to CS:$far_jump
far_jump:

# Change Task Register to point to a new valid TSS
	mov	$gdt_tss, %eax
	sub	$gdt, %eax	# EAX = (GDT+TSS) - GDT  =  TSS_index * 8
	ltr	%ax

	pop	%ecx
	pop	%ebx
	pop	%eax
	ret
.size	set_gdt, . - set_gdt

