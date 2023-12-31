#include <REG51F380.H>
#include "config_platform.h"
#include "serialIO.h"
#include "delay.h"
#include "display.h"


/*********************************************************
 * main loop			  																		 *
 *********************************************************/
void main(void){
		
	Init_Device();
	uart0_initialize();
	timer2_init_auto(-40000);
		
	timerAndInterrups();

	while(1){
		display();
	}
}