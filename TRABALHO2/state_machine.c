#include <REG51F380.H>
#include <REG51F380.H>
#include <stdio.h>
#include "config_platform.h"
#include "serialIO.h"
#include "timer2_delay.h"

sbit pb1 = P0^6;
sbit pb2 = P0^7;

char code *ptr;

char code sentidoNorteSul[] = "Hello uart0 m0\r\n";
char code sentidoEsteOeste[] = "testeEO\r\n";


unsigned char digits_array[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};


/*********************************************************/
typedef enum ENUM_STATES {S1 = 0, S2} e_states;

e_states state, nextstate;

/* function prototypes */
void state_1 (void);
void state_2 (void);

void temporizador(){
	char i;
	for (i = 9; i >= 0; --i) {
			P2 = digits_array[i];
			delay_s(1);
			nextstate = S2;
	}
	for(ptr = (char code*)sentidoNorteSul; *ptr!= '\0'; ptr++){
		while(uart0_putchar(*ptr)== -ENOBUFS);
	}
}

void NorteSul(){

	
//print_u0(sentidoNorteSul);
	
	temporizador();
}

void EsteOeste(){
	temporizador();
	nextstate = S1;
}
/*********************************************************/	

void (*state_process [])(void) = {state_1, state_2};


void state_1(void){
	NorteSul();
}
void state_2(void){
	
}

/*********************************************************/
void encode_FSM(){
		state_process[state] ();
}

/*********************************************************
 *    main function																	 		 *
 *********************************************************/
void main (void){
	
	
	Init_Device();
	uart0_initialize();
	timer2_init_auto(-40000);

	//Set by hardware when the Timer 2 high byte overflows from 0xFF to 0x00. In 16 bit 
	//mode, this will occur when Timer 2 overflows from 0xFFFF to 0x0000. 	
	TF2H = 0;	
	// Enable Flag Timer 2 Overflow
	// Timer 2 Run Control. Timer 2 is enabled by setting this bit to 1. 
	TR2 = 1;
	
	EA=1;
	
	state = nextstate = S1;
	
	while (1) {
		
		encode_FSM();
		state = nextstate;
		
	}
}
