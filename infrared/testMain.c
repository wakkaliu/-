#include <stdio.h>
#include <unistd.h>
#include "infrared_digital.h"

int main(int argc,char* argv[]){
	printf("test main.c\n");
	int i;
	char state[5]={0};
	
	bcm2835_init();
    bcm2835_spi_begin();
    
	LED=infrared_initial();
	
	while(1){
		infrared_state(LED);
		printf("%d\t",LED->ch3);
		printf("%d\t",LED->ch4);
		printf("%d\t",LED->ch5);
		printf("%d\t",LED->ch6);
		printf("%d\n",LED->ch7);
	
	
		a2d(state,LED);
		for(i=0;i<5;i++){
			printf("%d\t",state[i]);
		}
		printf("\n");
		usleep(1000000);
	}
	
	bcm2835_spi_end();
	bcm2835_close();
	return 0;
}
