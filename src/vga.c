#include "vga.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"
#include "ioport.h"
#include <stdarg.h>
#include <stdint.h>

uint16_t vga_crtc;

uint16_t *buf;
uint8_t height, width;
uint8_t color;

uint8_t x, y;

char num_buf[80];
char x_dig;
char sp_dig;

#define	vga_char(c, col)	( ((col & 0xFF) << 8) | (c & 0xFF) )
#define	vga_color(fg, bg)	( ((bg & 0x7)<<4) | (fg & 0xF) )

int	_num2str(uint32_t, int, char *, int);
void	_putc(char);
void	_puts(const char *);
void	_vga_update();

void
vga_init()
{
	int i, j;

	buf = (uint16_t *) VGA_BUF;
	width = VGA_WID;
	height = VGA_HEI;
	color = vga_color(VGA_COL_LGRAY, VGA_COL_BLACK);
	x = y = 0;

	i = inb(VGA_REG_MISC_R);
	vga_crtc = i & 1 ? 0x3D4 : 0x3B4;

	_vga_update();

	// Clear screen with spaces
	for (i = 0; i < height; ++i)
		for (j = 0; j < width; ++j)
			buf[i*width + j] = vga_char(' ', color);
}

void
_vga_update()
{
	uint8_t c;
	uint16_t pos;
	uint32_t i;

	pos = y * VGA_WID + x;

	c = inb(vga_crtc);
	outb(vga_crtc, 0xF);	// cursor low address
	outb(vga_crtc+1, (uint8_t) pos);
	outb(vga_crtc, 0xE);	// cursor high address
	outb(vga_crtc+1, (uint8_t) (pos >> 8));
	outb(vga_crtc, c);

	for (i = 80000000; i > 0; --i);
}


int
_num2str(uint32_t num, int base, char *str, int min_len)
{
	char *s;
	int len, n;
	uint32_t val;

	int siz, i;	// max len

	for (i = 0; i < 80; i++)
		num_buf[i] = 0;

	if (base != 2 && base != 8 && base != 10 && base != 16)
		return -1;

	siz = 80;
	val = num;
	len = 0;		// `len' will define how many digits needs to be written

	while (val > 0) {	// calculate `len'
		val /= base;
		len++;
	}

	if (min_len > len)
		len = min_len;

	if (len >= siz) {
		return -2;
	}

	s = str + len - 1;
	*(s+1) = '\0';

	while (num > 0) {
		n = num % base;
		num /= base;

		if (n >= 10)
			*s = x_dig + n-10;
		else
			*s = '0' + n;

		s--;
	}

	while (s >= str)
		*(s--) = sp_dig;

	return len;
}


/* TODO:
 *	left adjusment
 *	`#' flag
 *	`*' for argumens
 *	all other flags
 */
int
vga_printf(const char *fmt, ...)
{
	va_list ap;
	uint8_t mod;
	int base;
	char *ptr;

	int i;
	int minlen;
	int res;

	char c;
	char *s;
	uint32_t num;
	int32_t inum;

	res = 0;

	va_start(ap, fmt);
	ptr = fmt;
	base = 10;
	minlen = 0;
	x_dig = 'a';
	sp_dig = ' ';
	mod = 0;		// 00 - def(l),	01 - b,	02 - s,	04 - l,	08 - negative
				// 10 - zeros,	20 - ,	40 - ,	80 - 

	while (*ptr != '\0') {
		if (*ptr != '%') {
			base = 10;
			minlen = 0;
			mod = 0;
			x_dig = 'a';
			sp_dig = ' ';

			_putc(*ptr);
			ptr++;
			res++;
			continue;
		}
		ptr++;

skip_cond:
		;
		switch (*ptr) {
		case '%':
			_putc('%');

			res++;
			break;
		case 'c':
			c = va_arg(ap, int);	// `char' type does not exist here
			_putc(c);

			res++;
			break;
		case 's':
			s = va_arg(ap, char *);
			_puts(s);

			res += strlen(s);
			break;
		/* Numbers begin */
		case 'u':
unum:			num = va_arg(ap, uint32_t);
			goto print_num;
		case 'd':
		case 'i':
			inum = va_arg(ap, int32_t);
			if (inum < 0) {
				mod |= 8;
				inum = -inum;
			}
			num = (uint32_t) inum;
			goto print_num;
		case 'b':
			base = 2;
			goto unum;
		case 'o':
			base = 8;
			goto unum;
		case 'p':
			_puts("0x");
			res += 2;
		case 'x':
		case 'X':
			base = 16;
			x_dig = ( *ptr == 'p' ? 'a' : *ptr - 23 );
			goto unum;
		/* Flag characters */
		case '0':
			sp_dig = '0';
			ptr++;
			goto skip_cond;
		/* Length modifiers 
		 * LMs come first, so we can and should set `mod' value
		 */
		case 'h':
			if (*(ptr+1) == 'h') {
				ptr++;
				mod = 1;	// %hh - byte
			} else {
				mod = 2;	// %h - word
			}
			goto seek_num;
		case 'l':
			mod = 4;		// %l - dword
			goto seek_num;
		/* Field width */
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			i = 0;
			while (isdigit(*ptr))
				num_buf[i++] = *(ptr++);
			num_buf[i] = '\0';

			minlen = atoi(num_buf);
			goto skip_cond;
		default:			// wrong usage, return
			return -2;
		}

		ptr++;
		continue;


seek_num:	c = *(ptr+1);
		if (c != 'u' && c != 'd' && c != 'i' && c != 'x' && c != 'X' && c != 'o' && c != 'b') {
			return -2;
		}
		ptr++;
		goto skip_cond;


		/* Here we have `base',  `mod' and `num' */
print_num:	switch (mod & 0x7) {
		case 1:
			if (mod & 0x8)
				num = (uint8_t) (inum & 0xFF);
			else
				num = (uint8_t) (num & 0xFF);
			// num &= 0xFF;
			break;
		case 2:	
			if (mod & 0x8)
				num = (uint16_t) (inum & 0xFFFF);
			else
				num = (uint16_t) (num & 0xFFFF);
			// num &= 0xFFFF;
			break;
		case 0:
		case 4:
			// num &= 0xFFFFFFFF;
			break;
		}

		if (mod & 0x8) {
			_putc('-');
			res++;
		}
		res += _num2str(num, base, num_buf, minlen);

		_puts(num_buf);
		ptr++;
	}
	_vga_update();

	va_end(ap);
	return res;
}


void
vga_putc(char c)
{
	int len, i;

	switch (c) {
	case '\n':
		y++;
		x = 0;
		break;
	case '\r':
		x = 0;
		break;
	case '\t':
		len = 8 - (x % 8);

		for (i = x; i <= x+len && i < width; i++)
			buf[y*width + i] = vga_char(' ', color);
		x += len;
		break;
	case '\v':
	case '\b':
	default:
		buf[y*width + x] = vga_char(c, color);
		x++;
	}

	if (x >= width) {
		x = 0;
		y++;
	}
	if (y >= height) {	// scroll
		y = height-1;
		vga_scroll();
	}

	_vga_update();
}

/* the same but used only inside vga.c so no cursor update */
void
_putc(char c)
{
	int len, i;

	switch (c) {
	case '\n':
		y++;
		x = 0;
		break;
	case '\r':
		x = 0;
		break;
	case '\t':
		len = 8 - (x % 8);

		for (i = x; i <= x+len && i < width; i++)
			buf[y*width + i] = vga_char(' ', color);
		x += len;
		break;
	case '\v':
	case '\b':
	default:
		buf[y*width + x] = vga_char(c, color);
		x++;
	}

	if (x >= width) {
		x = 0;
		y++;
	}
	if (y >= height) {	// scroll
		y = height-1;
		vga_scroll();
	}
}

void
vga_puts(const char *str)
{
	char *s;

	s = str;
	while (*s != '\0')
		_putc(*(s++));

	_vga_update();
}

/* the same but used only inside vga.c so no cursor update */
void
_puts(const char *str)
{
	char *s;

	s = str;
	while (*s != '\0')
		_putc(*(s++));
}

void
vga_scroll()
{
	int i, j;

	for (i = 0; i < height-1; i++)
		for (j = 0; j < width; j++)
			buf[i*width + j] = buf[(i+1)*width + j];

	for (j = 0; j < width; j++)
		buf[i*width + j] = vga_char(' ', color);
}

	
#if 0
	uint8_t c;

	/* First we must discover which text mode we're in */
	outb(0x3CE, 6);		// graphics reg (port 3CEh), misc reg (index 6)
	c = inb (0x3CE+1);
	c >>= 2;
	c &= 3;

	switch (c) {
	0:			// 128K
	1:	buf = 0xA0000;	// 64K
		break;
	2:	buf = 0xB0000;	// 32K
		break;
	3:	buf = 0xB8000;	// 32K   this mode will be used, others not supported yet
		height = 80;
		width = 25;
		break;
	}
#endif

