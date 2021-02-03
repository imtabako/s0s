#if 0
#include "ioport.h"
#include <stdint.h>


inline uint8_t
inb(uint16_t port)
{
	uint8_t c;

	__asm__ __volatile__ ("inb %%dx, %%al" : "=a"(c) : "d"(port));
	return c;
}

inline uint16_t
inw(uint16_t port)
{
	uint16_t c;

	__asm__ __volatile__ ("inw %%dx, %%ax" : "=a"(c) : "d"(port));
	return c;
}

inline void
outb(uint16_t port, uint8_t data)
{
	__asm__ __volatile__ ("outb %%al, %%dx" :: "a"(data), "d"(port));
}

inline void
outw(uint16_t port, uint16_t data)
{
	__asm__ __volatile__ ("outw %%ax, %%dx" :: "a"(data), "d"(port));
}

#endif
