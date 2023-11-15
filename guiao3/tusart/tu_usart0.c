#include <REG51F380.H>
#include <stdio.h>
#include "config_platform.h"
#include "serialIO.h"

sbit PB1 = P0 ^ 6;
sbit PB2 = P0 ^ 7;
sbit seg_dp = P2^7;

unsigned char i;
unsigned char button1pressed; // flags se os botoes sao clicados
unsigned char button2pressed;
unsigned char blink = 50;


// vetor dos dos numeros 16 eletemnos
unsigned char vetor[16] = {0x40, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};

// DEBOUNCE
unsigned int debounce(bit PB) {
	char window;
	unsigned int j;
	for (j = 0; j < 8; j++)
	{
		window = (window << 1) | PB;
	}
	return window;
}

void timer2_init_auto(int reload){

	TMR2CN = 0;

	#define B_T2MH 5	

	CKCON &= ~((1 << B_T2MH));
	
	TMR2H = (reload) >> 8;
	TMR2L = (reload);

	TMR2RLH = (reload) >> 8;
	TMR2RLL = (reload);
	
}

void timer2_isr(void) interrupt 5 using 2 {
	
	// sysclock /12 = 4MHz
	// 1s 4,000,000 ticks!
	// 10ms => 40000 ticks!
	// 50 ciclos = 500ms

	
	if(--blink == 0){
		seg_dp ^= 1;
		blink = 50;
	}
	
		
	TF2H = 0;	
}


void main(void) {
	char c = 0;
	i = 0;
	button1pressed = 0; // flag se o botao ? clicado
	button2pressed = 0; // flag se o botao ? clicado
	
	Init_Device();
	uart0_initialize();
	timer2_init_auto(-40000);
	TF2H = 0;	
	ET2 = 1;
	TR2 = 1;
	EA = 1;

	
	while (1) {
		c = _getkey2();

		// incrementar
		if (debounce(!PB1) && !button1pressed) {

			if (i < 15) {
				i++;
			}
			else {
				i = 15;
			}
			putchar(i);
			putchar('\n');
			button1pressed = 1;
		}
		if (!debounce(!PB1)) { // reset a flag se o botao ? clicado
			button1pressed = 0;
		}

		if (c == 0x2b || c == 0x49 || c == 0x69) {
			if (i < 15) {
				i++;
			}
			else {
				i = 15;
			}
			putchar(i);
			putchar('\n');
		}

		// decrementar
		if (debounce(!PB2) && !button2pressed) {

			if (i > 0) {
				i--;
			}
			else {
				i = 0;
			}
			putchar(i);
			putchar('\n');
			button2pressed = 1;
		}
		if (!debounce(!PB2)) {
			button2pressed = 0;
		}

		if (c == 0x2d || c == 0x44 || c == 0x64) {
			if (i > 0) {
				i--;
			}
			else {
				i = 0;
			}
			putchar(i);
			putchar('\n');
		}
		
		P2 = vetor[i];
		
	}
}



