#include "wheel.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>



void initial(void){
	wheel_initial();
	
}

double control(int input,int offset){
	return 1000*((1/(double)offset)-(1/(double)input));
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
	wheelControl->left_duty=0.3;
	
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
		
		printf("\t\t\t\t\t\t\tfreq=%f\tperiod=%d\n",wheelControl->freq,wheelControl->period);
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
	char right_state,index,rCount[2];
	right_state=0;
	while(1){
		
		rCount[0]=0;
		rCount[1]=0;
		
		for(index=0;index<2;index++){
			do{
				gpio_state(rightWheelSpeed);
				rCount[index]++;
				usleep(1);
			}while(rightWheelSpeed->data==right_state);
		}
		wheelControl->right_duty=wheelControl->right_duty+control(rCount[1]+rCount[0],wheelControl->period);
		
		printf("R:%d  %f\n",rCount[1]+rCount[0],wheelControl->right_duty);
	}
	pthread_exit(NULL);
}

void *thread_left_wheel_control(void *arg){
	char left_state,index,lCount[2];
	left_state=0;
	while(1){
		
		lCount[0]=0;
		lCount[1]=0;
		
		for(index=0;index<2;index++){
			do{
				gpio_state(leftWheelSpeed);
				lCount[index]++;
				usleep(1);
			}while(leftWheelSpeed->data==left_state);
		}
		wheelControl->left_duty=wheelControl->left_duty+control(lCount[1]+lCount[0],wheelControl->period);
		
		printf("\t\t\t\tL:%d  %f\n",lCount[1]+lCount[0],wheelControl->left_duty);
	}
	pthread_exit(NULL);
}

void *thread_right_wheel_output(void *arg){
	while(1){
		rightWheelOutput->data=!rightWheelOutput->data;
		gpio_output(rightWheelOutput);
		
		if(rightWheelOutput->data==0){
			usleep((int)1000*(1-wheelControl->right_duty));
		}
		else{
			usleep((int)1000*wheelControl->right_duty);
		}
	}
	pthread_exit(NULL);
}

void *thread_left_wheel_output(void *arg){
	while(1){
		leftWheelOutput->data=!leftWheelOutput->data;
		gpio_output(leftWheelOutput);
		
		if(leftWheelOutput->data==0){
			usleep((int)1000*(1-wheelControl->left_duty));
		}
		else{
			usleep((int)1000*wheelControl->left_duty);
		}
	}
	pthread_exit(NULL);
}

