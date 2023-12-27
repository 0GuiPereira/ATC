#include <REG51F380.H>
#include "init_device.h"
#include <stdio.h>
#include "types.h"
#include "lcd1602.h"
#include "config_platform.h"
#include "serialIO.h"
#include "delay.h"




sbit PB1 = P0^6;

// LCD ADDRESS 0x27
#define LCD_ADDR_W 0x4E
#define LCD_ADDR_R 0x4F


/*********************************************************
 * main loop			  																		 *
 *********************************************************/
void main(void){
	
	int rc;
	char c = 0;

	
	init_device();
	timer2_init_auto(-40000);
	uart0_initialize();
	
	
	TF2H = 0;
	ET2 = 1;
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