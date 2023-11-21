#include <REG51F380.H>


sbit PB1 = P0 ^ 6;
sbit PB2 = P0 ^ 7;
sbit seg_dp = P2^7;

bit dp_state = 0;
bit button1pressed; // flags se os botoes sao clicados
bit button2pressed;
unsigned char blink = 50;
unsigned char index;
unsigned char prev_index = -1;


// vetor dos dos numeros 16 eletemnos {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};
unsigned char vetor[16] = 						{0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10, 0x08, 0x03, 0x27, 0x21, 0x06, 0x0E};

void init_device(void) {
    FLSCL = 0x90;
    CLKSEL = 0x03;
    PCA0MD = 0x00;
    XBR1 = 0x40;
    XBR0 = 0x01;
}

void uart0_init(void) {
    SCON0 = 0x10;
    TMOD = 0x20;
    CKCON = 0x08;
    TH1 = 0x30;
    TR1 = 1;
    TI0 = 1;
    RI0 = 1;
}

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
	
	char P2_state;
	P2_state = P2;
	
	// sysclock /12 = 4MHz
	// 1s 4,000,000 ticks!
	// 10ms => 40000 ticks!
	// 50 ciclos = 500ms

	blink--;
	if(blink == 0){
		//P2 = P2_state ^ (1 << 7);
		dp_state = ~dp_state;
		blink = 50;
	}
	
	seg_dp = dp_state;	
	TF2H = 0;	
}

void sendIndex() {
    char charToBuffer;

    if (index != prev_index) {
        while (TI0 == 0);
        
        // Converte o valor hex para caractere ASCII
        if (index < 10) {
            charToBuffer = index + '0';
        } else {
            charToBuffer = index - 0x0A + 'A';
        }

        SBUF0 = charToBuffer;
        TI0 = 0;
        prev_index = index;
    }

    P2 = vetor[index] | ((char)dp_state << 7);
}

void buttonHandler(){
		// incrementar
		if (debounce(!PB1) && !button1pressed) {

			if (index < 15) {
				index++;
			}
			else {
				index = 15;
			}
			sendIndex();
			button1pressed = 1;
		}
		if (!debounce(!PB1)) { // reset a flag se o botao ? clicado
			button1pressed = 0;
		}

		// decrementar
		if (debounce(!PB2) && !button2pressed) {

			if (index > 0) {
				index--;
			}
			else {
				index = 0;
			}
			
			sendIndex();
			button2pressed = 1;
		}
		
		if (!debounce(!PB2)) {
			button2pressed = 0;
		}		
}

char receiveIndex() {
    if (RI0 == 1) {
        RI0 = 0;

        if (SBUF0 == 'i' || SBUF0 == 'I' || SBUF0 == '+') {
            if (index < 15) {
                index++;
            }
        } else {
            if (SBUF0 == 'd' || SBUF0 == 'D' || SBUF0 == '-') {
                if (index > 0) {
                    index--;
                }
            } else {
                index = SBUF0;
            }
        }

        return 1;
    }

    return 0;
}


void main(void) {
	init_device();
	uart0_init();
	timer2_init_auto(-40000);
	
	
	
	TF2H = 0;	
	ET2 = 1;
	TR2 = 1;
	EA = 1;

	
	while (1) {
			buttonHandler();
        if (receiveIndex()) {
            sendIndex();
		}
	}
}