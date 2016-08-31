#include <stdio.h>
#include <unistd.h>
#include "infrared_digital.h"

int main(int argc,char* argv[]){
	printf("test main.c\n");
	int i,temp;
	char mode,state[5]={0};
	mode=2;
	
	bcm2835_init();
    bcm2835_spi_begin();
    
	LED=infrared_initial();
	
	while(1){
		infrared_state(LED);
		printf("%d\t",LED->ch3);
		printf("%d\t",LED->ch4);
		printf("%d\t",LED->ch5);
		printf("%d\t",LED->ch6);
		printf("%d\t",LED->ch7);
		
	
		temp=a2d(state,LED);
		printf("%d\n",temp);
		for(i=0;i<5;i++){
			printf("%d\t",state[i]);
		}
		printf("\tcontrol:%f\n",tracking_control(temp,&mode));
		usleep(1000000);
	}
	
	bcm2835_spi_end();
	bcm2835_close();
	return 0;
}
