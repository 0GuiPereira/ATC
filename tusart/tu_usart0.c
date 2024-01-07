#include <REG51F380.H>
#include <stdio.h>
#include "config_platform.h"
#include "serialIO.h"
#include "timer2_delay.h"

char code message0[] = "Hello uart0 m0\r\n";
char code message1[] = "Hello uart0 m1\r\n";

void main(void){
	char c = 0;
	char code *ptr;
	Init_Device();
	uart0_initialize();

	timer2_init_auto(-40000);

	//Set by hardware when the Timer 2 high byte overflows from 0xFF to 0x00. In 16 bit 
	//mode, this will occur when Timer 2 overflows from 0xFFFF to 0x0000. 	
	TF2H = 0;	
	// Enable Flag Timer 2 Overflow
	// Timer 2 Run Control. Timer 2 is enabled by setting this bit to 1. 
	TR2 = 1;
	
	
	EA=1;
	
		
	for(ptr = (char code*)message0; *ptr!= '\0'; ptr++){
		while(uart0_putchar(*ptr)== -ENOBUFS);
	}
	
	printf(message1);
	
	
	while(1) {
		c = _getkey();
		putchar(c);
		//putchar('\n');
	}
	
}