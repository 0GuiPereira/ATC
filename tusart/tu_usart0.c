#include <REG51F380.H>
#include <stdio.h>
#include "config_platform.h"
#include "serialIO.h"


void main(void){
	char c = 0;
	char code *ptr;
	Init_Device();
	uart0_initialize();
	EA=1;
//		
//	for(ptr = (char code*)message0; *ptr!= '\0'; ptr++){
//		while(uart0_putchar(*ptr)== -ENOBUFS);
//	}
//	
//	//printf(message1);
//	
	
	while(1) {
		//scanf("%c",&c);
		c = _getkey();
		putchar(c);
		//putchar('\n');
	}
	
}