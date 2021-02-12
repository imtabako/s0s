#ifndef	__IOPORT_H__
#define	__IOPORT_H__

#include <stdint.h>

#if 0
#define	outb(port, dat)	{ __asm__ __volatile__ ("outb %%al, %1" :: "a"(dat), "Nd"(port)); }
#define	outw(port, dat)	{ __asm__ __volatile__ ("outw %%ax, %1" :: "a"(dat), "Nd"(port)); }
#endif

static inline __attribute__ ((always_inline))
void
outb(uint16_t port, uint8_t data)
{
	__asm__ __volatile__ ("outb %%al, %1" :: "a"(data), "Nd"(port));
}

static inline __attribute__ ((always_inline))
void
outw(uint16_t port, uint8_t data)
{
	__asm__ __volatile__ ("outw %%ax, %1" :: "a"(data), "Nd"(port));
}

static inline __attribute__ ((always_inline))
uint8_t
inb(uint16_t port) {
	uint8_t res;

	__asm__ __volatile__ ("inb %1, %%al" : "=a"(res) : "Nd"(port));
	return res;
}

static inline __attribute__ ((always_inline))
uint16_t
inw(uint16_t port) {
	uint16_t res;

	__asm__ __volatile__ ("inw %1, %%ax" : "=a"(res) : "Nd"(port));
	return res;
}

static inline __attribute__ ((always_inline))
void
io_wait() {
	__asm__ __volatile__ ("outb	%al, $0x80");
}

#endif /*__IOPORT_H__ */
