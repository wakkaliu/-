#include "infrared_digital.h"

INF* infrared_initial(void){
    INF *infLED;
    infLED=malloc(sizeof(INF));
    infLED->ch3=0;
    infLED->ch4=0;
    infLED->ch5=0;
    infLED->ch6=0;
    infLED->ch7=0;
    infLED->mod=0;
    
    return infLED;
}

void infrared_state(INF* input){

	
	input->ch3=MCP3008_analog_read( SPI_CS ,channel_3);
	usleep(10000);
	
	input->ch4=MCP3008_analog_read( SPI_CS ,channel_4);
	usleep(10000);
	
	input->ch5=MCP3008_analog_read( SPI_CS ,channel_5);
	usleep(10000);
	
	input->ch6=MCP3008_analog_read( SPI_CS ,channel_6);
	usleep(10000);
	
	input->ch7=MCP3008_analog_read( SPI_CS ,channel_7);
	usleep(10000);
	
	
}

char trigger(char* state,int high_level,int low_level,int input){
	if((*(state)!=0)&&(input<low_level)){
		*state=0;
	}
	else if((*(state)==0)&&(input>high_level)){
		*state=1;
	}
	return *state;
}

char a2d(char *state,INF *input){
	trigger(state+0,ch3_h,ch3_l,input->ch3);
	trigger(state+1,ch4_h,ch4_l,input->ch4);
	trigger(state+2,ch5_h,ch5_l,input->ch5);
	trigger(state+3,ch6_h,ch6_l,input->ch6);
	trigger(state+4,ch7_h,ch7_l,input->ch7);
	char i,sum=0;
	for(i=0;i<=sizeof(state)/sizeof(char);i++){
		sum+=(*(state+i)<<i);
	}
	return sum;
}

//-------------------------------------------------------

double straight(char num){
	double rate;
	switch(num){
		case 1:
			rate=-0.4;
			break;
		case 3:
			rate=-0.3;
			break;
		case 2:
			rate=-0.2;
			break;
		case 6:
			rate=-0.1;
			break;
		case 4:
			rate=0;
			break;
		case 12:
			rate=0.1;
			break;
		case 8:
			rate=0.2;
			break;
		case 24:
			rate=0.3;
			break;
		case 16:
			rate=0.4;
			break;
		
		case 7:
			rate=-0.4;
			break;
		case 28:
			rate=0.4;
			break;
		default:
			rate=0;
			break;
	}
	return rate;
}
double leaveStation(char num){
	double rate;
	switch(num){
		case 1:
			rate=-0.4;
			break;
		case 3:
			rate=-0.3;
			break;
		case 2:
			rate=-0.2;
			break;
		case 6:
			rate=-0.1;
			break;
		case 4:
			rate=0;
			break;
		case 12:
			rate=0.1;
			break;
		case 8:
			rate=0.2;
			break;
		case 24:
			rate=0.3;
			break;
		case 16:
			rate=0.4;
			break;
		
		case 7:
			rate=-0.4;
			break;
		case 28:
			rate=0.4;
			break;
		
		case 17:
			rate=0.4;
			break;
		case 19:
			rate=0.35;
			break;
		case 20:
			rate=0.3;
			break;
		case 18:
			rate=0.3;
			break;
		case 22:
			rate=0.3;
			break;
		case 25:
			rate=0.3;
			break;
		case 9:
			rate=0.2;
			break;
		case 10:
			rate=0.2;
			break;
		case 11:
			rate=0.2;
			break;

		case 27:
			rate=0.2;
			break;
		case 26:
			rate=0.15;
			break;
		
		case 5:
			rate=0.1;
			break;
		case 13:
			rate=0.1;
			break;
			
		
		
		default:
			rate=0;
			break;
	}
	return rate;
}

double tracking_control(char stateNum,char* mode){
	double rateValue=0;
	
	switch(*mode){
		case 1:
			rateValue=straight(stateNum);
			break;
		case 2:
			rateValue=leaveStation(stateNum);
			break;
		default:
			break;
	}
	
	return rateValue;
}
