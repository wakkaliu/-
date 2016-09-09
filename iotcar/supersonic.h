#ifndef __SUPERSONIC_H__
#define __SUPERSONIC_H__

#include <stdio.h>
#include <stdlib.h>
#include "wheel.h"
#include <sys/time.h>
#include <unistd.h>
 
#define TRIG 17
#define ECHO 18
GPIO *trig, *echo;

void setup(void);
int getCM(void);

#endif
