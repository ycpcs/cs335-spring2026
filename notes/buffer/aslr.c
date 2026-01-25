#include <stdio.h>
#include <stdlib.h>

void main()
{
	char x[12];
	char *y =  (char *)malloc(12 * sizeof(char));

	printf("Address of x variable on stack: 0x%x\n", x);
	printf("Address of y variable on heap: 0x%x\n", y);
}

// To turn OFF address randomization: sudo sysctl -w kernel.randomize_va_space=0
// To turn ON address randomization: sudo sysctl -w kernel.randomize_va_space=2