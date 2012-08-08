#include <stdio.h>
#include <stdlib.h>
#include "simRand.h"

int main(int argc, char** argv)
{
	int i;
	rand_stream stream = simSeed(time(NULL));
	for(i = 0; i < 50000; i++)
	{
		printf("%d\n", simRand(&stream));
	}
	return 0;
}