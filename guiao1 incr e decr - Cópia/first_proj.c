#include <REG51F380.h>

sbit pb1 = P0^6;
sbit pb2 = P0^7;
sbit seg_dp = P2^7;

code const char seg_data[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10, 0x08, 0x03, 0x27, 0x21, 0x06, 0x0E};

char index = 0;
bit dp_state = 0;  // Estado do ponto decimal (ligado/desligado)
char prev_index = -1;  // Valor anterior, inicializado para um valor impossível

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 350; j++) {
            // Delay básico
        }
    }
}

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

void sendIndex() {
    char display_char;

    if (index != prev_index) {
        while (TI0 == 0);
        
        // Converte o valor hex para caractere ASCII
        if (index < 0x0A) {
            display_char = index + '0';
        } else {
            display_char = index - 0x0A + 'A';
        }

        SBUF0 = display_char;
        TI0 = 0;
        prev_index = index;
    }

    P2 = seg_data[index] | ((char)dp_state << 7);
}

void pressButton() {
    static bit pb1_prev = 1;
    static bit pb2_prev = 1;

    bit pb1_now = pb1;
    bit pb2_now = pb2;

    if (pb1_now == 0 && pb1_prev == 1) {
        if (index < 0x0F) {
            index++;
        }
        sendIndex();
    }

    if (pb2_now == 0 && pb2_prev == 1) {
        if (index > 0x00) {
            index--;
        }
        sendIndex();
    }

    pb1_prev = pb1_now;
    pb2_prev = pb2_now;
}

char receiveIndex() {
    if (RI0 == 1) {
        RI0 = 0;

        if (SBUF0 == 'i' || SBUF0 == 'I' || SBUF0 == '+') {
            if (index < 0x0F) {
                index++;
            }
        } else {
            if (SBUF0 == 'd' || SBUF0 == 'D' || SBUF0 == '-') {
                if (index > 0x00) {
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

void timer0_ISR(void) interrupt 1 {
    static unsigned int count = 0;

    TH0 = 0xFC;  // Recarrega o Timer0 para um atraso de 1 ms
    TL0 = 0x66;

    count++;
    if (count == 1000) {  // Aproximadamente 1 segundo
        dp_state = ~dp_state;  // Inverte o estado do ponto decimal
        count = 0;
    }

    seg_dp = dp_state;  // Atualiza o LED do ponto decimal
}

void main() {
    init_device();
    uart0_init();

    // Configuração do Timer0 para gerar interrupções a cada 1 ms
    TMOD |= 0x01;  // Timer0, modo 1 (16 bits)
    TH0 = 0xFC;    // Valor inicial para um atraso de 1 ms
    TL0 = 0x66;
    ET0 = 1;       // Habilita interrupção do Timer0
    TR0 = 1;       // Inicia o Timer0

    EA = 1;  // Habilita todas as interrupções globais

    while (1) {
        pressButton();
        if (receiveIndex()) {
            sendIndex();
				}
		}
}