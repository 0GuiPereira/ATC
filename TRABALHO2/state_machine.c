#include <REG51F380.H>
#include <REG51F380.H>
#include <stdio.h>
#include "config_platform.h"
#include "serialIO.h"
#include "timer2_delay.h"

sbit pb1 = P0^6;
sbit pb2 = P0^7;

char code *ptr;

<<<<<<< HEAD
char code estradaVerde[] = "Semaforo Estrada Verde\r\n";
char code estradaAmarelo[] = "Semaforo Estrada Amarelo\r\n";
char code estradaVermelho[] = "Semaforo Estrada Vermelho\r\n";

char code passadeiraVerde[] = "Semaforo Passadeira Verde\r\n";
char code passadeiraAmarelo[] = "Semaforo Passadeira Amarelo\r\n";
char code passadeiraVermelho[] = "Semaforo Passadeira Vermelho\r\n";


char code intermitente[] = "Semaforo Estrada Intermitente\r\n";
char code inicio[] = "Semaforo estrada sempre verde, passdeira só fica verde quando o PB1 é pressionado\r\n";

char numeros[][3] = {"0","1", "2", "3", "4", "5", "6", "7", "8", "9"};
=======
char code sentidoNorteSul[] = "Hello uart0 m0\r\n";
char code sentidoEsteOeste[] = "testeEO\r\n";
>>>>>>> parent of a1d659e (falta fazer, tudo a funcionar)


unsigned char digits_array[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};


/*********************************************************/
typedef enum ENUM_STATES {S1 = 0, S2} e_states;

e_states state, nextstate;

/* function prototypes */
void state_1 (void);
void state_2 (void);

<<<<<<< HEAD
//DEBOUNCE
unsigned int debounce(bit PB){
				char window;  
        unsigned int j;
    for(j = 0; j < 8; j++) {
        window = (window << 1) | PB;
    }
    return window;
}

void transmitString(const char *str) {
    const char *ptr = str;
    
    while (*ptr != '\0') {
        while (uart0_putchar(*ptr) == -ENOBUFS);
        ++ptr;
    }
}
void esperarPassadeira(){
	char i;
	for (i = 10; i >= 0; --i) {
			transmitString(numeros[i]);
			if (i < 10) {
					transmitString("...\n");
				if (i == 5){
					transmitString(estradaAmarelo);
				}
			}
			transmitString(estradaVermelho);
			transmitString(passadeiraVerde);
	}
}
=======
>>>>>>> parent of a1d659e (falta fazer, tudo a funcionar)
void temporizador(){
	char i;
<<<<<<< HEAD
	for (i = 10; i >= 0; --i) {
			P2 = digits_array[i];
			transmitString(numeros[i]);
			if (i < 10) {
					transmitString("...\n");
			}
	}
}

void temporizadorPassadeira(){
	char i;
	for (i = 10; i >= 0; --i) {
			P2 = digits_array[i];
			transmitString(numeros[i]);
			if (i < 10) {
					transmitString("...\n");
			if (i == 5){
				transmitString(passadeiraAmarelo);
			}
			}
			delay_s(1);
			transmitString(passadeiraVermelho);
	}
}

void butaopassadeira(){
	if(debounce(!PB1) && !button1pressed) {
		
		transmitString("Passadeira ficará verde em 10 segundos\r\n");
		temporizador();
		transmitString(passadeiraVerde);
		transmitString("Tens 10 segundos para passar...\r\n");
		temporizadorPassadeira();
		delay_s(1);
		transmitString(estradaVerde);
		
		button1pressed = 1;               
    }	
			if(!debounce(!PB1)){ // reset a flag se o botao ? clicado
				button1pressed = 0;
			}
=======
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

>>>>>>> parent of a1d659e (falta fazer, tudo a funcionar)
	
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
