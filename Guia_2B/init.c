#include <SI_EFM8BB3_Register_Enums.h>
#include <stdio.h>
#include "init.h"

void init_cpu_clock49(void){
	WDTCN = 0xDE;     //WATCH DOG
	WDTCN = 0xAD;     //WATCH DOG
	PCA0MD = 0; //SO NO SIMULADOR
	SFRPAGE = 0x10; //SFRPAGE to 1
	PFE0CN |= (1<<4);	//FLRT bit a 1
	SFRPAGE = 0;     //SFRPAGE a 0
	CLKSEL = 0;
	CLKSEL = 0;
	CLKSEL |= 0x03;
	CLKSEL |= 0x03;
}
void init_timer1(void){
	CKCON0 = 0x08;
	TCON_TR1 = 0;
	TMOD = 0x20; 
	TH1 = 43;      //BAUD RATE 115200
	TL1 = 43;      //BAUD RATE 115200
	TCON_TR1 = 1;
	TCON_TF1 = 0;
}

void init_xbar(void){
	XBR2 |= 0x40;
	XBR0 |= 0x01;
}

void init_uart0(void){
	SCON0_REN = 1;
	SCON0_TI = 1;
	IE_ES0 = 1;
}

void init_device(void){
	init_cpu_clock49();
	//PCA0MD = 0; //SO NO SIMULADOR
	init_timer1();
	init_xbar();
	init_uart0();
}
	