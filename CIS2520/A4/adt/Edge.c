#include "Edge.h"

Edge* EdgeNew(Vertex* start, Vertex* end, int weight)
{
	Edge* this = malloc(sizeof(Edge));
	if(this != NULL)
	{
		this->start = start;
		this->end = end;
		this->weight = weight;
	}
	return this;
}

void EdgeFree(Edge* this)
{
	if(this != NULL)
	{
		free(this);
	}
}

Vertex* EdgeGetStart(Edge* this)
{
	if(this != NULL)
	{
		return this->start;
	}
	return NULL;
}

Vertex* EdgeGetEnd(Edge* this)
{
	if(this != NULL)
	{
		return this->end;
	}
	return NULL;
}

int EdgeGetWeight(Edge* this)
{
	if(this != NULL)
	{
		return this->weight;
	}
	return 0;
}

void EdgeWrite(Edge* this, FILE* f)
{
	fprintf(f, "%d:%d ", VertexGetIndex(this->end), EdgeGetWeight(this));
}
