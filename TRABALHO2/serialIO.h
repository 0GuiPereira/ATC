#ifndef _SERIAL_IO_MODULE_
#define _SERIAL_IO_MODULE_

// https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/errno.h

#define ENODATA 61   /* No data available */
#define ENOBUFS 105  /* No buffer space available */

void uart0_initialize (void);

char uart0_putchar (unsigned char c);

char uart0_getchar (void);

unsigned char uart0_get_rxbufsize (void);

unsigned char uart0_get_txbufsize (void);


char _getkey (void);

char _getkey2 (void);

char putchar (char c);

void print_u0(char* p_message);



#endif // _SERIAL_IO_MODULE_