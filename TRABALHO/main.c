#include <REG51F380.H>
#include <stdio.h>
#include "lcd1602.h"
#include "config_platform.h"
#include "serialIO.h"
#include "delay.h"
#include "randTH.h"


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
	

	Init_Device();
	uart0_initialize();
	//timer2_init_auto(-40000);
	
	
	
	//Set by hardware when the Timer 2/3 high byte overflows from 0xFF to 0x00. In 16 bit 
	//mode, this will occur when Timer 2/3 overflows from 0xFFFF to 0x0000. 	
	TF2H = 0;	
	TMR3CN &= ~(1 << B_TF3H);
	
	//ET2 = 1;
	//EIE1 |= (1 << B_ET3);
	
	// Timer 2/3 Run Control. Timer 2/3 is enabled by setting this bit to 1. 
	TMR3CN |= (1 << B_TR3);
	TR2 = 1;
	
	EA=1; //enable interrupts
	

	
	
		while(1){
			
		c = _getkey();
		putchar(c);
			
//		lcd1602Clear(LCD_ADDR_W);	    // clear the memory

//		lcd1602Control(LCD_ADDR_W, 1,0,1); // backlight on, underline on, blink block on 
//		
//		lcd1602SetCursor(LCD_ADDR_W, 0, 0);
//				
//		P2 = 0x86;
//		randTemp();
//		randTemp();
//		delay_s(1);
//		P2 = 0xC6;
//			
//		
//		
//		lcd1602WriteString(LCD_ADDR_W, "OLAAA");
//			
//		lcd1602Control(LCD_ADDR_W, 1,0,1); // backlight on, underline off, blink block on 
//		
//		lcd1602SetCursor(LCD_ADDR_W, 5, 1);
//		
//		delay_s(1);
//		
//		lcd1602WriteString(LCD_ADDR_W, "STAY COOL!!!");
//		
//		delay_s(1);
//		
//		lcd1602Shutdown(LCD_ADDR_W);
//		
//		lcd1602Init(LCD_ADDR_W);

//		lcd1602Control(LCD_ADDR_W, 1,0,1); // backlight on, underline on, blink block on 
//		
//		lcd1602SetCursor(LCD_ADDR_W, 0, 0);
//		
//		delay_s(1);
//		
//		lcd1602WriteString(LCD_ADDR_W, "HELLO AGAIN!!");
//		
//		lcd1602Control(LCD_ADDR_W, 1,1,1); // backlight on, underline on, blink block on 
//		
//		lcd1602SetCursor(LCD_ADDR_W, 0, 1);

//		delay_s(1);

			
		
	}
}