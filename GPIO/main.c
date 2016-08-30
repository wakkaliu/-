#include "gpio_mod_ver2.h"

int main(int argc, char const *argv[])
{
    /* code */
    GPIO *A;
    A=gpio_initial(2,"in");
    while(1){
        gpio_state(A);
        printf("%d\n",A->data);
    }
    
}