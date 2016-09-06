#include "gpio_mod_ver2.h"
#include "time.h"

#define DETECT_SIGNAL 1
#define OFF_SIGNAL 0

char detect_fun(GPIO *A, GPIO *B);
char check_position(int *r);

char detect_fun(GPIO *A, GPIO *B){
    int signal_A, signal_B;
    int i = 0, count = 0, j=0; 
    int r[3];
    char result = NULL;

    signal_A = gpio_state(A);
    signal_B = gpio_state(B);

    while(1){
        // while B do not detect, stay here until detect signal
        while(1){
            usleep(100000);

            printf("%d, %d\n",A->data, B->data);


            if( gpio_state(B) == OFF_SIGNAL )
                ;
            else if( gpio_state(B) == DETECT_SIGNAL)
                break;
            else  
                printf("detect error!!!!!!!!!!!!!!!!!");
        }

        for(i = 0; i<3; i++){
            printf("enter black!");
                // check line is 1 or 0
            if(gpio_state(B) == DETECT_SIGNAL){
                if(gpio_state(A) == DETECT_SIGNAL)
                    r[i] = 1;
                else if(gpio_state(A) == OFF_SIGNAL)
                    r[i] = 0;
                else
                    printf("detect error!!!!!!!!!!!!!!!!!");

            }
            printf("in black!");

            while(1){
                usleep(100000);
                if(gpio_state(B) == DETECT_SIGNAL){
                    printf("in black= %d, %d\n",A->data, B->data);
                }
                else
                    break;
            }
            printf("enter white!");
            
            while(1){
                if(i == 2)
                    break;
                usleep(100000);
                if(gpio_state(B) == OFF_SIGNAL)
                    printf("in white= %d, %d\n",A->data, B->data);
                else
                    break;
            }
            printf("%d time result is %d\n", i, r[i]);
        }

        result = check_position(r);
        break;
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


int main(){
    char result = NULL;
    GPIO *A, *B;

    A=gpio_initial(3,"in");
    B=gpio_initial(2,"in");

    result = detect_fun(A, B);

    printf("current position is %c", result);

    return 0;
}