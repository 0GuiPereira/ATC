#include <REG51F380.H>


	/* enable cross-bar for I/Os 
	 * PB1 = 0.6; PB2 = 0.7;
	 * 7seg display = P2;
	 * uart0 tx = 0.4; uart rx = 0.5 
	 * i2c SDA = 0.2; i2c SCK = 0.3;
	*/

/*
SDA - P0.2
SCLK - P0.3
GND - GND
VCC - 5V
*/



// Peripheral specific initialization functions,
// Called from the Init_Device() function
void PCA_Init()
{
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x00;
}



void Port_IO_Init()
{
   	XBR1      = 0x40;
}


void Oscillator_Init()
{
    FLSCL     = 0x90;
    CLKSEL    = 0x03;
		P0MDOUT   = 0x04;
}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    PCA_Init();
		Oscillator_Init();
		Port_IO_Init();
	
}

void timerAndInterrups(){
	//Set by hardware when the Timer 2/3 high byte overflows from 0xFF to 0x00. In 16 bit 
	//mode, this will occur when Timer 2/3 overflows from 0xFFFF to 0x0000. 	
	TF2H = 0;	
	//TMR3CN &= ~(1 << B_TF3H);
	
	
	// Timer 2/3 Run Control. Timer 2/3 is enabled by setting this bit to 1. 
	//TMR3CN |= (1 << B_TR3);
	TR2 = 1;
	
	EA=1; //enable interrupts
}
