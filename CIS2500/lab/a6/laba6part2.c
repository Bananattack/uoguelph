#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int count;
	double total;
} data;

int main()
{
	data bleh;
	FILE *f;
	int i;

	f = fopen("bleh", "w+");
	for (i = 0; i < 10; i++)
	{
		bleh.count = i;
		bleh.total = i * 0.5;
		fwrite(&bleh, sizeof(data), 1, f);
	}

	fseek(f, sizeof(data) * 2, SEEK_SET);
	printf("Position = %d\n", (int) ftell(f));
	fread(&bleh, sizeof(data), 1, f);
	printf("Structure = %d %f\n", bleh.count, bleh.total);

	fseek(f, sizeof(data) * 6, SEEK_SET);
	printf("Position = %d\n", (int) ftell(f));
	fread(&bleh, sizeof(data), 1, f);
	printf("Structure = %d %f\n", bleh.count, bleh.total);

	fclose(f);
	return 0;
}
