#include <REG51F380.H>
#include "init_device.h"
#include <string.h>
#include <stdio.h>

#define B_TF3H 7
#define B_TR3 2
#define B_ET3 7

sbit pb1 = P0^6;
sbit pb2 = P0^7;

//chave por defeito	            	8			0			5			1
unsigned char default_key[4] = {0x80, 0xC0, 0x92, 0xF9};
 
//																	L			0			1			2			3			4			5			6			7			8			9
unsigned char digits_array[11] = {0xC7, 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};


unsigned char button1pressed; //flags se os botoes sao clicados
unsigned char button2pressed;
unsigned char wrongkeycount;
char c = 0;
 

/*********************************************************/
typedef enum ENUM_STATES {S1 = 0, S2, S3, S4, S5} e_states;

e_states state, nextstate;

/* function prototypes */
void state_1(void);
void state_2(void);
void state_3(void);
void state_4(void);
void state_5(void);


/*********************************************************
 * uart0 init				  																	 *
 *********************************************************/
void uart0_init(void){	
    /* UART0 Control                            */
		/* set REN0, enable reception								*/
    SCON0     = 0x10;
	
		/* Timer/Counter Mode                       */	
		TMOD      = 0x20;
	
    /* Clock Control                            */
    CKCON     = 0x08;
	
		/* Timer/Counter 1 High                     
		 * 0x30 generates a baudrate of 115200 bps	*/
    TH1       = 0x30;
	
		/* Timer 1 On/Off Control 		   	          */
		TR1 			= 1;
	
		/* UART0 TX Interrupt Flag                  */
		TI0 			= 1;
		RI0 = 1;
}


/*********************************************************
 * getc using uart0   																	 *
 *********************************************************/
char uart0_getc(char* c){

	if(RI0 == 1){
		RI0 = 0;
		*c = SBUF0; 
		return 1;
	}
	return 0;
}


void timer3_init_auto(char reload){

	TMR3CN = 0;

	#define B_T3MH 7	

	CKCON &= ~((1 << B_T3MH));
	
	TMR3H = (reload) >> 8;
	TMR3L = (reload);

	TMR3RLH = (reload) >> 8;
	TMR3RLL = (reload);
	
}


void timer2_init_auto(char reload){

	TMR2CN = 0;

	#define B_T2MH 5	

	CKCON &= ~((1 << B_T2MH));
	
	TMR2H = (reload) >> 8;
	TMR2L = (reload);

	TMR2RLH = (reload) >> 8;
	TMR2RLL = (reload);
	
}

void delay_250us(){
	while(!(TMR3CN & (1 << B_TF3H)));
	TMR3CN &= ~(1 << B_TF3H);  
}

void delay_s(unsigned char s) {
    unsigned char i = 0;

    while(i != (s*100)) {
        i++;
        while(!TF2H);
        TF2H = 0;
    }
}


void square_wave(){
	while (1){ // mudar para aceitar a HWFLAG
		//HIGH
		P1 |= 0x80;
		delay_250us();
		//LOW
		P1 &= ~(0x80);
		delay_250us();
	}
}

// validar a key introduzida pelo utilizador
bit validate_key(unsigned char v1[4], unsigned char v2[4]){
	return memcmp(v1, v2, 4) == 0;
}

//DEBOUNCE
unsigned char debounce(bit PB){
        char window = 0;  
        unsigned char j;
    for(j = 0; j < 8; j++) {
        window = (window << 1) | PB;
    }
    return window;
}

// ler a key a ser posta pelo utilizador
void read_key(){
		unsigned char digit_index;			//index dos digitos
		unsigned char index_input_key;	//index da key a ser lida
		unsigned char input_key[4];
		

	if(uart0_getc(&c)== 0x45){ // palavra enter 
			printf("enter");
			input_key[index_input_key] = digits_array[digit_index];
			index_input_key++;
			digit_index = 0;
		}
		if(index_input_key == 4){
			if(validate_key(input_key,default_key)){
				index_input_key = 0;
				wrongkeycount = 0;
				nextstate = S2;
			}
			else{
				index_input_key = 0;
				nextstate = S3;
			}
		}

		
    if(uart0_getc(&c) == 0x2b) { // caracter +
			printf("incremento");
			
      if (digit_index < 11) {
				digit_index++;
			}
			if (digit_index > 10){
        digit_index = 0;
			}
    }

		P2 = digits_array[digit_index];
}

void errorhandling(){
		wrongkeycount++;
		if(wrongkeycount == 1){
			P2 = 0x86; // E
			delay_s(1); //diz que foi erro pelo E no display passado 1 segundo muda para C de close e espera mais 4 segundos num total de 5 segundos
			P2 = 0xC6; // C
			delay_s(4);//wait 4 secs
			nextstate = S1;
		}
		if(wrongkeycount == 2){
			P2 = 0x86; // E
			delay_s(1); //diz que foi erro pelo E no display passado 1 segundo muda para C de close e espera mais 9 segundos
			P2 = 0xC6; // C
			delay_s(9);//wait 9 secs
			nextstate = S1;
		}
		if(wrongkeycount == 3){
			P2 = 0x86; // E
			delay_s(1); //diz que foi erro pelo E no display passado 1 segundo muda para C de close e espera mais 14 segundos
			P2 = 0xC6; // C
			delay_s(14);//wait 14 secs
			nextstate = S1;
		}
		if(wrongkeycount == 4){
			P2 = 0x86; // E
			delay_s(1); //diz que foi erro pelo E no display passado 1 segundo muda para C de close e espera mais 19 segundos
			P2 = 0xC6; // C
			delay_s(19);//wait 19 secs
			nextstate = S1;
		}
		if(wrongkeycount == 5){
			nextstate = S4;
		}
}

void open_menu(){//							  	O		  	P		  C
		unsigned char open_menu[3] = {0xA3 , 0x8C, 0xC6};
		unsigned char index;
		
		if(!debounce(pb2) && !button2pressed){
			button2pressed = 1;
			if(open_menu[index] == 0x8C){
				nextstate = S5;
				index = 0;
			}
			if(open_menu[index] == 0xC6){
				nextstate = S1;
				index = 0;
			}
		}
		
		if(debounce(pb2)){
			button2pressed = 0;
		}
		
		if(!debounce(pb1) && !button1pressed){
			
			
			button1pressed = 1;            
      if (index < 3){
				index++;
			}
			if (index > 2){
        index = 0;
			}
    }
		
		if(debounce(pb1)){
			button1pressed = 0;
		}

		P2 = open_menu[index];
	
}
void set_key(){//									P			0			1			2			3			4			5			6			7			8			9
		unsigned char set_key[11] = {0x8C, 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
		unsigned char index;
		unsigned char index_new_key;
		
		if(!debounce(pb2) && !button2pressed){
			button2pressed = 1;
			default_key[index_new_key] = set_key[index];
			index_new_key++;
			index = 0;
		}
		
		if(index_new_key == 4){
			index_new_key = 0;
			nextstate = S1;
		}
		
		if(debounce(pb2)){
			button2pressed = 0;
		}
		
		if(!debounce(pb1) && !button1pressed){
			
			button1pressed = 1;            
      if (index < 11){
				index++;
			}
			if (index > 10){
        index = 0;
			}
    }
		
		if(debounce(pb1)){
			button1pressed = 0;
		}

		P2 = set_key[index];
}


/*********************************************************/	
//													close/locked  	open		error			alarm		set_key
void (*state_process [])(void) = {state_1, state_2, state_3, state_4, state_5};


void state_1(void){
		read_key();
		P1 = 0x10;
}
void state_2(void){
		open_menu();
		P1 &= ~(0x10);
}
void state_3(void){
		errorhandling();
}
void state_4(void){
		P2 = 0x88;
	  square_wave();
}
void state_5(void){
		set_key();
}

/*********************************************************/
void encode_FSM(){
		state_process[state] ();
}

/*********************************************************/


/*********************************************************
 *    main function																	 		 *
 *********************************************************/
void main (void){
	
	Init_Device();
	uart0_init();	
	
	timer2_init_auto(-40000);
	timer3_init_auto(-1000);
	
	state = nextstate = S1;
	
	printf("uart a funcionar");
	
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
			
	while (1) {
		encode_FSM();
		state = nextstate;
	}
}
