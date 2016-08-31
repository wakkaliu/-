#ifndef __SUPERSONIC_H__
#define __SUPERSONIC_H__

//gcc main.c supersonic.c -lwiringPi

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
 
#define TRIG 0
#define ECHO 1

void setup();
int getCM();

#endif
