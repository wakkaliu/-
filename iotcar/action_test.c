#include "wheel.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>




void initial(void){
	wheel_initial();
}

void *thread_tcp(void *arg);
void *thread_right_wheel_output(void *arg);
void *thread_left_wheel_output(void *arg);



int main(int argc, char* argv[]){
	pthread_t id_tcp;
	pthread_t idr_output,idl_output;
	
	initial();
	printf("initial compelet\n");
	
	wheelControl->right_duty=0.98;
	wheelControl->left_duty=0.98;
	
	pthread_create(&id_tcp,NULL,thread_tcp,NULL);
	pthread_create(&idr_output,NULL,thread_right_wheel_output,NULL);
	pthread_create(&idl_output,NULL,thread_left_wheel_output,NULL);
	
	char rState=0,lState=0;
	int rCount=0,lCount=0;
	while(1){
		
		
		
		
		
		
		gpio_state(rightWheelSpeed);
		gpio_state(leftWheelSpeed);
		if (rightWheelSpeed->data==rState){
			rCount++;
		}
		else{
			printf("R:%d\tstate:%d\n",rCount,rState);
			rCount=1;
			rState=rightWheelSpeed->data;
		}
		if (leftWheelSpeed->data==lState){
			lCount++;
		}
		else{
			printf("\t\t\tL:%d\tstate:%d\n",lCount,lState);
			lCount=1;
			lState=leftWheelSpeed->data;
		}
		usleep(1);
	}
	
	pthread_join(id_tcp,NULL);
	pthread_join(idr_output,NULL);
	pthread_join(idl_output,NULL);
	return 0;	
}

void *thread_tcp(void *arg){
	
	pthread_exit(NULL);
}
void *thread_right_wheel_output(void *arg){
	while(1){
		rightWheelOutput->data=!rightWheelOutput->data;
		gpio_output(rightWheelOutput);
		
		if(rightWheelOutput->data==0){
			usleep(10000*(1-wheelControl->right_duty));
		}
		else{
			usleep(10000*wheelControl->right_duty);
		}
	}
	pthread_exit(NULL);
}

void *thread_left_wheel_output(void *arg){
	while(1){
		leftWheelOutput->data=!leftWheelOutput->data;
		gpio_output(leftWheelOutput);
		
		if(leftWheelOutput->data==0){
			usleep(10000*(1-wheelControl->left_duty));
		}
		else{
			usleep(10000*wheelControl->left_duty);
		}
	}
	pthread_exit(NULL);
}

