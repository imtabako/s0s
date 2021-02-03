#include "vga.h"
#include "stdlib.h"
#include <stdint.h>
#include <stddef.h>

extern void _end_kernel, _end_text;
extern void END_KERNEL, END_KERNEL_ALIGNED;

int
kernel_main(int eax, int ebx)
{
	vga_init();

	vga_printf("EAX: %X\t EBX: %X\n", eax, ebx);
	vga_printf("sizeofs:\nint = %d\tintmax = %d\tuint32 = %d\tuint16 = %d\tsize_t = %d\n",
		   sizeof (int), sizeof (intmax_t), sizeof (uint32_t), sizeof (uint16_t), sizeof (size_t));

	vga_printf("%d\t%d\t%d\n", atoi("1"), atoi("12345"), atoi("0"));

	vga_printf("|%15x|\n\n", 123456);
	vga_printf("|%d|\n", 12345);
	vga_printf("|%4d|\n", 12345);
	vga_printf("|%40d|\n", 12345);
	vga_printf("|%040d|\n", 12345);
	vga_printf("|%40hhd|\n", 12345);
	vga_printf("%d_%u_%x_%o\n\n", 213, 2, 1, 4);

	vga_printf("END_KERNEL:\t%X\taligned: %X\n", &END_KERNEL, &END_KERNEL_ALIGNED);

	return 0;
}
