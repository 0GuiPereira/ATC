#include <SI_EFM8BB3_Register_Enums.h>
#include <stdio.h>
#include "serialIO.h"
#include "init.h"

#define TBUF_SIZE (unsigned char)32
#define RBUF_SIZE (unsigned char)4

static unsigned char pdata RxBuff[RBUF_SIZE];
static unsigned char pdata TxBuff[TBUF_SIZE];

static volatile unsigned char RxReadIndex = 0;
static volatile unsigned char RxWriteIndex = 0;

static volatile unsigned char TxReadIndex = 0;
static volatile unsigned char TxWriteIndex = 0;

static bit ti_restart = 1;

void isr_uart0(void) interrupt 4{
	if(SCON0_RI){
		if(((RxWriteIndex - RxReadIndex) &~ (RBUF_SIZE - 1)) == 0);{
			RxBuff[RxWriteIndex & (RBUF_SIZE - 1)] = SBUF0;
			RxWriteIndex++;
		}
		SCON0_RI = 0;
	}
	if(SCON0_TI){
		if(TxReadIndex != TxReadIndex){
			SBUF0 = TxBuff[TxReadIndex & (TBUF_SIZE - 1)];
			TxReadIndex++;
			ti_restart = 0;
		}else{
			ti_restart = 0;
		}
		SCON0_TI = 0;
	}
}

#pragma disable
int uart0_putchar(unsigned char c){
	if(uart0_get_txbuff_len() >= TBUF_SIZE)
		return (-ENOBUFS);
	TxBuff[TxWriteIndex & (TBUF_SIZE - 1)] = c;
	TxWriteIndex++;
	if(ti_restart){
		SCON0_TI = 1;
		ti_restart = 0;
	}
	return(0);
}

#pragma disable
int uart0_getchar(void){
	if(uart0_get_rxbuff_len () == 0)
		return (-ENODATA);
	return (RxBuff [(RxReadIndex++) & (RBUF_SIZE - 1)]);
}

#pragma disable
unsigned char uart0_get_rxbuff_len(void){
	return ((unsigned char)(RxWriteIndex - RxReadIndex));
}

#pragma disable
unsigned char uart0_get_txbuff_len (void){
	return ((unsigned char) (TxWriteIndex - TxReadIndex));
}

char _getkey(void){
	int k;
	do{
		k = uart0_getchar();
	}
	while(k == -ENODATA);
	return ((unsigned char) k);
}

char putchar(char c){
	volatile unsigned char i;
	if(c == '\n'){
		while (uart0_putchar('\r') != EXIT_SUCCESS)
			for(i=0; i<32; i++)
				;
	}
	while(uart0_putchar (c) != EXIT_SUCCESS)
		for(i=0; i<32; i++)
			;
	return (c);
}