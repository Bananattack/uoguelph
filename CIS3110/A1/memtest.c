#include <stdio.h>
#include "memory.h"

int main(void)
{
	int accessible, read_only;
	memory(&accessible, &read_only);
	printf("%d are accessible from the program, and %d of which are readonly.", accessible, read_only);
}
