#include <REG51F380.H>


/*********************************************************/
/* timer2 init code here */

void timer2_init_auto(int reload){

//SFR Definition 25.9. TMR2CN: Timer 2 Control
//---------------------------------------------------------------
//Bit |	 7	 |	 6  |		5		 |	4		 |	3 		|	2 	|		1	 |	0
//---------------------------------------------------------------
//Name|	TF2H | TF2L | TF2LEN |TF2CEN |T2SPLIT |	TR2 |T2CSS |T2XCLK
//---------------------------------------------------------------
	// sysclock /12
	TMR2CN = 0;

#define B_T2MH 5	
//SFR Definition 25.1. CKCON: Clock Control
//-------------------------------------------------------------
//Bit |	 7	 |	 6  |		5	 |	4	 |	3	|	2 	|		1	 |	0
//-------------------------------------------------------------
//Name|	T3MH | T3ML | T2MH |T2ML |T1M |	T0M |		SCA[1:0]
//-------------------------------------------------------------	
	
	//Timer 2 high byte T2MH and low byte T2ML use the system clock
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

