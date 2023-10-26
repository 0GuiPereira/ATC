#include <SI_EFM8BB3_Register_Enums.h>
#include <stdio.h>
#include "init.h"
#include "pca0.h"
#include "serialIO.h"
#include "fsm.h"

void main(void){
	
	init_device();
	init_pca0_pwm();
	init_fsm();
	IE_EA = 1;
	
	
	while(1){
		encode_fsm();
		fsm_switch();
	}	
}