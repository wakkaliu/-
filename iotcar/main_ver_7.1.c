#include "wheel.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "infrared_digital.h"
#include "tcp_client_mod.h"
#include "time.h"

#define CYCLE 5000


#define DETECT_SIGNAL 1
#define OFF_SIGNAL 0

GPIO *barCode;
char mode;
#define base_duty 0.7

void start(void){
	gpio_action(rightWheelOutput);
	gpio_action(leftWheelOutput);
	usleep(CYCLE);
}

void initial(void){
	wheel_initial();
	//barCode=gpio_initial(2,"in");
	bcm2835_init();
    bcm2835_spi_begin();
  	//start();
}
int timeDiff(struct  timeval start,struct  timeval end){
	return 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
}



void *thread_tcp(void *arg);
void *thread_wheel_control(void *arg);
void *thread_wheel_output(void *arg);
/**
int decide_time(GPIO *A);
int det_base(GPIO *A);
int det_D(GPIO *A, float base_t, int *r);
int dec_num(float sp_time, float base);
void exception_check(GPIO *A, float base_t);
char bar_code(void);
**/
int main(int argc, char* argv[]){
	char state[5]={0},sum=0,position;
	
	pthread_t id_tcp;
	pthread_t id_control;
	pthread_t id_output;
	
	//if( (sock_tcp=socket(AF_INET, SOCK_STREAM, 0)) < 0 ){ //PF_INET 
    //    perror("socket()");
    //}
    //socka=client_initial(sock_tcp,"192.168.5.136");
	
	initial();

	LED=infrared_initial();
    printf("initial compelet\n");
	pthread_create(&id_tcp,NULL,thread_tcp,NULL);
	pthread_create(&id_control,NULL,thread_wheel_control,NULL);
	pthread_create(&id_output,NULL,thread_wheel_output,NULL);
	
	wheelControl->left_duty=0;
	wheelControl->right_duty=0;
	mode='0';
	
	printf("begin main while\n");
	/**
	while(1){
		infrared_state(LED);
		sum=a2d(state,LED);
		
		if (mode=='3'){
			mode='0';
		}
		
		
		if(mode=='0' || mode=='1'){
			if(tracking_control(sum,mode)>0.01){
				wheelControl->left_duty=base_duty*(1-tracking_control(sum,mode));
				wheelControl->right_duty=base_duty;
			}
			else if(tracking_control(sum,mode)<-0.01){
				wheelControl->left_duty=base_duty;
				wheelControl->right_duty=base_duty*(1+tracking_control(sum,mode));
			}
			else{
				wheelControl->left_duty=base_duty;
				wheelControl->right_duty=base_duty;
				
			}
		}
		else if(mode=='2'){
			wheelControl->left_duty=0;
			wheelControl->right_duty=0;
			position='w';
			//client_send_char(sock_tcp,socka,position);
			//mode=client_recv_char(sock_tcp,socka);
			//printf("%c\n",mode);
		}
		
		printf("%d\t",LED->ch3);
		printf("%d\t",LED->ch4);
		printf("%d\t",LED->ch5);
		printf("%d\t",LED->ch6);
		printf("%d\n",LED->ch7);
		int i;
		for(i=0;i<5;i++){
			printf("%d\t",state[i]);
		}
		printf("\n");
		
		
		//wheelControl->left_duty=base_duty*(1-tracking_control(sum,&mode));
		//wheelControl->right_duty=base_duty*(1+tracking_control(sum,&mode));
		
		printf("L: %f\tR: %f\n",wheelControl->left_duty,wheelControl->right_duty);
	}
	
	**/
	pthread_join(id_tcp,NULL);
	pthread_join(id_control,NULL);
	pthread_join(id_output,NULL);
	
	bcm2835_spi_end();
	bcm2835_close();
	
	return 0;	
}

void *thread_tcp(void *arg){
	char position;
    /**
    while(1){
		
		
    	position=bar_code();
		
		if((position>='a'&&position<='c')||(position>='A'&&position<='C')){
			client_send_char(sock_tcp,socka,position);
			mode=client_recv_char(sock_tcp,socka);
			printf("%c\n",mode);
		}
	}
	
	
	//close(sock_tcp);
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
		//printf("L:%f\tR:%f\n",MHz/wheelControl->left_period,MHz/wheelControl->right_period);
		usleep(CYCLE);
	}
	pthread_exit(NULL);
}

void *thread_wheel_output(void *arg){
	struct  timeval start;
    struct  timeval end;
    int time_diff;
	while(1){
		
		gettimeofday(&start,NULL);
		rightWheelOutput->data=1;
		leftWheelOutput->data=1;
		gpio_output(rightWheelOutput);
		gpio_output(leftWheelOutput);
		do{
			gettimeofday(&end,NULL);
			time_diff=timeDiff(start,end);
			
			if(time_diff>((int) CYCLE*wheelControl->right_duty)){
				rightWheelOutput->data=0;
				gpio_output(rightWheelOutput);
			}
			if(time_diff>((int) CYCLE*wheelControl->left_duty)){
				leftWheelOutput->data=0;
				gpio_output(leftWheelOutput);
			}
		}while(time_diff<=CYCLE);
	}
	pthread_exit(NULL);
}
