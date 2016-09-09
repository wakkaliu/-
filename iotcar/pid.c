#include "pid.h"

PI* piControlInitial(void){
	PI *piFunc;
	piFunc = malloc(sizeof(PI));
	piFunc -> input=0;
	piFunc -> i_value=0;
	piFunc -> offset=0;
	piFunc -> output=0;
	return piFunc;
}

double piControl(PI *piFunc){
	double errorValue,procValue;
	errorValue=piFunc->offset-piFunc->input;
	piFunc->i_value=piFunc->i_value+errorValue*INTEGRAL;
	procValue=piFunc->i_value+PROPORTIONAL*errorValue;

	piFunc->output=GAIN*(piFunc->input+procValue);
	if(piFunc->output>=1){
		piFunc->output=1;
	}
	else if(piFunc->output<=0){
		piFunc->output=0;
	}
	return piFunc->output;
}
