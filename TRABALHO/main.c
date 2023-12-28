#include <REG51F380.H>
#include "init_device.h"
#include <stdio.h>
#include "types.h"
#include "lcd1602.h"
#include "config_platform.h"
#include "serialIO.h"
#include "delay.h"

#include <stdlib.h>

unsigned char temp[25] = 	{22, 21, 25, 22, 25, 23, 28, 20, 25, 27, 22, 22, 28, 22, 29, 28, 27, 27, 26, 23, 28, 22, 21, 27, 29};
unsigned char hum[25] = 	{63, 70, 60, 60, 69, 69, 69, 60, 67, 60, 63, 66, 67, 62, 69, 66, 64, 63, 70, 60, 69, 65, 67, 62, 70};
	
sbit PB1 = P0^6;

// LCD ADDRESS 0x27
#define LCD_ADDR_W 0x4E
#define LCD_ADDR_R 0x4F


/*********************************************************
 * main loop			  																		 *
 *********************************************************/
void main(void){
	
	char rc;
	char c = 0;
	

	init_device();
	timer2_init_auto(-40000);
	uart0_initialize();
	
	
	//Set by hardware when the Timer 2/3 high byte overflows from 0xFF to 0x00. In 16 bit 
	//mode, this will occur when Timer 2/3 overflows from 0xFFFF to 0x0000. 	
	TF2H = 0;	
	TMR3CN &= ~(1 << B_TF3H);
	
	// Enable Flag Timer 2/3 Overflow
	ET2 = 1;
	EIE1 |= (1 << B_ET3);
	
	// Timer 2/3 Run Control. Timer 2/3 is enabled by setting this bit to 1. 
	TMR3CN |= (1 << B_TR3);
	TR2 = 1;
	
	EA=1; //enable interrupts
	

	
	rc = lcd1602Init(LCD_ADDR_W);
	if (rc)
	{
		while(1);
	}
	
		while(1){
		lcd1602Clear(LCD_ADDR_W);	    // clear the memory

		lcd1602Control(LCD_ADDR_W, 1,0,1); // backlight on, underline on, blink block on 
		
		lcd1602SetCursor(LCD_ADDR_W, 0, 0);
				
		P2 = 0x86;
		delay_s(1);
		P2 = 0xC6;
			
		
		
		lcd1602WriteString(LCD_ADDR_W, "OLAAA");
			
		lcd1602Control(LCD_ADDR_W, 1,0,1); // backlight on, underline off, blink block on 
		
		lcd1602SetCursor(LCD_ADDR_W, 5, 1);
		
		delay_s(1);
		
		lcd1602WriteString(LCD_ADDR_W, "STAY COOL!!!");
		
		delay_s(1);
		
		lcd1602Shutdown(LCD_ADDR_W);
		
		lcd1602Init(LCD_ADDR_W);

		lcd1602Control(LCD_ADDR_W, 1,0,1); // backlight on, underline on, blink block on 
		
		lcd1602SetCursor(LCD_ADDR_W, 0, 0);
		
		delay_s(1);
		
		lcd1602WriteString(LCD_ADDR_W, "HELLO AGAIN!!");
		
		lcd1602Control(LCD_ADDR_W, 1,1,1); // backlight on, underline on, blink block on 
		
		lcd1602SetCursor(LCD_ADDR_W, 0, 1);

		delay_s(1);
		
	}
}