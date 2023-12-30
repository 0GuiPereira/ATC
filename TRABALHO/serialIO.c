#include <REG51F380.H>
#include "serialIO.h"
#include <string.h>

/*------------------------------------------------------------------------------
Notes:

The length of the receive and transmit buffers must be a power of 2.

Each buffer has a WriteIndex and a ReadIndex.

If WriteIndex = ReadIndex, the buffer is empty.

(ReadIndex -WriteIndex)% buffer_size = the number of characters in the buffer.
------------------------------------------------------------------------------*/

#define TBUF_SIZE   16           // Must be a power of 2 AND 2 <= SIZE <= 128
#define RBUF_SIZE   32           // Must be a power of 2 AND 2 <= SIZE <= 128


static unsigned char pdata TxBuff [TBUF_SIZE];
static unsigned char pdata RxBuff [RBUF_SIZE];

static volatile unsigned char TxWriteIndex = 0;
static volatile unsigned char TxReadIndex = 0;

static volatile unsigned char RxWriteIndex = 0;
static volatile unsigned char RxReadIndex = 0;

static bit ti_restart = 1;  /* Not Zero if TI = 1 is required */

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void uart0_isr (void) interrupt 4
{
	/*------------------------------------------------
	Received data interrupt.
	------------------------------------------------*/
	if (RI0 != 0)
	{
		RI0 = 0;
			//is there space left?
		if (((RxWriteIndex - RxReadIndex) & ~(RBUF_SIZE-1)) == 0)
		{
			RxBuff [RxWriteIndex & (RBUF_SIZE-1)] = SBUF0;
			RxWriteIndex++;
		}
	}

	/*------------------------------------------------
	Transmitted data interrupt.
	------------------------------------------------*/
	if (TI0 != 0)
	{
		
		//is there any byte to send?
		if (TxWriteIndex != TxReadIndex)
		{
			SBUF0 = TxBuff [TxReadIndex & (TBUF_SIZE-1)];
			TxReadIndex++;
			ti_restart = 0;
		}
		else //subsequent insertion in buffer tbuf will require
			 // TI to be set to 1 so that machine starts to pump bytes again
			ti_restart = 1;
		TI0 = 0;
	}

}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable
void uart0_initialize (void)
{
	
//---------------------------------------------------------
//	XBR0: Port I/O Crossbar Register 0
//---------------------------------------------------------
//	7		|		6		|		5		|		4  |	3		|		2	 |	1		|	0		|
//---------------------------------------------------------
// CP1AE| CP1E  | CP0AE | CP0E |SYSCKE| SMB0E| SPI0E|URT0E|
//---------------------------------------------------------
#define B_URT0E 0
	XBR0      = (1<<B_URT0E) ;	
	
	//---------------------------------------------------------
//	 SCON0: Serial Port 0 Control
//---------------------------------------------------------
//	7		|		6		|		5		|		4  |	3		|		2	 |	1		|	0		|
//---------------------------------------------------------
//S0MODE|   -		| MCE0  | REN0 | TB80 | RB80 | TI0  | RI0 |
//---------------------------------------------------------
#define B_REN0 4
	
	SCON0= 0;
	SCON0 |=(1<<B_REN0);
    	
//---------------------------------------------------------
//	 CKCON: Clock Control
//---------------------------------------------------------
//	7		|		6		|		5		|		4  |	3		|		2	 |	1		|	0		|
//---------------------------------------------------------
// T3MH | T3ML  | T2MH  |	T2ML | T1M  | T0M  |   SCA[1:0]
//---------------------------------------------------------
#define B_T1M 3
#define B_SCA1 1
#define B_SCA0 0
	
	//CKCON     = 0x08;//	 clock is prescaled by sca[1:0] = 0 => sysclk/12
	CKCON &=~((1<<B_SCA1)|(1<<B_SCA0));
	CKCON |= (1<<B_T1M);
	
	//---------------------------------------------------------
//	 TMOD:
//---------------------------------------------------------
//	7		|		6		|		5		|		4  |	3		|		2	 |	1		|	0		|
//---------------------------------------------------------
// GATE1| C/T1  |    T1M[1:0]  | GATE0| C/T0 |  T0M[1:0]  |
//---------------------------------------------------------
#define B_T1M1 5
#define B_T1M0 4

		//TMOD      = 0x20; // mode 2 8 bit auto-reload
		TMOD &= ~(1<<B_T1M0);
		TMOD |=	 (1<<B_T1M1);
		
    TH1       = 0x30;
		TR1=1;
	
//---------------------------------------------------------
//	 IE: Interrupt Enable
//---------------------------------------------------------
//	7		|		6		|		5		|		4  |	3		|		2	 |	1		|	0		|
//---------------------------------------------------------
// 	EA 	| ESPI0 |	 ET2 	|  ES0 |  ET1 |  EX1 |  ET0 | EX0 |
//---------------------------------------------------------	
#define B_EA 7
#define B_ES0 4
		IE |= (1<<B_ES0);

}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable
char uart0_putchar ( unsigned char c )
{
	/*------------------------------------------------
	If the buffer is full, return an error value.
	------------------------------------------------*/
	if (uart0_get_txbufsize () >= TBUF_SIZE)
	  return (-ENOBUFS);

	/*------------------------------------------------
	Add the data to the transmit buffer.  If the
	transmit interrupt is disabled, then enable it.
	------------------------------------------------*/
	TxBuff [TxWriteIndex & (TBUF_SIZE - 1)] = c;
	TxWriteIndex++;

	if (ti_restart)
	{
		ti_restart = 0;
		TI0 = 1;               /* generate transmit interrupt */
	}

	return (0);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable
char uart0_getchar (void)
{
	if (uart0_get_rxbufsize () == 0)
	  return (-ENODATA);

	return (RxBuff [(RxReadIndex++) & (RBUF_SIZE - 1)]);
}
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable
unsigned char uart0_get_rxbufsize (void)
{
	return (RxWriteIndex - RxReadIndex);
}
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#pragma disable
unsigned char uart0_get_txbufsize (void)
{
	return (TxWriteIndex - TxReadIndex);
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
_getkey waits until a character is received from the serial port. i.e. if the buffer is empty, this
function hangs waiting for a character to be received.
------------------------------------------------------------------------------*/
char _getkey (void)
{
	int k;

	do
	{
		k = uart0_getchar ();
	}
	while (k == -ENODATA);

	return ((unsigned char) k);
}
/*------------------------------------------------------------------------------
_getkey2 does not waits until a character is received from the serial port.  
if the buffer is empty, this function will return -1.
------------------------------------------------------------------------------*/
char _getkey2 (void)
{
	return uart0_getchar ();
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
char putchar (char c)
{
	volatile unsigned int i;
	
	if(c == '\n')	//on '\n', translate into '\r\n'
	{
		//try to enqueue the char, wait until it is accepted
		while (uart0_putchar ('\r') != 0)
			for (i=0; i<10; i++) //dummy sleep
				;
	}
	//try to enqueue the char, wait until it is accepted
	while (uart0_putchar (c) != 0)
		for (i=0; i<10; i++)	//dummy sleep
			;

	return (c);
}
/*----------------------------------------------------------------------
-------------------------------------------------------------------------*/
