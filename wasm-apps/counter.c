#include <stdio.h>
#include <stdlib.h>

void stupid_delay(void)
{
	volatile int i;
	for (i = 0; i < 100000; i++){}
}

int main(int argc, char **argv)
{
    int target = 10; //TODO: change this to use argv
    
    
    for (int i = 1; i <= target; i ++) {
    	printf("%d\n", i);
    	stupid_delay();
    }
    
    return 0;
}
