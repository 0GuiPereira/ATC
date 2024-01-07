#include <REG51F380.H>
#include <stdio.h>
#include "config_platform.h"
#include "serialIO.h"

void test(void){


	while(1) {
		//scanf("%c",&c);
		char c = _getkey();
		putchar(c);
		putchar('\n');
	}
}