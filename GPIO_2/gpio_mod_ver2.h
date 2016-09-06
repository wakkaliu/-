#ifndef __GPIO_MOD_VER2_H__
#define __GPIO_MOD_VER2_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct gpio{
        unsigned char en:1;
        int pinNumber;
        char* mode;
        unsigned short data;
}GPIO;

GPIO* gpio_initial(int number,char* pinMode);
int gpio_state(GPIO *pin);
void gpio_output(GPIO *pin);
void gpio_stop(GPIO *pin);
void gpio_action(GPIO *pin);
void gpio_HIGH(GPIO* pin);
void gpio_LOW(GPIO* pin);

/**
read_example:
GPIO *A;
A=gpio_initial(23,"in");
gpio_state(A);
printf("%d\n",A->data);

write_example:
GPIO *B;
B=gpio_initial(22,"out");
B->data=1;
gpio_output(B);
**/
#endif