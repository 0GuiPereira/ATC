#include <REG51F380.H>
#include <stdio.h>
#include "lcd1602.h"
#include "serialIO.h"
#include "delay.h"

#define LCD_ADDR_W 0x4E
#define LCD_ADDR_R 0x4F

unsigned char buffTemp[16];
unsigned char buffHum[16];


void display(){
	
		char i;
		for (i = 0; i < 32; i++) {
				char c = uart0_getchar();

        if (i < 16) {
            buffTemp[i] = c;
        }

        if (i >= 16 && i < 32) {
            buffHum[i - 16] = c;
        }
    }
		
			
		lcd1602Clear(LCD_ADDR_W);	    // clear the memory

		lcd1602Control(LCD_ADDR_W, 1,0,0); // backlight on, underline on, blink block on 
		
		lcd1602SetCursor(LCD_ADDR_W, 0, 0);
				

			

		lcd1602WriteString(LCD_ADDR_W, buffTemp);

		
			
		lcd1602Control(LCD_ADDR_W, 1,0,0); // backlight on, underline off, blink block on 
		
		lcd1602SetCursor(LCD_ADDR_W, 0, 1);
		
		
		
		lcd1602WriteString(LCD_ADDR_W, buffHum);
		
		delay_ms(500);

		
		lcd1602Shutdown(LCD_ADDR_W);
		
		lcd1602Init(LCD_ADDR_W);
}