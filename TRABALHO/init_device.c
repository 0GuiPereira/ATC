#include <REG51F380.H>

void init_device(void){
	
  /* Disable WDT */
	PCA0MD    = 0x00;
	/* select clock source */
	FLSCL     = 0x90;
	CLKSEL    = 0x03;
	/* enable cross-bar for I/Os 
	 * PB1 = 0.6; PB2 = 0.7;
	 * 7seg display = P2;
	 * uart0 tx = 0.4; uart rx = 0.5 
	 * i2c SDA = 0.2; i2c SCK = 0.3;
	*/

	// To Work with the i2c/LCD
	P0MDOUT   = 0x04;
	
  XBR1      = 0x40;
	/* enable uart0 with TX pin on P0.4 and RX pin on P0.5 */
	XBR0      = 0x01;

}