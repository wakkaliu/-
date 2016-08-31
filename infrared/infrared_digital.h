#ifndef __INFRARED_DIGITAL_H__
#define __INFRARED_DIGITAL_H__

#include <mcp3008.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define SPI_CS 0
#define channel_0 0
#define channel_1 1
#define channel_2 2
#define channel_3 3
#define channel_4 4
#define channel_5 5
#define channel_6 6
#define channel_7 7

#define turnAroundConstance 50
#define ADC_divider 2048
#define normalMode 0
#define leaveMode 1
#define enterMode 2

#define ch3_h 270
#define ch3_l 250
#define ch4_h 320
#define ch4_l 300
#define ch5_h 220
#define ch5_l 200
#define ch6_h 290
#define ch6_l 270
#define ch7_h 280
#define ch7_l 260

typedef struct infraredLED{
	
	unsigned int ch3:12;
	unsigned int ch4:12;
	unsigned int ch5:12;
	unsigned int ch6:12;
	unsigned int ch7:12;
	unsigned int mod;
}INF;


INF *LED;

INF* infrared_initial(void);
void infrared_state(INF* input);
char trigger(char* state,int high_level,int low_level,int input);
char a2d(char *state,INF *input);
double straight(char num);
double enterStation(char num);
double leaveStation(char num);
double tracking_control(char stateNum,char* mode);

#endif
