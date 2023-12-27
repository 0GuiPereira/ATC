#include <REG51F380.H>
#include "types.h"
#include "i2c-bitbang.h"

/*********************************************************
 * function prototypes 																	 *
 *********************************************************/
 void init_device(void);
 
/*********************************************************/	
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
  XBR1      = 0x40;
	/* enable uart0 with TX pin on P0.4 and RX pin on P0.5 */
	XBR0      = 0x01;
}

/*********************************************************
 * main loop			  																		 *
 *********************************************************/
void main(void){

	unsigned char ack = 1;
	unsigned char rdata = 0;
	
	init_device();

	/* sequence to test i2c bitbanging functions */
	
	// start condition
	i2c_start();
	// write byte
	i2c_write(0xA0); 		
	// read ack
	if(read_ack() == NACK){
		while(1);
	};
	// write byte
	i2c_write(0xAC); 
	// read ack
	if(read_ack() == NACK){
		while(1);
	};
	// write byte
	i2c_write(0x10);	
	// read ack
	if(read_ack() == NACK){
		while(1);
	};
	// read some data from i2c bus
	rdata = i2c_read();
	
	// write an ack/nack signal
	write_ack(ACK);                   // output ACK/NAK bit
	
	// stop condition
	i2c_stop();	

	while(1);
	
}