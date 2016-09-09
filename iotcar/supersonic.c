#include "supersonic.h"
void setup(void){
        trig = gpio_initial (TRIG, "out");
        echo = gpio_initial (ECHO, "in");
 
        //TRIG pin must start LOW
        trig -> data = 0;
        gpio_output(trig);
        usleep(500);
}
 
int getCM(void){
        //Send trig pulse
        trig -> data = 1;
        gpio_output(trig);
        usleep(20);
        trig -> data = 0;
        gpio_output(trig);
 
        //Wait for echo start
        while(gpio_state(echo)  == 0);
 
        //Wait for echo end
        struct timeval startTime;
        struct timeval endTime;
        gettimeofday(&startTime,NULL);
        
        while(gpio_state(echo) == 1);
        gettimeofday(&endTime,NULL);
 
        //Get distance in cm
        int distance = (1000000 * (endTime.tv_sec-startTime.tv_sec)+ endTime.tv_usec-startTime.tv_usec)/58;
 
        return distance;
}
 
