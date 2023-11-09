#include <REG51F380.H>
#include "types.h"

/*********************************************************/
sbit seg_dp = P2^7;

static int blink = 100;
/*********************************************************/	
void System_Init(void){
   PCA0MD    = 0x00;
   FLSCL     = 0x90;
   CLKSEL    = 0x03;
	 XBR1      = 0x40;
}
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

/*********************************************************
 * my main loop			  																	 *
 *********************************************************/
void main (void){
	
	
	System_Init(); 
	
	timer2_init_auto(-40000);

	//Set by hardware when the Timer 2 high byte overflows from 0xFF to 0x00. In 16 bit 
	//mode, this will occur when Timer 2 overflows from 0xFFFF to 0x0000. 	
	TF2H = 0;	
	// Enable Flag Timer 2 Overflow
	ET2 = 1;
	// Timer 2 Run Control. Timer 2 is enabled by setting this bit to 1. 
	TR2 = 1;
	
	while(1){
		
		while(!TF2H);
		TF2H = 0;
		
//		seg_dp ^=1;	

		if(--blink == 0){
			seg_dp ^= 1;
			blink = 100;
		}	
	}
}