#include <REG51F380.H>
#include <stdio.h>
#include "serialIO.h"


//atribui��o dos botoes a variaveis
// ativos a 0
sbit PB1 = P0^6;
sbit PB2 = P0^7;


unsigned int i;
unsigned char button1pressed; //flags se os botoes sao clicados
unsigned char button2pressed;

// vetor dos dos numeros 16 eletemnos
unsigned char vetor[16] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E };

void init_device(void)
{
        //WD
    PCA0MD         = 0x00;
    // enable cross-bar for I/Os
    XBR1        = 0x40;
    FLSCL     = 0x90;
    CLKSEL    = 0x03;
}

//DEBOUNCE
unsigned int debounce(bit PB){
				char window;  
        unsigned int j;
    for(j = 0; j < 8; j++) {
        window = (window << 1) | PB;
    }
    return window;
}


void main()
{
		char c;
		char code *ptr;
		Init_Device();
		uart0_initialize();
	
    init_device();
    
    i = 0;
    button1pressed = 0; //flag se o botao � clicado
    button2pressed = 0; //flag se o botao � clicado
	
		EA=1;

    
    while (1) {

    // incrementar
    if(debounce(!PB1) && !button1pressed) {
            
             if (i < 15) {
                i++;
            } else {
                i = 15;
            }
						button1pressed = 1;               
    }   
			if(!debounce(!PB1)){ // reset a flag se o botao � clicado
				button1pressed = 0;
			}
    // decrementar 
    if(debounce(!PB2) && !button2pressed) {
			
              if (i > 0) {
                i--;
            } else {
                i = 0;
            }
						button2pressed = 1;       
    }
			if(!debounce(!PB2)){
				button2pressed = 0;
			}
   
    
        P2 = vetor[i];
	}
        
    
}