#ifndef __INFRARED_ANALOG_H__
#define __INFRARED_ANALOG_H__

#include <mcp3008.h>
#include <unistd.h>
#include <stdio.h>

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

typedef struct infraredLED{
	unsigned int rightLED:12;
	unsigned int frontLED:12;
	unsigned int leftLED:12;
	unsigned int tempRight:12;
	unsigned int tempFront:12;
	unsigned int tempLeft:12;
	unsigned int mod;
}INF;


INF *LED;

INF* infrared_initial(void);
void infrared_state(INF* input);
char barcode(INF *input);

double tracking_control(INF *input,char* mode);

#endif
