#include "wheel.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "infrared_digital.h"
#include "tcp_client_mod.h"


#define CYCLE 25000
#define base_duty 0.8

void start(void){
	gpio_action(rightWheelOutput);
	gpio_action(leftWheelOutput);
	usleep(CYCLE);
}

void initial(void){
	wheel_initial();
	bcm2835_init();
    bcm2835_spi_begin();
  	//start();
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



void *thread_tcp(void *arg);
void *thread_wheel_control(void *arg);
void *thread_right_wheel_output(void *arg);
void *thread_left_wheel_output(void *arg);

char mode;
int main(int argc, char* argv[]){
	char state[5]={0},sum=0,i;
	pthread_t id_tcp;
	pthread_t id_control;
	pthread_t idr_output,idl_output;
	
	initial();
	printf("initial compelet\n");
	
	
	LED=infrared_initial();
    
	pthread_create(&id_tcp,NULL,thread_tcp,NULL);
	pthread_create(&id_control,NULL,thread_wheel_control,NULL);
	pthread_create(&idr_output,NULL,thread_right_wheel_output,NULL);
	pthread_create(&idl_output,NULL,thread_left_wheel_output,NULL);
	
	wheelControl->left_duty=base_duty;
	wheelControl->right_duty=base_duty;
	while(1){
		infrared_state(LED);
		sum=a2d(state,LED);
		
		//mode=tcp_char-48;
		tcp_char='1';
		mode='1';
		if(mode=='0'){
			wheelControl->left_duty=0;
			wheelControl->right_duty=0;
		}
		else if(mode=='1' || mode=='2'){
			if(tracking_control(sum,&mode)>0){
				wheelControl->left_duty=base_duty*(1-tracking_control(sum,&mode));
				wheelControl->right_duty=base_duty;
			}
			else if(tracking_control(sum,&mode)<0){
				wheelControl->left_duty=base_duty;
				wheelControl->right_duty=base_duty*(1+tracking_control(sum,&mode));
			}
			else{
				wheelControl->left_duty=base_duty;
				wheelControl->right_duty=base_duty;
			}
		}
		else if(mode=='3'){
			wheelControl->left_duty=base_duty;
			wheelControl->right_duty=base_duty;
			usleep(CYCLE);
		}
		
		printf("%d\t",LED->ch3);
		printf("%d\t",LED->ch4);
		printf("%d\t",LED->ch5);
		printf("%d\t",LED->ch6);
		printf("%d\n",LED->ch7);
		for(i=0;i<5;i++){
			printf("%d\t",state[i]);
		}
		printf("\n");
		
		
		//wheelControl->left_duty=base_duty*(1-tracking_control(sum,&mode));
		//wheelControl->right_duty=base_duty*(1+tracking_control(sum,&mode));
		
		printf("L: %f\tR: %f\n",wheelControl->left_duty,wheelControl->right_duty);
	}
	
	
	pthread_join(id_tcp,NULL);
	pthread_join(id_control,NULL);
	pthread_join(idr_output,NULL);
	pthread_join(idl_output,NULL);
	
	bcm2835_spi_end();
	bcm2835_close();
	return 0;	
}

void *thread_tcp(void *arg){
	/**
	if( (sock_tcp=socket(AF_INET, SOCK_STREAM, 0)) < 0 ){ //PF_INET 
        perror("socket()");
    }
    
	socka=client_initial(sock_tcp,"127.0.0.1");
	
	//socka=client_initial(sock_tcp,"127.0.0.1");
	while(1){
		if(tcp_char>='0'&&tcp_char<='3'){
			client_send_char(sock_tcp,socka,tcp_char);
			tcp_char=client_recv_char(sock_tcp,socka);
		}
		
		
		usleep(CYCLE);
	}
	close(sock_tcp);
	**/
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
			
		}
		LState=leftWheelSpeed->data;
		
		gpio_state(rightWheelSpeed);
		if((RState==0)&&(rightWheelSpeed->data)){
			gettimeofday(&rightEnd,NULL);
			
			wheelControl->right_period=timeDiff(rightStart,rightEnd);
			gettimeofday(&rightStart,NULL);
			
			
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
		
		if(leftWheelOutput->data==0){
			usleep((int) CYCLE*(1-wheelControl->left_duty));
		}
		else{
			usleep((int) CYCLE*wheelControl->left_duty);
		}
	}
	pthread_exit(NULL);
}
