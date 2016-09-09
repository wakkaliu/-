#include "wheel.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#define CYCLE 10000

void initial(void){
	wheel_initial();
	
}

double control(double* duty,int input,int offset){
	double temp=0.001*MHz*((1/(double)offset)-(1/(double)input));
	if( (*duty+temp)>=1){
		*duty=0.99999;
	}
	else if ((*duty+temp)<=0){
		*duty=0.00001;
	}
	else{
		*duty+=temp;
	}
	return *duty;
}

int timeDiff(struct  timeval start,struct  timeval end){
	return 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
}

void start(void){
	gpio_action(rightWheelOutput);
	gpio_action(leftWheelOutput);
	usleep(100*CYCLE);
}

void *thread_tcp(void *arg);
void *thread_wheel_control(void *arg);
void *thread_right_wheel_output(void *arg);
void *thread_left_wheel_output(void *arg);


int main(int argc, char* argv[]){
	pthread_t id_tcp;
	pthread_t id_control;
	pthread_t idr_output,idl_output;
	
	initial();
	printf("initial compelet\n");
	
	start();
	
	pthread_create(&id_tcp,NULL,thread_tcp,NULL);
	pthread_create(&id_control,NULL,thread_wheel_control,NULL);
	pthread_create(&idr_output,NULL,thread_right_wheel_output,NULL);
	pthread_create(&idl_output,NULL,thread_left_wheel_output,NULL);
	
	
	double delta=0.01;
	while(1){/**
		if(wheelControl->freq<=initialFrequency){
			delta=0.01;
		}
		else if(wheelControl->freq>=maxFrequency){
			delta=-0.01;
		}
		wheelControl->freq+=delta;**/
		
		wheelControl->freq=15;
		freq2period(wheelControl);
		//printf("period:%d\tL:%d\tR:%d\n",wheelControl->period,wheelControl->left_period,wheelControl->right_period);
		printf("freq:%.5f\tL:%.5f\tR:%.5f\n",wheelControl->freq,MHz/(double)(wheelControl->left_period),MHz/(double)(wheelControl->right_period));
		usleep(1000000);
	}
	
	pthread_join(id_tcp,NULL);
	pthread_join(id_control,NULL);
	pthread_join(idr_output,NULL);
	pthread_join(idl_output,NULL);
	return 0;	
}

void *thread_tcp(void *arg){
	
	pthread_exit(NULL);
}

void *thread_wheel_control(void *arg){
	char LState=0,RState=0;
	struct  timeval leftStart;
    struct  timeval leftEnd;
    struct  timeval rightStart;
    struct  timeval rightEnd;
    
    gettimeofday(&leftStart,NULL);
    gettimeofday(&rightStart,NULL);
    
	while(1){
		gpio_state(leftWheelSpeed);
		
		if((LState==0)&&(leftWheelSpeed->data)){
			gettimeofday(&leftEnd,NULL);
			
			wheelControl->left_period=timeDiff(leftStart,leftEnd);
			gettimeofday(&leftStart,NULL);
			
			//printf("L: %d\n",wheelControl->left_period);
		}
		LState=leftWheelSpeed->data;
		
		gpio_state(rightWheelSpeed);
		if((RState==0)&&(rightWheelSpeed->data)){
			gettimeofday(&rightEnd,NULL);
			
			wheelControl->right_period=timeDiff(rightStart,rightEnd);
			gettimeofday(&rightStart,NULL);
			
			//printf("\t\t\t\t\tR: %d\n",wheelControl->right_period);
		}
		RState=rightWheelSpeed->data;
		
		usleep(CYCLE);
	}
	pthread_exit(NULL);
}

void *thread_right_wheel_output(void *arg){
	while(1){
		
		
		rightWheelOutput->data=!rightWheelOutput->data;
		gpio_output(rightWheelOutput);
		
		printf("\t\t\tR:%f\n",wheelControl->right_duty);
		if(rightWheelOutput->data==0){
			usleep((int) CYCLE*(1-wheelControl->right_duty));
		}
		else{
			usleep((int) CYCLE*wheelControl->right_duty);
		}
	}
	pthread_exit(NULL);
}

void *thread_left_wheel_output(void *arg){
	while(1){
		
		
		leftWheelOutput->data=!leftWheelOutput->data;
		gpio_output(leftWheelOutput);
		
		printf("L:%f\n",wheelControl->left_duty);
		if(leftWheelOutput->data==0){
			usleep((int) CYCLE*(1-wheelControl->left_duty));
		}
		else{
			usleep((int) CYCLE*wheelControl->left_duty);
		}
	}
	pthread_exit(NULL);
}
