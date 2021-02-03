/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)
 
/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* 
	 * Higher half kernel trick is to virtually map kernel space to some
	 * place (3 GiB) so as user space may begin at address 0
	 * 
	 * Begin putting sections at 1 MiB, a conventional place for kernels to be
	 * loaded at by the bootloader
	 */
	. = 0xC0100000;
 
	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.text BLOCK(4K) : AT ( ADDR (.text) - 0xC0000000 )
	{
		*(.multiboot)
		*(.text)
	}
 
	/* Read-only data. */
	.rodata BLOCK(4K) : AT ( ADDR (.rodata) - 0xC0000000 )
	{
		*(.rodata)
	}
 
	/* Read-write data (initialized) */
	.data BLOCK(4K) : AT ( ADDR (.data) - 0xC0000000 )
	{
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : AT ( ADDR (.bss) - 0xC0000000 )
	{
		*(COMMON)
		*(.bss)
		*(.bss_end)
	}
 
	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
}
