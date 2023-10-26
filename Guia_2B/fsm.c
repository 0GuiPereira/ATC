#include <SI_EFM8BB3_Register_Enums.h>
#include <stdio.h>
#include "fsm.h"
#include "pca0.h"
#include "serialIO.h"
#include "init.h"

typedef enum my_states{S_Stopped = 0, S_Speed_Controller, S_Sinewave} state_t;
	state_t state, n_state;


static void stopped(void){
	pca0_stop();
	if(uart0_getchar() == 'S'){
		pca0_start();
		n_state = S_Sinewave;
	}

	if(uart0_getchar() == 'C'){
		pca0_start();
		n_state = S_Speed_Controller;
	}
}

static void sinewave(void){
	if(uart0_getchar() == 'P'){
		pca0_stop();
		n_state = S_Stopped;
	}
}
static void speed_controller(void){
	if(uart0_getchar() == 'I')
		inc_duty_cycle();
		
	if(uart0_getchar() == 'D')
		dec_duty_cycle();
		
	if(uart0_getchar() == 'P'){
		pca0_stop();
		n_state = S_Stopped;
	}
}

void encode_fsm(void){
	switch(state){
		case S_Stopped:
			stopped();
		break;
		case S_Speed_Controller:
			speed_controller();
		break;
		case S_Sinewave:
			sinewave();
		break;
		default:
			state = S_Stopped;
		break;
	}
}

void fsm_switch(void){
	state = n_state;
}

void init_fsm(void){
	state = S_Stopped;
	n_state = S_Stopped;
}