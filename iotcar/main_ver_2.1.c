#include "wheel.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void *thread_right_wheel_output(void *arg);
void *thread_left_wheel_output(void *arg);
void *thread_right_wheel_control(void *arg);
void *thread_left_wheel_control(void *arg);

int main(int argc, char* argv[]){
	
	pthread_t idr_output,idl_output;
	pthread_t idr_control,idl_control;
	
	wheel_initial();
	printf("initial compelet\n");
	
	pthread_create(&idr_output,NULL,thread_right_wheel_output,NULL);
	pthread_create(&idl_output,NULL,thread_left_wheel_output,NULL);
	pthread_create(&idr_control,NULL,thread_right_wheel_control,NULL);
	pthread_create(&idl_control,NULL,thread_left_wheel_control,NULL);
	
	double frequency=10,delta;
	
	while(1){
		if(frequency>=31){
			delta=-0.2;
		}
		else if(frequency<=10){
			delta=0.5;
		}
		frequency+=delta;
		printf("%f\t%f\n",frequency,MHz/frequency);
		
		rightWheelControl->freq=frequency;
		leftWheelControl->freq=frequency;
		
		usleep(500000);	
	}
	
	
	pthread_join(idr_output,NULL);
	pthread_join(idl_output,NULL);
	pthread_join(idr_control,NULL);
	pthread_join(idl_control,NULL);
	return 0;	
}

void *thread_right_wheel_control(void *arg){
	while(1){
		freq2period(rightWheelControl);
		rightWheelControl->plus=!(rightWheelControl->plus);
		usleep(rightWheelControl->period_2);
	}
	pthread_exit(NULL);
}

void *thread_left_wheel_control(void *arg){
	while(1){
		freq2period(leftWheelControl);
		leftWheelControl->plus=!(leftWheelControl->plus);
		usleep(leftWheelControl->period_2);
	}
	pthread_exit(NULL);
}

void *thread_right_wheel_output(void *arg){
	while(1){
		LED2->data=gpio_state(rightWheelSpeed);
		gpio_output(LED2);
		
		rightWheelOutput->data=(rightWheelSpeed->data)^(rightWheelControl->plus);
		gpio_output(rightWheelOutput);
	}
	pthread_exit(NULL);
}

void *thread_left_wheel_output(void *arg){
	while(1){
		LED3->data=gpio_state(leftWheelSpeed);
		gpio_output(LED3);
		
		leftWheelOutput->data=(leftWheelSpeed->data)^(leftWheelControl->plus);
		gpio_output(leftWheelOutput);
	}
	pthread_exit(NULL);
}



