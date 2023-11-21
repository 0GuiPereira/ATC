#include <REG51F380.H>
#include "init_device.h"


/*********************************************************/	
void Init_Device(void)
{
	PCA0MD    = 0x00;
  /* select clock source */
	FLSCL     = 0x90;		
  CLKSEL    = 0x03;  	
	
	/* enable cross-bar for I/Os */
	XBR1      = 0x41;
	
	//P1.4 output LOCKED
	P1MDOUT |= 0x10;
	
	//P1.6 output ALARM
	P1MDOUT |= 0x40;
	
	//P1.7 input HWFLAG
	 P1MDIN |= 0x80;

	
	

}

/*********************************************************/	
