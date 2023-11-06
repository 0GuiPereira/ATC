#include <REG51F380.H>
#define ENABLE_UART0 (ES0 = 1)
#define DISABLE_UART0 (ES0 = 0)
#define BUFFERSIZE 0xff

// Dados existentes em cada um dos buffers
extern volatile unsigned char TxNumberOfData;
extern volatile unsigned char RxNumberOfData;

// Apontadores para o buffers circular
unsigned char TxBufWritePtr=0, RxBufReadPtr=0;

// Buffers de tranmissao e rececao
volatile unsigned char TxBuffer[BUFFERSIZE];
volatile unsigned char RxBuffer[BUFFERSIZE];

// Apontadores de leitura no buffer TxBuffer
volatile unsigned char TxBufReadPtr=0;

// Apontadores de escrita no buffer RxBuffer
volatile unsigned char RxBufWritePtr=0;

// Dados existentes em cada um dos buffers
volatile unsigned char TxNumberOfData=0;
volatile unsigned char RxNumberOfData=0;

void configUART0(void) {
    P0MDOUT |= 0x10; //Ativa Push-pull
    //no pino TX (recomendado)

    //Configurar timer1
    //Segundo a tabela 23.1 do manual do micro
    //(SYSCLK=24MHz)
    CKCON |= 0x08; //T1M=0 >> 155200bps.
    //Comentar p/ 9600bps

    TH1 = 0x98; //Valor de autoreload
    // p/ 115200 e 9600

    //Selecao e ativacao do timer
    TMOD |= 0x20; //Timer1, modo2 (autorelaod)
    TR1 = 1; 
	  //TCON |=0x40 >> Ativa timer

    //Configurar porta serie (SCON)
    //8-bit UART por defeito
    REN0 = 1; 
	  //SCON0 |=0x10 >> Ativa rececao uart0

    //Interrupcoes
    ES0 = 1; //Ativa a interr da porta serie (Reg IE.4)
    EA = 1; //Ativa globalmente as interr. (Reg IE.7)
}

unsigned char readUART(unsigned char *value) {
    DISABLE_UART0;
    switch (RxNumberOfData) {
        BUFFEREMPTY:
        ENABLE_UART0;
        return 0;
        default:
        RxNumberOfData--;
        *value=RxBuffer[RxBufReadPtr];
        RxBufReadPtr = (RxBufReadPtr+1) % BUFFERSIZE;
    }
    ENABLE_UART0;
    return 1;
}

unsigned char writeUART(unsigned char value) {
    DISABLE_UART0;
    switch (TxNumberOfData) {
        BUFFERFULL:
        ENABLE_UART0;
        return 0;
        BUFFEREMPTY :
        sbuf = value;
        break;
        default:
        //TxNumberOfData++;
        TxBuffer[TxBufWritePtr]=value;
        TxBufWritePtr = (TxBufWritePtr+1) % BUFFERSIZE;
    }
    TxNumberOfData++;
    ENABLE_UART0;
    return 1;
}

void UARTInterrupt(void) __interrupt (bit 4) {
    if (RI0) {
        RI0 = 0;
        if (RxNumberOfData < BUFFERSIZE) {
            RxBuffer[RxBufWritePtr] = SBUF;
            RxWritePtr = (RxBufWritePtr +1) % BUFFERSIZE;
            RxNumberOfData++;
        }
    }
    if (TI0 == 1) {
        TI0=0;
        TxNumberOfData--;
        if (TxNumberOfData) {
            SBUF = TxBuffer[TxBufReadPtr];
            TxReadPtr = (TxBufReadPtr +1) % BUFFERSIZE;
            //TxNumberOfData--;
        }
    }
}

void main (void) {
    char hello[] = "Hello World";
    char *chr = hello;
    char chr_r;
    XBR0 |= 0x01; //Ativa UART0 no crossbar
    XBR1 |= 0x40; //Ativa crossbar
    CLKSEL |= 0x02; //SYSCLK derivado do
    // Oscil/2=24MHz

    configUART0();

    while (*chr) {
        writeUART(*chr);
        chr++;
    }

    while (1) {
        while (readUART(chr_r))
        writeUART(chr_r);
        else {
            chr = hello;
            while (*chr) {
                writeUART(*chr);
                chr++;
            }
        }
    }
    
}
