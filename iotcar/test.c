#include "wheel.h"
GPIO *test1,*test2;

int main(int argc,char* argv[]){
	
	printf("%d\n",sizeof(GPIO));
	test1=gpio_initial(2,"in");
	test2=gpio_initial(3,"out");
	printf("%d\n",sizeof(*test1));
	test2->data=1;
	printf("%d\t%d\n",test1->data,test1->pinNumber);
	gpio_output(test2);
	gpio_state(test1);
	printf("%d\n",gpio_state(test1));
	
	return 0;
}
