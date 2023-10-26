#include <SI_EFM8BB3_Register_Enums.h>
#include <stdio.h>
#include "pca0.h"

#define ECF_BIT 0 //Timer 0 overflow
#define CCF1_BIT 1 //Flag de comparação do Modulo 1
#define CF_BIT 7    //Flag de interrupção
#define CR_BIT 6    //Permite parar/continuar o contador
#define EIE1_EPCA0_ENABLED 0x10 //Ativar interrupções do PCA
#define XBR1_PCA0ME_CEX0 0x01 //CEX0 como modulo I/O do PCA
#define ECOMP_BIT 6  //Ativar comparador
#define PWM_BIT 1 //Ativar 8-bit pwm
#define MAT_BIT 3 //Ativar match
#define ECCF_BIT 0 //Ativar flag de comparação

sbit PCA0CN0_CRUN = PCA0CN0^6; //Controlo do PCA - Começar o Timer 
sbit PCA0CN0_CFLAG = PCA0CN0 ^7; //Controlo do PCA - Flag de interrupção

signed int D = 50;
code unsigned char coef[] = {127,137,148,158,168,177,186,194,202,209,215,220,225,228,231,232,233,232,231,228,225,220,215,209,202,194,186,177,168,158,148
,137,127,117,106,96,86,77,68,60,52,45,39,34,29,26,23,22,21,22,23,26,29,34,39,45,52,60,68,77,86,96,106,117};
unsigned char index;

	
void init_pca0_pwm(void){
	XBR1 |= XBR1_PCA0ME_CEX0;
	P0SKIP |= 2;
	P0MDOUT |= 2; //Modo do Output do Port 0 - Push-Pull
	PCA0CN0_CR = 0; //Parar o contador
	PCA0CN0_CF = 0; //Parar a interupção
	PCA0MD |= 2<<1; //Modo do PCA 
	PCA0CPL0 = coef[0];
	PCA0CPH0 = coef[1];
	index = 2;
	PCA0CPM0 = (1<<ECOMP_BIT) | (1<<PWM_BIT); //Modo do modulo 0 do PCA

	TCON_TR0 = 0;
	CKCON0 |= 0x04; //Controlo do Clock - Timer 0 SYSCLK
	TMOD |= 0x02; //Modo do Timer - 8-bit autoreload
	TL0 = -60;
	TH0 = -60;
	TCON_TR0 = 1; //Começar o Timer
	TCON_TF0=0;
	
	PCA0CPL1 = 0x00;
	PCA0CPH1 = 0x01;
	PCA0CPM1 = (1<<ECOMP_BIT) | (1<<MAT_BIT) | (1<<ECCF_BIT);
	EIE1 |= EIE1_EPCA0_ENABLED; //Ativar interrupção do PCA
}

void inc_duty_cycle(void){
	D += 10;	
	if(D >= 100)
		D = 100;
	PCA0CPH0 = 256 - ((256 * D)/100);
}

void dec_duty_cycle(void){
	D -= 10;
	if(D <= 0)
		D = 0;
	PCA0CPH0 = 256 - ((256 * D)/100);
}

void pca0_start(void){
	PCA0CPH0 = 0x80;
	PCA0CN0_CR = 1;
	TCON_TR0 = 1;
}

void pca0_stop(void){
	PCA0CPH0 = 0;
	PCA0CN0_CR = 0;
	PCA0L = 0;
	PCA0H = 0;
	TCON_TR0 = 0;
}

void isr_pca0(void)interrupt 11 using 2{
	PCA0CPH0 = coef[index]; //Modo de Captura do modulo 0 PCA 
	index++;
	if(index == sizeof(coef))
		index = 0;
	PCA0CPH1++; //Modo de Captura do modulo 1 PCA 
	PCA0CN0 &= ~(1<<CCF1_BIT); // Controlo do PCA
}