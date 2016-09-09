#include "wheel.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void *thread_right_wheel_output(void *arg);
void *thread_left_wheel_output(void *arg);

int plus;

int main(int argc, char* argv[]){
	
	pthread_t idr_output,idl_output;
	
	wheel_init();
	printf("initial compelet\n");
	
	
	pthread_create(&idr_output,NULL,thread_right_wheel_output,NULL);
	pthread_create(&idl_output,NULL,thread_left_wheel_output,NULL);
	
	int i,period=50000,delta;
	
	while(1){
		if(period==33000){
			delta=20;
		}
		else if(period==50000){
			delta=-20;
		}
		period+=delta;
		
		printf("%d\n",period);
		for(i=0;i<(500000/period);i++){
			plus=1;
			usleep(0.5*period);
			plus=0;
			usleep(0.5*period);
		}
		
	}
	
	
	pthread_join(idr_output,NULL);
	pthread_join(idl_output,NULL);
	return 0;	
}

void *thread_right_wheel_output(void *arg){
	while(1){
		LED2->data=gpio_state(rightWheelSpeed);
		gpio_output(LED2);
		
		rightWheelOutput->data=(rightWheelSpeed->data)^plus;
		gpio_output(rightWheelOutput);
	}
	pthread_exit(NULL);
}

void *thread_left_wheel_output(void *arg){
	while(1){
		LED3->data=gpio_state(leftWheelSpeed);
		gpio_output(LED3);
		
		leftWheelOutput->data=(leftWheelSpeed->data)^plus;
		gpio_output(leftWheelOutput);
	}
	pthread_exit(NULL);
}


