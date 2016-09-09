#include "infrared_digital.h"
#define R1 -0.5
#define R2 -0.7
#define R3 -0.9
#define R4 -0.95
#define R5 -1
#define L1 0.5
#define L2 0.7
#define L3 0.9
#define L4 0.95
#define L5 1
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
	usleep(measTime);
	
	input->ch4=MCP3008_analog_read( SPI_CS ,channel_4);
	usleep(measTime);
	
	input->ch5=MCP3008_analog_read( SPI_CS ,channel_5);
	usleep(measTime);
	
	input->ch6=MCP3008_analog_read( SPI_CS ,channel_6);
	usleep(measTime);
	
	input->ch7=MCP3008_analog_read( SPI_CS ,channel_7);
	usleep(measTime);
	
	
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

double leaveStation(char num){
	double rate;
	switch(num){
		case 1:
			rate=L5;
			break;
		case 2:
			rate=L2;
			break;
		case 3:
			rate=L3;
			break;
		case 5:
			rate=L5;
			break;
		case 6:
			rate=L1;
			break;
		case 7:
			rate=L4;
			break;
		case 8:
			rate=R2;
			break;
		case 9:
			rate=L5;
			break;
		case 10:
			rate=L2;
			break;
		case 11:
			rate=L3;
			break;
		case 12:
			rate=R1;
			break;
		case 13:
			rate=L5;
			break;
		case 15:
			rate=L5;
			break;
		case 16:
			rate=R5;
			break;
		case 17:
			rate=L5;
			break;
		case 18:
			rate=L2;
			break;
		case 19:
			rate=L3;
			break;
		case 22:
			rate=L1;
			break;
		case 23:
			rate=L4;
			break;
		case 24:
			rate=R3;
			break;
		case 25:
			rate=L5;
			break;
		case 26:
			rate=L2;
			break;
		case 27:
			rate=L3;
			break;
		case 28:
			rate=R4;
			break;
		case 29:
			rate=L5;
			break;
		case 30:
			rate=R5;
			break;
		default:
			rate=0;
			break;
	}
	return rate;
}

double enterStation(char num){
	double rate;
	switch(num){
		case 1:
			rate=L5;
			break;
		case 2:
			rate=L2;
			break;
		case 3:
			rate=L3;
			break;
		case 6:
			rate=L1;
			break;
		case 7:
			rate=L4;
			break;
		case 8:
			rate=R2;
			break;
		case 9:
			rate=R2;
			break;
		case 10:
			rate=R2;
			break;
		case 11:
			rate=R2;
			break;
		case 12:
			rate=R1;
			break;
		case 13:
			rate=R1;
			break;
		case 15:
			rate=L5;
			break;
		case 16:
			rate=R5;
			break;
		case 17:
			rate=R5;
			break;
		case 18:
			rate=R5;
			break;
		case 19:
			rate=R5;
			break;
		case 20:
			rate=R5;
			break;
		case 22:
			rate=R5;
			break;
		case 23:
			rate=R5;
			break;
		case 24:
			rate=R3;
			break;
		case 25:
			rate=R3;
			break;
		case 26:
			rate=R3;
			break;
		case 27:
			rate=R3;
			break;
		case 28:
			rate=R4;
			break;
		case 29:
			rate=R4;
			break;
		case 30:
			rate=R5;
			break;
		default:
			rate=0;
			break;
	}
	return rate;
}

double tracking_control(char stateNum,char mode){
	double rateValue=0;
	
	switch(mode){
		case '0':
			rateValue=leaveStation(stateNum);
			break;
		case '1':
			rateValue=enterStation(stateNum);
			break;
		default:
			break;
	}
	
	return rateValue;
}
