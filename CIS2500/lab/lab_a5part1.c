#include <stdio.h>

int main()
{
	char buffer[4096];
	float a, b, c;

	fgets(buffer, 4096, stdin);
	sscanf(buffer, "%f %f %f", &a, &b, &c);
	sprintf(buffer, "%f %f %f", a, b, c);
	printf("%s\n", buffer);

	return 0;
}
