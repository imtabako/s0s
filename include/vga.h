#ifndef	__VGA_H__
#define	__VGA_H__


#define	VGA_REG_MISC_R	0x3CC
#define	VGA_REG_MISC_W	0x3C2
// CRTC reg
#define	VGA_IND_CURSORLOC_L	0x0F
#define	VGA_IND_CURSORLOC_H	0x0E

#define	VGA_COL_BRIGHT	(1 << 3)

#define	VGA_COL_BLACK	0
#define	VGA_COL_BLUE 	1
#define	VGA_COL_GREEN	2
#define	VGA_COL_CYAN 	3
#define	VGA_COL_RED  	4
#define	VGA_COL_MAGEN	5
#define	VGA_COL_BROWN	6
#define	VGA_COL_LGRAY	7

#define	VGA_COL_DARK_GRAY	( VGA_COL_BLACK	| VGA_COL_BRIGHT )
#define	VGA_COL_LIGHT_BLUE	( VGA_COL_BLUE	| VGA_COL_BRIGHT )
#define	VGA_COL_LIGHT_GREEN	( VGA_COL_GREEN	| VGA_COL_BRIGHT )
#define	VGA_COL_LIGHT_CYAN	( VGA_COL_CYAN	| VGA_COL_BRIGHT )
#define	VGA_COL_LIGHT_RED 	( VGA_COL_RED	| VGA_COL_BRIGHT )
#define	VGA_COL_LIGHT_MAGEN	( VGA_COL_MAGEN	| VGA_COL_BRIGHT )
#define	VGA_COL_YELLOW		( VGA_COL_BROWN	| VGA_COL_BRIGHT )
#define	VGA_COL_WHITE		( VGA_COL_LGRAY	| VGA_COL_BRIGHT )

#define VGA_BUF	(0xB8000 | 0xC0000000)
#define VGA_WID	80
#define VGA_HEI	25


void	vga_init();

void	vga_getpos(int *resx, int *resy);

void	vga_putc(char c);

void	vga_puts(const char *str);

int	vga_printf(const char *fmt, ...);

void	vga_backsp();

void	vga_scroll();

#endif	/* __VGA_H__ */

