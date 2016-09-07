#include "gpio_mod_ver2.h"
#include "time.h"

#define DETECT_SIGNAL 1
#define OFF_SIGNAL 0

char detect_fun(GPIO *A, GPIO *B);
char check_position(int *r);