#include <REG51F380.H>
#include <stdio.h>
#include "config_platform.h"
#include "serialIO.h"


void main(void){
	char c = 0;

	Init_Device();
	uart0_initialize();
	EA=1;
		

	while(1) {
		//scanf("%c",&c);
		c = _getkey();
		putchar(c);
		//putchar('\n');
	}
	
}