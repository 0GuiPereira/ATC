#include <REG51F380.H>

sbit seg_A = P2^7;

void init_device(void)
{
    PCA0MD  = 0x00;
    // enable cross-bar for I/Os
    XBR1    = 0x40;
}

void configureTimer() {
    TMOD |= 0x01; // Set Timer 0 to Mode 1 (16-bit mode)
    TH0 = 0x4C;  // Set the high byte of the initial value for 0.5-second delay
    TL0 = 0x00;  // Set the low byte of the initial value

    // Enable Timer 0 interrupt
    ET0 = 1;
    EA = 1;

    // Start Timer 0
    TR0 = 1;
}

void timer0_isr() interrupt 1 {
    // Reset Timer values for a 0.5-second delay
    TH0 = 0x26;
    TL0 = 0x00;

    // Toggle seg_A pin
    seg_A = ~seg_A;
}

void main()
{
    init_device();

    configureTimer();

    while (1)
    {
        // Your main program code here
    }
}
