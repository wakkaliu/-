#include "signal_detect_double.h"

int main(){
    char result = NULL;
    GPIO *A, *B;

    A=gpio_initial(3,"in");
    B=gpio_initial(2,"in");

    result = detect_fun(A, B);

    printf("\n current position is %c\n ", result);

    return 0;
}