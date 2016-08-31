#include "infrared_analog.h"

INF* infrared_initial(void){
	INF *infLED;
	bcm2835_init();
    
    infLED->frontLED=0;
    infLED->leftLED=0;
    infLED->rightLED=0;
    infLED->tempRight=0;
    infLED->tempFront=0;
    infLED->tempLeft=0;
    infLED->mod=0;
    
    return infLED;
}

void infrared_state(INF* input){
	bcm2835_spi_begin();
	
	input->rightLED=MCP3008_analog_read( SPI_CS ,channel_2);
	usleep(10000);
	
	input->frontLED=MCP3008_analog_read( SPI_CS ,channel_3);
	usleep(10000);
	
	input->leftLED=MCP3008_analog_read( SPI_CS ,channel_4);
	usleep(10000);
	bcm2835_spi_end();
	
}

char barcode(INF *input){
	if(input->frontLED<255){
		return 0;
	}
	else{
		return 1;
	}
}


double tracking_control(INF *input,char* mode){
	double rateValue;
	int sign,temp;
	infrared_state(input);
	
	if((*mode==leaveMode) || (*mode==enterMode)){
		printf("change to intersection mode.\n");
		input->mod=*mode;
		input->tempLeft=input->leftLED;
		input->tempRight=input->rightLED;
		input->tempFront=input->frontLED;
		
		if (*mode == leaveMode){			//don't go to station.
			printf("straight\n");
		}
		else if(*mode == enterMode){	//go to station.
			printf("go to station.\n");
		}
		*mode= 0;	//end_change
	}
	
	switch(input->mod){
		case normalMode :
			rateValue=((double)(input->rightLED-input->leftLED))/ADC_divider;
			break;
		case leaveMode:
			if(input->rightLED>input->frontLED){
				rateValue=((double)(input->frontLED-input->leftLED))/ADC_divider;
			}
			else{
				rateValue=((double)(input->rightLED-input->leftLED))/ADC_divider;
			}
			
			break;
		case enterMode :
			if(input->leftLED>input->frontLED){
				rateValue=((double)(input->rightLED-input->frontLED))/ADC_divider;
			}
			else{
				rateValue=((double)(input->rightLED-input->leftLED))/ADC_divider;
			}
			break;
		default :
			rateValue=0;
			break;
	}
	
	return rateValue;
}
