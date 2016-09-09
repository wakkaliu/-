#include "wheel.h"

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

double centerFrequency;
char flag,com,checkpoint;

int main(int argc, char* argv[]){
	
	pthread_t idr_output,idl_output;
	pthread_t idr_control,idl_control;
	
	flag=0,checkpoint=0;
	initial();
	centerFrequency=initialFrequency;
	printf("initial compelet\n");
	
	pthread_create(&idr_output,NULL,thread_right_wheel_output,NULL);
	pthread_create(&idl_output,NULL,thread_left_wheel_output,NULL);
	pthread_create(&idr_control,NULL,thread_right_wheel_control,NULL);
	pthread_create(&idl_control,NULL,thread_left_wheel_control,NULL);
	
	centerFrequency=initialFrequency;
	
	while(1){
		flag=flag^checkpoint;
		infrared_state(LED);
		
		if(flag){
			//sand checkpoint to server.
			
			//get command
			if ((com == 0) || (com == 1)){
				changeMode(LED,com);
				com=5;
			}
			turnAround(intersection(LED,&flag));
		}
		else{
			turnAround(straight(LED));
		}
		
		speed_control(&centerFrequency);
		usleep(100000);
	}
	
	
	pthread_join(idr_output,NULL);
	pthread_join(idl_output,NULL);
	pthread_join(idr_control,NULL);
	pthread_join(idl_control,NULL);
	return 0;	
}


