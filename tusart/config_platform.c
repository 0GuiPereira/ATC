#include <REG51F380.H>


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
