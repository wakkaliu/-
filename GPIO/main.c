#include "gpio_mod_ver2.h"
#include "time.h"

int decide_time(GPIO *A);
int det_base(GPIO *A);
int det_D(GPIO *A, float base_t, int *r);
int dec_num(float sp_time, float base);

int decide_time(GPIO *A)
{
    float time = 0.0;
    int signal;

    while(1){
        signal = gpio_state(A);
        printf("after stage 2");
        if(signal == 0){
            clock_t begin = clock();
            while(1){
                signal = gpio_state(A);
                if(signal == 0){}
                else
                    break;
            }
            clock_t end = clock();
            time = end - begin;
            return time;
        }
    }
}

// This function decide the base time for detecting signal
int det_base(GPIO *A)
{
    int signal, count = 0; 
    float base_time = 0.0, begin = 0.0, end = 0.0;

    while(1){
        signal = gpio_state(A);
        //printf("%d\n",A->data);
        if(signal == 0)
            count++;
            //printf("count: %d", count);
            if(count>=5){
                printf("I am break.......\n");
                base_time = decide_time(A);
                //printf("after stage 1");
                printf("base_time is %f\n", base_time);
                return base_time;
            }
        else if(signal == 1){
            count = 0;
            return 0;
        }
        //printf("after stage 1");
    }    
}
// detect the signal after base time is decided, count 3 times to decide r[]
int det_D(GPIO *A, float base_t, int *r)
{
    float time[3], ratio[3];
    int i, signal, count = 0;
    float spend_time;

    while(1){
        signal = gpio_state(A);
        printf("A->data: %d\n", A->data);
        if(signal == 0){
            count++;
            printf("det_D count =%d\n", count);
        
            if(count>=5){
                printf("I am det_D break!\n");
                break;
            }
        }
        else{
            //printf("I am return 0\n"); 
            count = 0;
            printf("I am det_D return 0!\n");
            //return 0;
        }    
    }

     for( i=0; i<3; i++){
        printf("%d time gpio\n", i);
        signal = gpio_state(A);

        if(signal == 1){
            while(1){
                signal = gpio_state(A);
                printf("gpio signal = %d\n", signal);
                if(signal == 0)
                    break;
            }
        }

        if(signal == 0){
            clock_t begin = clock();
            while(1){
                signal = gpio_state(A);
                if(signal == 0){}
                    else
                        break;
                }
                clock_t end = clock();
                spend_time = end - begin;
                time[i] = spend_time;
                ratio[i] = spend_time/base_t;
                r[i] = dec_num(spend_time, base_t);
            }
        
    }
    printf("r[0]=%f, time[0] = %f, ratio[0] =%f, r[1]=%f, time[1] = %f,  ratio[1] = %f, \
            r[2] = %f, time[2] = %f, ratio[2] = %f\n",r[0], time[0], ratio[0], r[1],  \
            time[1], ratio[1], r[2], time[2], ratio[2]);
    return 1;
}
//decide the signal is 1 or 0 base on the base time we measure at beginnig
int dec_num(float sp_time, float base)
{
    float ratio;
    ratio = sp_time/base;
    // decide the accurancy for 0
    if ((ratio > 0.8) && (ratio < 1.2)){
        return 0;
    }
    // decide the accurancy for 1
    else if((ratio > 1.6) && (ratio < 2.4)){
        return 1;
    }
    else
        printf("dec_num error!!");
}

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