#ifndef	__IOPORT_H__
#define	__IOPORT_H__

/*
#define	inb(port, val)	{ __asm__ __volatile__ ("inb %%dx, %%al" : "=a"(val) : "d"(port)); }
#define	inw(port, val)	{ __asm__ __volatile__ ("inw %%dx, %%ax" : "=a"(val) : "d"(port)); }
*/
#define	outb(port, dat)	{ __asm__ __volatile__ ("outb %%al, %%dx" :: "a"(dat), "d"(port)); }
#define	outw(port, dat)	{ __asm__ __volatile__ ("outw %%ax, %%dx" :: "a"(dat), "d"(port)); }

static inline __attribute__ ((always_inline))
uint8_t
inb(uint16_t port) {
	uint8_t c;

	__asm__ __volatile__ ("inb %%dx, %%al" : "=a"(c) : "d"(port));
	return c;
}

static inline __attribute__ ((always_inline))
uint16_t
inw(uint16_t port) {
	uint16_t c;

	__asm__ __volatile__ ("inw %%dx, %%ax" : "=a"(c) : "d"(port));
	return c;
}

/*
#include <stdint.h>

inline uint8_t inb(uint16_t);
inline uint16_t inw(uint16_t);
inline void outb(uint16_t, uint8_t);
inline void outw(uint16_t, uint16_t);
*/

#endif /*__IOPORT_H__ */
