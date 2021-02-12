#ifndef	__PIC_H__
#define	__PIC_H__

/* i/o ports */
#define PIC1		0x20
#define PIC2		0xA0
#define PIC1_CMD	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_CMD	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_OFFSET	0x20		// int vector offset, 0-31 are reserved

#define	ICW1_IC4	(1 << 0)	// set if ICW4 is needed (BUF, 8086)
#define	ICW1_SNGL	(1 << 1)	// set if single PIC
#define	ICW1_ADI	(1 << 2)	// no effect for 8086
#define	ICW1_LTIM	(1 << 3)	// set if Level triggered mode
#define	ICW1_D4		(1 << 4)	// `1' indicates ICW1

/* For ICW2,3 no bit flags are set; they correspond to IRs and offsets */

#define	ICW4_PM		(1 << 0)	// processor mode; 1 for 8086
#define	ICW4_AEOI	(1 << 1)	// set if auto EOI
#define	ICW4_MS		(1 << 2)	// set if master; used with BUF only
#define	ICW4_BUF	(1 << 3)	// set if buffered mode
#define	ICW4_BUF_SLAVE	ICW4_BUF
#define	ICW4_BUF_MASTER	(ICW4_BUF | ICW4_MS)
#define	ICW4_SFNM	(1 << 4)	// set if special fully nested mode

/* defaults */
#define	ICW1		(ICW1_IC4 | ICW1_D4)	// 0x11
#define	ICW2		PIC_OFFSET
#define	ICW3_MASTER	0x4		// indicates that IR2 has a slave
#define	ICW3_SLAVE	0x2		// ID `2' is a slave
#define	ICW4		ICW4_PM

void	pic_init();

void	pic_remap(int offset1, int offset2);

void	pic_mask(unsigned int irq);

void	pic_unmask(unsigned int irq);


#endif	/* __PIC_H__ */

