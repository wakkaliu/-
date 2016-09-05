#include "signal_detect.h"

int main(int argc, char const *argv[])
{
    /* code */
    GPIO *A;
    // base time for counting signal
    int r[3], status, i;
    float base_t = 0.0;
    char position;
    
    A=gpio_initial(2,"in");

    while(1){
        while(1){
            // decide the base time for counting
            base_t = det_base(A);
            if (base_t == 0){
                printf("not detect base_t\n");
            }
            else if (base_t > 0)
                break;
            else
                printf("base_t error");
            usleep(100000);
        }
        status = det_D(A, base_t, r);
            //check function det_D is success or not, return r[]
        if(status == 1){
            printf("det_D succees!!!\n");
            printf("r[] is:");
            for(i=0; i<3; i++)
                printf("%d",r[i]);
            printf("\n");
            break;
        }
        else
            printf("det_D false!!!\n");
    }
            if((r[0] == 0) && (r[1] == 0) && (r[2] == 0))
                position = 'a';
            else if((r[0] == 0) && (r[1] == 0) && (r[2] == 1))
                position = 'b';
            else if((r[0] == 0) && (r[1] == 1) && (r[2] == 0))
                position = 'c';
            else if((r[0] == 1) && (r[1] == 0) && (r[2] == 0))
                position = 'A';
            else if((r[0] == 1) && (r[1] == 0) && (r[2] == 1))
                position = 'B';
            else if((r[0] == 1) && (r[1] == 1) && (r[2] == 0))
                position = 'C';
            else{
                position = 'f';
                printf("position error!!!");
            }

            printf("position is %c\n", position);
    return 0;

}