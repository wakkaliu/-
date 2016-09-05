#include "gpio_mod_ver2.h"
#include "time.h"

#define DETECT_SIGNAL 1
#define OFF_SIGNAL 0

int decide_time(GPIO *A);
int det_base(GPIO *A);
int det_D(GPIO *A, float base_t, int *r);
int dec_num(float sp_time, float base);