#include "wheel.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "infrared_digital.h"
#include "tcp_client_mod.h"
#include "signal_detect_double.h"
#include "supersonic.h"

#define CYCLE 5000


GPIO *barCode;
char mode,supersonicFlag;
double base_duty;

void start(void){
	gpio_action(rightWheelOutput);
	gpio_action(leftWheelOutput);
	usleep(CYCLE);
}

void initial(void){
	
	/**
	if( (sock_tcp=socket(AF_INET, SOCK_STREAM, 0)) < 0 ){ //PF_INET 
        perror("socket()");
    }
    socka=client_initial(sock_tcp,"192.168.5.32");
	**/
	
	setup();
	wheel_initial();
	barCodeA=gpio_initial(2,"in");
	barCodeB=gpio_initial(3,"in");
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


int main(int argc, char* argv[]){
	char state[5]={0},sum=0,position,tempMode;
	supersonicFlag=0;
	base_duty=0.7;
	pthread_t id_tcp;
	pthread_t id_control;
	pthread_t id_output;
	
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
	
	while(1){
		infrared_state(LED);
		sum=a2d(state,LED);
		
		if (mode=='3'){
			mode='0';
		}
		
		
		if(supersonicFlag==1){
			
			wheelControl->left_duty=0;
			wheelControl->right_duty=0;
			//printf("danger\n");
			position='d';
			
			//client_send_char(sock_tcp,socka,position);
			usleep(100000);
		}
		else{
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
				printf("%c\n",mode);
			}
		}
		
		
		
		/**
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
		**/
	}
	
	
	pthread_join(id_tcp,NULL);
	pthread_join(id_control,NULL);
	pthread_join(id_output,NULL);
	
	bcm2835_spi_end();
	bcm2835_close();
	
	return 0;	
}

void *thread_tcp(void *arg){
	char position=0;
	int distant=100;
    
    while(1){
    	
    	//distant=getCM();
    	if(distant<=20){
    		supersonicFlag=1;
		}
		else if(distant>30){
			supersonicFlag=0;
		}
    	//printf("supersonic : %d\n",distant);
    	//usleep(100000);
		
    	position=detect_fun(barCodeA, barCodeB);
		printf("\n current position is %c\n ", position);
		/**
		if((position>='a'&&position<='c')||(position>='A'&&position<='C')){
			client_send_char(sock_tcp,socka,position);
			mode=client_recv_char(sock_tcp,socka);
			printf("%c\n",mode);
		}
		**/
	}
	
	
	//close(sock_tcp);
	
	pthread_exit(NULL);
}

void *thread_wheel_control(void *arg){
	char LState=0,RState=0,LCount=0,RCount=0;
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
			LCount=0;
		}
		else{
			LCount++;
			if(LCount>=2000){
				gettimeofday(&leftEnd,NULL);
			
				wheelControl->left_period=timeDiff(leftStart,leftEnd);
				gettimeofday(&leftStart,NULL);
				LCount=0;
			}
		}
		LState=leftWheelSpeed->data;
		
		gpio_state(rightWheelSpeed);
		if((RState==0)&&(rightWheelSpeed->data)){
			gettimeofday(&rightEnd,NULL);
			
			wheelControl->right_period=timeDiff(rightStart,rightEnd);
			gettimeofday(&rightStart,NULL);
			RCount=0;
		}
		else{
			RCount++;
			if(RCount>=2000){
				gettimeofday(&rightEnd,NULL);
			
				wheelControl->right_period=timeDiff(rightStart,rightEnd);
				gettimeofday(&rightStart,NULL);
				RCount=0;
			}
		}
		RState=rightWheelSpeed->data;
		//printf("L:%f\tR:%f\n",MHz/(double) wheelControl->left_period,MHz/(double) wheelControl->right_period);
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

//---supersonic.h---
void setup(void){
        trig = gpio_initial (TRIG, "out");
        echo = gpio_initial (ECHO, "in");
 
        //TRIG pin must start LOW
        trig -> data = 0;
        gpio_output(trig);
        usleep(500);
}
 
int getCM(void){
    //Send trig pulse
    trig -> data = 1;
    gpio_output(trig);
    usleep(20);
    trig -> data = 0;
    gpio_output(trig);
 
    //Wait for echo start
    while(gpio_state(echo)  == 0);
 
    //Wait for echo end
    struct timeval startTime;
    struct timeval endTime;
    gettimeofday(&startTime,NULL);
        
    while(gpio_state(echo) == 1);
    gettimeofday(&endTime,NULL);
 
    //Get distance in cm
    return (1000000 * (endTime.tv_sec-startTime.tv_sec)+ endTime.tv_usec-startTime.tv_usec)/58;
}
//----------------------------
