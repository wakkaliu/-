#include "wheel.h"
#include <stdio.h>
#include <stdlib.h>
#include "pid.h"

//---------gpio_mod-----------------------------------
GPIO* gpio_initial(int number,char* pinMode){
	GPIO* pin;
	char str[50];
	FILE *gpioFile,*gpioGenerator;
	sprintf(str,"/sys/class/gpio/gpio%d/direction",number);
	
	pin = malloc(sizeof(GPIO));
	pin->en = 1;
	pin->data = 0;
	pin->pinNumber = number;
	pin->mode = pinMode;
	
	if((gpioFile=fopen(str,"r"))==0){
		gpioGenerator=fopen("/sys/class/gpio/export","w");
		fprintf(gpioGenerator,"%d",pin->pinNumber);
		fclose(gpioGenerator);
	}
	else{
		fclose(gpioFile);
	}
	gpioFile=fopen(str,"w");
	fprintf(gpioFile,pin->mode);
	fclose(gpioFile);
	return pin;
}

int gpio_state(GPIO *pin){
	char str[50];
	FILE *gpioFile;
	sprintf(str,"/sys/class/gpio/gpio%d/value",pin->pinNumber);
	
	if(pin->en){
		gpioFile=fopen(str,"r");
		fscanf(gpioFile,"%d",&(pin->data));
		fclose(gpioFile);
	}
	else{
		pin->data=0;
	}
	return pin->data;
}

void gpio_output(GPIO *pin){
	char str[50];
	FILE *gpioFile;
	sprintf(str,"/sys/class/gpio/gpio%d/value",pin->pinNumber);
	
	gpioFile=fopen(str,"w");
	if((pin->data)==0){
		fprintf(gpioFile,"%d",0);
	}
	else{
		fprintf(gpioFile,"%d",1);
	}
	fclose(gpioFile);
}

void gpio_stop(GPIO *pin){
	pin->en=0;
	pin->data=0;
}

void gpio_action(GPIO *pin){
	pin->en=1;
	pin->data=1;
	gpio_output(pin);
	
}

//----------------------------------------------------------

int freq2period(REF *inputData){ //frequency(Hz) to period(micro second)
	inputData->period=(int) (MHz/(inputData->freq));
	
	return (inputData->period);
}

//----------------------------------------------------------
void wheel_initial(void){
	
	rightWheelSpeed=gpio_initial(rightWheelSpeedPin,"in");
	leftWheelSpeed=gpio_initial(leftWheelSpeedPin,"in");
	rightWheelOutput=gpio_initial(rightWheelOutputPin,"out");
	leftWheelOutput=gpio_initial(leftWheelOutputPin,"out");
	
	wheelControl = malloc(sizeof(REF));
	wheelControl->left_duty=1;
	wheelControl->right_duty=1;
	wheelControl->rate=0;
	wheelControl->left_period=0;
	wheelControl->right_period=0;
	wheelControl->left_plus=0;
	wheelControl->right_plus=0;
	wheelControl->freq=initialFrequency;
	freq2period(wheelControl);
	
	
}
