#include <REG51F380.H>
#include "types.h"


/*********************************************************/	
void System_Init(void){
   PCA0MD    &= ~0x40;
   PCA0MD    = 0x00;
   FLSCL     = 0x90;
   CLKSEL    = 0x03;
	
	 XBR1      = 0x40;
}



void fordelay(){
	
	long i = 0;
	for (i=0; i<=100000;i++){
	}
}

void main (void){
	
	System_Init();
	
	P2 = 0x00;
	
	while(1) {
		P2 = 0xFF;
		fordelay();
		P2 = 0x00;
		fordelay();
	}

}


