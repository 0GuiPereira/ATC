#include <REG51F380.H>
#include "delay.h"


void timer2_init_auto(int reload){

	TMR2CN = 0;

	#define B_T2MH 5	

	CKCON &= ~((1 << B_T2MH));
	
	TMR2H = (reload) >> 8;
	TMR2L = (reload);

	TMR2RLH = (reload) >> 8;
	TMR2RLL = (reload);
	
}

void delay_s(unsigned char s) {
    unsigned char i = 0;

    while(i != (s*100)) {
        i++;
        while(!TF2H);
        TF2H = 0;
    }
}