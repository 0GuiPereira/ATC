#include <REG51F380.H>
#include <stdio.h>
#include "types.h"
#include "lcd1602.h"

sbit PB1 = P0^6;

// LCD ADDRESS 0x27
#define LCD_ADDR_W 0x4E
#define LCD_ADDR_R 0x4F

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

	// To Work with the i2c/LCD
	P0MDOUT   = 0x04;
	
  XBR1      = 0x40;
	/* enable uart0 with TX pin on P0.4 and RX pin on P0.5 */
	XBR0      = 0x01;

}

/*********************************************************
 * main loop			  																		 *
 *********************************************************/
void main(void){
	
	int rc;
	
	init_device();
	
	while(PB1);
	while(!PB1);
	
	rc = lcd1602Init(LCD_ADDR_W);
	if (rc)
	{
		while(1);
	}
	
		while(1){
		lcd1602Clear(LCD_ADDR_W);	    // clear the memory

		lcd1602Control(LCD_ADDR_W, 1,0,1); // backlight on, underline on, blink block on 
		
		lcd1602SetCursor(LCD_ADDR_W, 0, 0);
		
		while(PB1);
		while(!PB1);
		
		lcd1602WriteString(LCD_ADDR_W, "HELLO LETI!!");
			
		lcd1602Control(LCD_ADDR_W, 1,0,1); // backlight on, underline off, blink block on 
		
		lcd1602SetCursor(LCD_ADDR_W, 0, 0);
		
		while(PB1);
		while(!PB1);
		
		lcd1602WriteString(LCD_ADDR_W, "STAY COOL!!!");
		
		while(PB1);
		while(!PB1);
		
		lcd1602Shutdown(LCD_ADDR_W);
		
		lcd1602Init(LCD_ADDR_W);

		lcd1602Control(LCD_ADDR_W, 1,0,1); // backlight on, underline on, blink block on 
		
		lcd1602SetCursor(LCD_ADDR_W, 0, 0);
		
		while(PB1);
		while(!PB1);
		
		lcd1602WriteString(LCD_ADDR_W, "HELLO AGAIN!!");
		
		lcd1602Control(LCD_ADDR_W, 1,1,1); // backlight on, underline on, blink block on 
		
		lcd1602SetCursor(LCD_ADDR_W, 0, 1);

		while(PB1);
		while(!PB1);
		
	}
}