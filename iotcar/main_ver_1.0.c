#include "wheel.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void *thread_right_wheel_output(void *arg);
void *thread_left_wheel_output(void *arg);

/**
void *thread_right_wheel_speed(void *arg);
void *thread_left_wheel_speed(void *arg);
**/
int count;
int main(int argc, char* argv[]){
	
	pthread_t idr_output,idl_output;
	
	wheel_init();
	printf("initial compelet\n");
	
	
	pthread_create(&idr_output,NULL,thread_right_wheel_output,NULL);
	pthread_create(&idl_output,NULL,thread_left_wheel_output,NULL);
	//pthread_create(&idr_speed,NULL,thread_right_wheel_speed,NULL);
	//pthread_create(&idl_speed,NULL,thread_left_wheel_speed,NULL);
	
	count=10;
	int delta,i;
	while(1){
		if(count==10)
			delta=-1;
		else if (count==2)
			delta=1;
		printf("%d\n",count);
		for (i=0;i<80;i++){
			rightWheelOutput->data=1;
			leftWheelOutput->data=1;
			usleep(2500*count);
		
			rightWheelOutput->data=0;
			leftWheelOutput->data=0;
			usleep(2500*(10-count));
		}
		
		count+=delta;
	}
	
	
	pthread_join(idr_output,NULL);
	pthread_join(idl_output,NULL);
	//pthread_join(idr_speed,NULL);
	//pthread_join(idl_speed,NULL);
	return 0;
	
}
/**
void *thread_right_wheel_speed(void *arg){
	while(1){
		rightWheelOutput->data=0;
		usleep(10*count);
		rightWheelOutput->data=1;
		usleep(10*(10-count));
	}
	pthreat_exit(NULL);
}
void *thread_left_wheel_speed(void *arg){
	while(1){
		leftWheelOutput->data= 1;
		usleep(100);
		leftWheelOutput->data= 0;
		usleep(900);
	}
	pthreat_exit(NULL);
}
**/

void *thread_right_wheel_output(void *arg){
	while(1){
		gpio_output(rightWheelOutput);
	}
	pthreat_exit(NULL);
}

void *thread_left_wheel_output(void *arg){
	while(1){
		gpio_output(leftWheelOutput);
	}
	pthreat_exit(NULL);
}

		/*
		int temp=0,count=0;
		gpio_state(rightWheelSpeed);
		if (rightWheelSpeed->data==temp)
			count++;
		else{
			printf("%d\n",count);
			temp=!temp;
			count=1;
		}
		*/
		
		/*
		gpio_state(leftWheelSpeed);
		if (leftWheelSpeed->data==temp)
			count++;
		else{
			printf("%d\n",count);
			temp=!temp;
			count=1;
		}
		*/	
