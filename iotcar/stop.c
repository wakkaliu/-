#include "wheel.h"
#include <stdlib.h>
#include <unistd.h>

void initial(void){
	wheel_initial();
	//piControlInitial(piCenter);
}

int main(int argc, char* argv[]){
	
	initial();
	printf("initial compelet\n");
	
	gpio_stop(rightWheelOutput);
	gpio_stop(leftWheelOutput);
	
	gpio_output(rightWheelOutput);
	gpio_output(leftWheelOutput);
	return 0;	
}



