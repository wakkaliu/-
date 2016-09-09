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

double control(int input,int offset){
	return 10000*((1/(double)offset)-(1/(double)input));
}

int timeDiff(struct  timeval start,struct  timeval end){
	return 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
}
void *thread_tcp(void *arg);
void *thread_right_wheel_control(void *arg);
void *thread_left_wheel_control(void *arg);
void *thread_right_wheel_output(void *arg);
void *thread_left_wheel_output(void *arg);



int main(int argc, char* argv[]){
	pthread_t id_tcp;
	pthread_t idr_control,idl_control;
	pthread_t idr_output,idl_output;
	
	initial();
	printf("initial compelet\n");
	
	wheelControl->right_duty=0.85;
	wheelControl->left_duty=0.85;
	
	pthread_create(&id_tcp,NULL,thread_tcp,NULL);
	pthread_create(&idr_control,NULL,thread_right_wheel_control,NULL);
	pthread_create(&idl_control,NULL,thread_left_wheel_control,NULL);
	pthread_create(&idr_output,NULL,thread_right_wheel_output,NULL);
	pthread_create(&idl_output,NULL,thread_left_wheel_output,NULL);
	
	
	double delta=0.01;
	while(1){
		/**if(wheelControl->freq<=initialFrequency){
			delta=0.01;
		}
		else if(wheelControl->freq>=maxspeedFrequency){
			delta=-0.01;
		}**/
		//wheelControl->freq+=delta;
		wheelControl->freq=50;
		freq2period(wheelControl);
		printf("LW:%fRW:%f\n",wheelControl->left_duty,wheelControl->right_duty);
		//printf("\t\t\t\t\t\t\tfreq=%f\tperiod=%d\n",wheelControl->freq,wheelControl->period);
		usleep(1000000);
	}
	
	pthread_join(id_tcp,NULL);
	pthread_join(idr_control,NULL);
	pthread_join(idl_control,NULL);
	pthread_join(idr_output,NULL);
	pthread_join(idl_output,NULL);
	return 0;	
}

void *thread_tcp(void *arg){
	
	pthread_exit(NULL);
}

void *thread_right_wheel_control(void *arg){
	char tempState=0;
	struct  timeval rightStart;
    struct  timeval rightEnd;
    gettimeofday(&rightStart,NULL);
	while(1){
		gpio_state(rightWheelSpeed);
		if((tempState==0)&&(rightWheelSpeed->data)){
			gettimeofday(&rightEnd,NULL);
			
			wheelControl->right_period=timeDiff(rightStart,rightEnd);
			gettimeofday(&rightStart,NULL);
			
			printf("\t\t\t\t\tR: %d\n",wheelControl->right_period);
		}
		tempState=rightWheelSpeed->data;
		usleep(CYCLE);
	}
	pthread_exit(NULL);
}

void *thread_left_wheel_control(void *arg){
	char tempState=0;
	struct  timeval leftStart;
    struct  timeval leftEnd;
    gettimeofday(&leftStart,NULL);
	while(1){
		gpio_state(leftWheelSpeed);
		if((tempState==0)&&(leftWheelSpeed->data)){
			gettimeofday(&leftEnd,NULL);
			
			wheelControl->left_period=timeDiff(leftStart,leftEnd);
			gettimeofday(&leftStart,NULL);
			
			printf("L: %d\n",wheelControl->left_period);
		}
		tempState=leftWheelSpeed->data;
		usleep(CYCLE);
	}
	pthread_exit(NULL);
}

void *thread_right_wheel_output(void *arg){
	while(1){
		
		wheelControl->right_duty=wheelControl->right_duty+control(wheelControl->right_period,wheelControl->period);
		
		rightWheelOutput->data=!rightWheelOutput->data;
		gpio_output(rightWheelOutput);
		
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
		
		wheelControl->left_duty=wheelControl->left_duty+control(wheelControl->left_period,wheelControl->period);
		
		leftWheelOutput->data=!leftWheelOutput->data;
		gpio_output(leftWheelOutput);
		
		if(leftWheelOutput->data==0){
			usleep((int) CYCLE*(1-wheelControl->left_duty));
		}
		else{
			usleep((int) CYCLE*wheelControl->left_duty);
		}
	}
	pthread_exit(NULL);
}

