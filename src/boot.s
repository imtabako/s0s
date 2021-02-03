.set ALIGN,	1<<0			/* align loaded modules on page boundaries */
.set MEMINFO,	1<<1			/* provide memory map */
.set FLAGS,	ALIGN | MEMINFO		/* Multiboot `flag' field */
.set MAGIC,	0x1BADB002		/* `magic number' lets the bootloader find the header */
.set CHECKSUM,	-(MAGIC + FLAGS)	/* checksum of above, to prove we are multiboot */

.set KERNEL_BASE,	0XC0000000		# Higher half kernel base (3 Gb)
.set KERNEL_PAGE,	KERNEL_BASE >> 22	# Higher half kernel page table index


.section .multiboot
.align 4
.long	MAGIC
.long	FLAGS
.long	CHECKSUM


.section .bss
.align 16	# unnecessary actually
stack_bottom:
.skip 16384	# 16 KiB
stack_top:

.align	0x1000	# unnecessary actually
page_dir:
.skip	0x1000	# 1024 dwords (4Kb)
page_tbl0:
.skip	0x1000

.align	16	# unnecessary actually
.global tss
tss:
.skip	0x68	# 104 bytes (24 entries by 4 bytes)

.section .bss_end
.global END_KERNEL	# We link .bss as the last section, so  end of .bss = end of kernel
.global	END_KERNEL_ALIGNED
END_KERNEL:
.align	0x1000
END_KERNEL_ALIGNED:


.section .text
.global	_hstart
_hstart:
.global	_start
.set	_start, _hstart - KERNEL_BASE
.type	_hstart, @function
_hstart:
	# Page memory managment
	# 1) Map memory identically
	# 1.1) Set up first page table to map kernel space (0 -- 10A000)
	mov	$(END_KERNEL_ALIGNED - KERNEL_BASE), %ecx	# Size of kernel space
	sub	$0x1000, %ecx	# END_KERNEL is aligned, decrease by 1 page
	mov	%ecx, %esi
	or	$3, %esi	# ESI = page address | flags (Present, R/W)
	shr	$12, %ecx

.Lloop:	
	mov	%esi, page_tbl0-KERNEL_BASE(, %ecx, 4)
	sub	$0x1000, %esi
	loop	.Lloop

	# 1.2) Set up first page dir entry; DIR[0] = TBL0 | FLAGS
	mov	$(page_tbl0 - KERNEL_BASE), %edi	# PhysAddr of firt table
	or	$3, %edi				# EDI = TBL0 | flags (Presetn, R/W)
	mov	%edi, (page_dir - KERNEL_BASE)

	# 2) Also map first table to higher half (IND 0x300 in my case)
	mov	$KERNEL_PAGE, %ecx
	mov	%edi, page_dir-KERNEL_BASE(, %ecx, 4)

	# 3) Enable paging mechanism
	# 3.1) Move address of Page Directory to CR3
	mov	$(page_dir - KERNEL_BASE), %edi
	mov	%edi, %cr3

	# 3.2) Set paging bit (31 bit) in CR0
	mov	%cr0, %ecx
	or	$0x80000000, %ecx	# Paging Enabled bit
	mov	%ecx, %cr0
	# Paging now enabled	TODO: is JMP needed?

	lea	(_pg_jump), %ecx
	jmp	%ecx
_pg_jump:

	# 4) Unmap first table
	movl	$0x0, (%edi)
	# Paging is in higher half now

	/* Move value of top of the stack into esp (stack reg) */
	mov	$stack_top, %esp

	# Set IDTR_limit (2-byte) to 0; if any interrupt occurs, system will shut down
	# IDTR_base (4-byte) does not matter in this case (but it will be 0)
	sub	$6, %esp
	movw	$0, (%esp)
	lidt	(%esp)		# load IDTR from 6-byte memory unit
	add	$6, %esp

	/* ABI requires the stack is 16-byte aligned at the time of the call instruction */
	#  Note: nothing changes if stack is not 16b-aligned; TODO: discover what it means
	call	set_gdt

	call	set_idt

	push	%ebx
	push	%eax
	call	kernel_main
	add	$8, %esp

	/* 
	 * put the computer into an infinite loop: 
	 * 1) disable interrupts with `cli' 
	 * 2) wait for the next interrupt with `hlt'
	 * 3) jump to `hlt' instruction if it ever wakes up
	 */
	cli
1:	hlt
	jmp 1b

/* set size of _start symbol. Useful for debugging or call tracing */
.size	_hstart,	. - _hstart
