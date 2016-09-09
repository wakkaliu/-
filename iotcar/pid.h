#ifndef __PID_H__
#define __PID_H__

#define PROPORTIONAL 1
#define INTEGRAL -0.00000000005
#define GAIN 1


#include <stdlib.h>
typedef struct pi{
	double input;
	double offset;
	double i_value;
	double output;
}PI;

PI* piControlInitial(void);
double piControl(PI *piFunc);

#endif
