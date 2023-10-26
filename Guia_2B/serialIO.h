#ifndef __SERIALIO_H__
#define __SERIALIO_H__

#define EXIT_SUCCESS 0
#define ENODATA 61
#define ENOBUFS 105

extern int uart0_putchar(unsigned char c);
extern int uart0_getchar(void);
extern unsigned char uart0_get_rxbuff_len(void);
extern unsigned char uart0_get_txbuff_len(void);

#endif
