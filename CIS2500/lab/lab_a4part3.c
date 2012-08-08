#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct player
{
	char name[32];
	float x, y;
	int hp, hpMax;
};

struct player* AllocatePlayer()
{
	struct player* p = malloc(sizeof(struct player));
	strcpy(p->name, "Jim");
	p->x = 16;
	p->y = 56.3;
	p->hp = p->hpMax = 304;
	return p;
}

int main (void)
{
	struct player* p = AllocatePlayer();

	printf("Name: %s\n", p->name);
	printf("Location: (%f, %f)\n", p->x, p->y);
	printf("HP: %d / %d\n", p->hp, p->hpMax);

	free(p);

	return 0;
}
