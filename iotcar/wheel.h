#ifndef __WHEEL_H__
#define __WHEEL_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MHz 1000000
#define initialFrequency 10
#define maxFrequency 60

#define rightWheelSpeedPin 23
#define leftWheelSpeedPin 24
#define rightWheelOutputPin 27
#define leftWheelOutputPin 22



typedef struct gpio{
	unsigned char en:1;
	int pinNumber;
	char* mode;
	int data;
}GPIO;

typedef struct ref{
	double freq;
	unsigned int period;
	double rate;
	double right_duty;
	double left_duty;
	unsigned int right_period;
	unsigned int left_period;
	char right_plus:1;
	char left_plus:1;
}REF;


GPIO* gpio_initial(int number,char* pinMode);

int gpio_state(GPIO *pin);
void gpio_output(GPIO *pin);
void gpio_stop(GPIO *pin);
void gpio_action(GPIO *pin);

int freq2period(REF *inputData);
void wheel_initial(void);


GPIO *rightWheelSpeed,*leftWheelSpeed,*rightWheelOutput,*leftWheelOutput;
REF *wheelControl;


#endif
