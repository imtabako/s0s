#include "pic.h"
#include "vga.h"
#include "ioport.h"
#include <stdint.h>

#define	NELEMS(x)	(sizeof(x) / sizeof((x)[0]))

// extern char *buf;

#define	KBD_SIZ	64
/*
	unsigned char kbd_buf[KBD_SIZ];
	unsigned char *kbd_stack;
	int kbd_curr = 0;
*/

unsigned char kbd_scan[] = {
    0x10, 'q', 0x11, 'w', 0x12, 'e', 0x13, 'r',
    0x14, 't', 0x15, 'y', 0x16, 'u', 0x17, 'i',
    0x18, 'o', 0x19, 'p', 0x1A, '[', 0x1B, ']',
    0x1E, 'a', 0x1F, 's', 0x20, 'd', 0x21, 'f',
    0x22, 'g', 0x23, 'h', 0x24, 'j', 0x25, 'k',
    0x26, 'l', 0x27, ';', 0x28, '\'', 0x2C, 'z',
    0x2D, 'x', 0x2E, 'c', 0x2F, 'v', 0x30, 'b',
    0x31, 'n', 0x32, 'm', 0x33, ',', 0x34, '.',
    0x35, '/', 0x39, ' '
};
unsigned char kbd_flags = 0;

void _kbd_shit();

void
kbd_handler(short key)
{
	int x, y;
	char c;
	int i;

	key &= 0xFF;

	switch (key) {
	case 0x1C:	/* ENTER */
		vga_putc('\n');
		return;
	case 0x0F:	/* TAB */
		vga_putc('\t');
		return;
	case 0x0E:
		vga_backsp();
		return;
	case 0x3A:	/* CAPS */
		kbd_flags ^= 0x01;
		return;
	case 0x2A:	/* SHIFT */
		return;
	case 0x01:	/* ESC */
		
		_kbd_shit();
	}

	if (key >= 0x3B && key <= 0x40) {
		_vga_chcl(key-0x3B);
		return;
	}

	for (i = 0; i < NELEMS(kbd_scan); i += 2)
		if (key == kbd_scan[i])
			break;

	if (i >= NELEMS(kbd_scan))
		return;

	c = kbd_scan[i+1];
	if (kbd_flags & 0x01)
		c -= 0x20;
	vga_putc(c);

	return;
}

char _sos[] = \
" ____  _                 _         ___  ____  \n"
"/ ___|(_)_ __ ___  _ __ | | ___   / _ \\/ ___| \n"
"\\___ \\| | '_ ` _ \\| '_ \\| |/ _ \\ | | | \\___ \\ \n"
" ___) | | | | | | | |_) | |  __/ | |_| |___) |\n"
"|____/|_|_| |_| |_| .__/|_|\\___|  \\___/|____/ \n"
"                  |_|                         \n";

uint16_t _shit[] = {
    0x96e2, 0x8896, 0xe280, 0x96e2, 0x9196, 0xe291, 
    0x96e2, 0x8096, 0xe291, 0x96e2, 0x8096, 0xe291, 
    0xe288, 0x96e2, 0x9196, 0xe291, 0x96e2, 0x8096, 
    0xe291, 0x96e2, 0x8896, 0xe288, 0x96e2, 0x8096, 
    0x8096, 0xe291, 0x96e2, 0x9196, 0xe280, 0x96e2, 
    0x8096, 0xe291, 0x96e2, 0x8096, 0xe280, 0xe20a, 
    0x96e2, 0x8896, 0xe291, 0x96e2, 0x8096, 0xe288, 
    0x96e2, 0x9196, 0xe291, 0x96e2, 0x8096, 0xe280, 
    0x96e2, 0x9196, 0x9196, 0xe291, 0x96e2, 0x8896, 
    0xe291, 0x96e2, 0x8896, 0xe280, 0x96e2, 0x8896, 
    0xe291, 0x96e2, 0x8896, 0xe291, 0xe280, 0x96e2, 
    0x9196, 0xe280, 0x96e2, 0x9196, 0xe291, 0x96e2, 
    0x8096, 0xe280, 0x96e2, 0x9196, 0xe280, 0x96e2, 
    0x96e2, 0x8896, 0xe288, 0x96e2, 0x9196, 0xe291, 
    0x96e2, 0x8896, 0xe291, 0xe288, 0x96e2, 0x9196, 
    0xe288, 0x96e2, 0x9196, 0xe288, 0x96e2, 0x9196, 
    0x9196, 0xe291, 0x96e2, 0x8096, 0xe280, 0x96e2, 
    0x8096, 0xe280, 0x000a
};

void
_kbd_shit()
{
	vga_puts(_sos);
	vga_puts("\n\n");
	_vga_cp(_shit, NELEMS(_shit));
}

void
pic_init()
{
	pic_remap(PIC_OFFSET, PIC_OFFSET+8);
//	pic_mask(2);			// IRQ 2 is used for cascade only

	outb(PIC1_DATA, 0xFC);		// mask all IRQs but first two
	outb(PIC2_DATA, 0xFF);		// mask all IRQs on slave
}

void
pic_remap(int offset1, int offset2)
{
	uint8_t a1, a2;

	a1 = inb(PIC1_DATA);		// save masks
	a2 = inb(PIC2_DATA);		// save masks

	outb(PIC1_CMD, ICW1);		// ICW1: cascade mode, wait for ICW4
	io_wait();
	outb(PIC2_CMD, ICW1);
	io_wait();

	outb(PIC1_DATA, offset1);	// ICW2: vector offsets
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();

	outb(PIC1_DATA, ICW3_MASTER);	// ICW3: mask and id for slave PIC
	io_wait();
	outb(PIC2_DATA, ICW3_SLAVE);
	io_wait();

	outb(PIC1_DATA, ICW4);		// ICW4: 8086 mode, no AOEI, no buf
	io_wait();
	outb(PIC2_DATA, ICW4);
	io_wait();

	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void
pic_mask(unsigned int irq)
{
	uint16_t port;
	uint8_t mask;

	if (irq < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		irq -= 8;
	}

	mask = inb(port);
	mask |= 1 << irq;

	outb(port, mask);
}

void
pic_unmask(unsigned int irq)
{
	uint16_t port;
	uint8_t mask;

	if (irq < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		irq -= 8;
	}

	mask = inb(port);
	mask &= ~(1 << irq);

	outb(port, mask);
}
