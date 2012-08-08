#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct part
{
	char name[30];
	int number; 
	float cost;

};

int main(void)
{
	struct part staticStruct;
	struct part* dynamicStruct = malloc(sizeof(struct part));

	strcpy(staticStruct.name, "Bob");
	staticStruct.number = 3;
	staticStruct.cost = 5.03;

	strcpy(dynamicStruct->name, "Joe");
	dynamicStruct->number = 5;
	dynamicStruct->cost = 2.87;

	printf("static name is %s\n", staticStruct.name);
	printf("static number is %d\n", staticStruct.number);
	printf("static cost is %f\n", staticStruct.cost);

	printf("dynamic name is %s\n", dynamicStruct->name);
	printf("dynamic number is %d\n", dynamicStruct->number);
	printf("dynamic cost is %f\n", dynamicStruct->cost);

	free(dynamicStruct);
	return 0;
}
