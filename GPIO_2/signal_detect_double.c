#include "signal_detect_double.h"

char detect_fun(GPIO *A, GPIO *B){
    int signal_A, signal_B;
    int i = 0, count = 0, j=0; 
    int r[3];
    char result = NULL;
    float t_check = 0.0;
    long fiveseconds = CLOCKS_PER_SEC * 5;

    signal_A = gpio_state(A);
    signal_B = gpio_state(B);

    while(1){
        // while B do not detect, stay here until detect signal
        while(1){
            usleep(100000);

            printf("%d, %d\n",gpio_state(A), gpio_state(B));


            if( gpio_state(B) == OFF_SIGNAL )
                ;
            else if( gpio_state(B) == DETECT_SIGNAL)
                break;
            else  
                printf("detect error!!!!!!!!!!!!!!!!!");
        }

        for(i = 0; i<3; i++){
            //printf("enter black!");
                // check line is 1 or 0
            printf("in black= %d, %d\n",A->data, B->data);
            if(gpio_state(B) == DETECT_SIGNAL){
                if(gpio_state(A) == DETECT_SIGNAL)
                    r[i] = 1;
                else if(gpio_state(A) == OFF_SIGNAL)
                    r[i] = 0;
                else
                    printf("detect error!!!!!!!!!!!!!!!!!");

            }
            //printf("in black!");

            while(1){
                usleep(1000);
                if(gpio_state(B) == DETECT_SIGNAL){
                    //printf("in black= %d, %d\n",A->data, B->data);
                }
                else
                    break;
            }
            //printf("enter white!");

            clock_t begin = clock();
            while(1){
                if(i == 2)
                    break;
                usleep(1000);
                if(gpio_state(B) == OFF_SIGNAL){
                    //printf("in white= %d, %d\n",A->data, B->data);
                    clock_t end = clock();
                    t_check = end - begin;
                    //printf("t_check_time = %.2f\n", t_check);
                    if( t_check > fiveseconds)
                        break;
                }
                else
                    break;
            }
            if( t_check > fiveseconds)
                break;
            printf("\n %d time result is %d\n", i, r[i]);
        }
        if( t_check > fiveseconds)
            printf("I have arrive 5 secs");
        else{
            result = check_position(r);
            break;
        }
        
    }
}

char check_position(int *r){
    printf("r[0]=%d, r[1]=%d, r[2]=%d", r[0], r[1], r[2]);
    if((r[0] == 0) && (r[1] == 0) && (r[2] == 0))
                return 'a';
            else if((r[0] == 0) && (r[1] == 0) && (r[2] == 1))
                return 'b';
            else if((r[0] == 0) && (r[1] == 1) && (r[2] == 0))
                return 'c';
            else if((r[0] == 1) && (r[1] == 0) && (r[2] == 0))
                return 'A';
            else if((r[0] == 1) && (r[1] == 0) && (r[2] == 1))
                return 'B';
            else if((r[0] == 1) && (r[1] == 1) && (r[2] == 0))
                return 'C';
            else{
                return 'f';
                printf("position error!!!");
            }
}
