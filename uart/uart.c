#include <REG51F380.H>
#include <stdio.h>
#include "types.h"

/*********************************************************
 * function prototypes 																	 *
 *********************************************************/
// init uart 0
void uart0_init(void);
// our simple printf version
void print_u0(char* p_message);
// our simple scanf version 
char uart0_getc(char* c);
// our putchar version 
void uart0_putc(char c);

/*********************************************************/
		
code char message_u0[] = "Hello 8051!\r\n";

/*********************************************************/	
void Init_Device(void){
	
  /* Disable WDT */
	PCA0MD    = 0x00;
	/* select clock source */
	FLSCL     = 0x90;
	CLKSEL    = 0x03;
	/* enable cross-bar for I/Os */
  XBR1      = 0x40;
	/* enable uart0 with TX pin on P0.4 and RX pin on P0.5 */
	XBR0      = 0x01;
}

/*********************************************************
 * my main loop			  																	 *
 *********************************************************/
void main (void){
	
	// to store the received or transmitted byte
	char c = 0;	
	
	Init_Device();	
	uart0_init();
	
	/* Global Interrupt Enable */
	EA = 1;	
	
	/* print message using printf from stdlib.h */
	printf("Hello from printf()\n");
	
	/* print a char from our putchar */
	uart0_putc('L');
	uart0_putc('E');
	uart0_putc('T');
	uart0_putc('I');
	uart0_putc('\n');

	/* print a message using our print function */
	print_u0(message_u0);
			
	while(1){
	
		if(uart0_getc(&c)){
			if(c != 0)
				uart0_putc(c);
		}
	}
}

/*********************************************************
 * uart0 init				  																	 *
 *********************************************************/
void uart0_init(void){	
    /* UART0 Control                            */
		/* set REN0, enable reception								*/
    SCON0     = 0x10;
	
		/* Timer/Counter Mode                       */	
		TMOD      = 0x20;
	
    /* Clock Control                            */
    CKCON     = 0x08;
	
		/* Timer/Counter 1 High                     
		 * 0x30 generates a baudrate of 115200 bps	*/
    TH1       = 0x30;
	
		/* Timer 1 On/Off Control 		   	          */
		TR1 			= 1;
	
		/* UART0 TX Interrupt Flag                  */
		TI0 			= 1;
		RI0 = 1;
}
/*********************************************************
 * print using uart0  																	 *
 *********************************************************/
void print_u0(char* p_message){
	
	unsigned char i = 0;
	
	while(p_message[i] != 0){
		
		/* Transmission flag														*/
		while(TI0 == 0);
		
		/* UART write/read buffer (1 byte only)					*/
		SBUF0 = p_message[i];
		
		/* Transmission flag control. set by hardware.	*/
		TI0 = 0;
		i++;
	}
}

/*********************************************************
 * putc using uart0   																	 *
 *********************************************************/
void uart0_putc(char c){
	
	while(TI0 == 0);
	TI0 = 0;
	SBUF0 = c;
	
}

/*********************************************************
 * getc using uart0   																	 *
 *********************************************************/
char uart0_getc(char* c){

	if(RI0 == 1){
		RI0 = 0;
		*c = SBUF0; 
		return 1;
	}
	return 0;
}
 