#include "wheel.h"

double centerFrequency;

int main(int argc, char* argv[]){
	
	pthread_t idr_output,idl_output;
	pthread_t idr_control,idl_control;
	
	centerFrequency=initialFrequency;
	
	initial();
	printf("initial compelet\n");
	
	pthread_create(&idr_output,NULL,thread_right_wheel_output,NULL);
	pthread_create(&idl_output,NULL,thread_left_wheel_output,NULL);
	pthread_create(&idr_control,NULL,thread_right_wheel_control,NULL);
	pthread_create(&idl_control,NULL,thread_left_wheel_control,NULL);
	
	
	while(1){

		
		if(gpio_state(butten1)==0){
			rightWheelOutput->en++;
		}
		if(gpio_state(butten2)==0){
			leftWheelOutput->en++;
		}
		
		controlCenterInput(0);
		speed_control(&centerFrequency);
		
		usleep(100000);
	}
	
	
	pthread_join(idr_output,NULL);
	pthread_join(idl_output,NULL);
	pthread_join(idr_control,NULL);
	pthread_join(idl_control,NULL);
	return 0;	
}


