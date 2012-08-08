#include "Vertex.h"

Vertex* VertexNew(char* name)
{
	Vertex* this = malloc(sizeof(Vertex));
	if(this != NULL)
	{
		this->name = name;
		this->edges = listCreate();
	}
	return this;
}

void VertexFree(Vertex* this)
{
	if(this != NULL)
	{
		VertexClearEdges(this);
		listDelete(this->edges);
		StringFree(this->name);
		free(this);
	}
}

void VertexClearEdges(Vertex* this)
{
	Edge* current;
	if(!listIsEmpty(this->edges))
	{
		listHead(this->edges);
		current = listGetCurrent(this->edges);
		while(current != NULL)
		{
			listDelCurrent(this->edges);
			EdgeFree(current);
			current = listGetCurrent(this->edges);
		}
	}	
}

Vertex* VertexInsertEdge(Vertex* this, Vertex* end, int weight)
{
	List* iterator;
	Edge* current;

	if(this != NULL)
	{
		iterator = listHead(this->edges);
		current = listGetCurrent(this->edges);
		while (iterator != NULL)
		{
			if(EdgeGetStart(current) == this
				&& EdgeGetEnd(current) == end)
			{
				return NULL;
			}
			iterator = listNext(this->edges);
			current = listGetCurrent(this->edges);
		}
		current = EdgeNew(this, end, weight);
		listTail(this->edges);
		listAddAfter(this->edges, current);
		return this;
	}
	return NULL;
}

Vertex* VertexRemoveEdge(Vertex* this, Vertex* end)
{
	List* iterator;
	Edge* current;

	if(this != NULL)
	{
		iterator = listHead(this->edges);
		current = listGetCurrent(this->edges);
		while (iterator != NULL)
		{
			if(EdgeGetStart(current) == this
				&& EdgeGetEnd(current) == end)
			{
				listDelCurrent(this->edges);
				EdgeFree(current);
				return this;
			}
			iterator = listNext(this->edges);
			current = listGetCurrent(this->edges);
		}
	}
	return NULL;
}

char* VertexGetName(Vertex* this)
{
	return this->name;
}

void VertexSetName(Vertex* this, char* name)
{
	StringFree(this->name);
	this->name = name;
}

int VertexGetIndex(Vertex* this)
{
	return this->index;
}

void VertexSetIndex(Vertex* this, int index)
{
	this->index = index;
}

void VertexWrite(Vertex* this, FILE* f)
{
	List* iterator;
	Edge* current;
	int index;

	if(this != NULL)
	{
		fprintf(f, "%d %s ", this->index, this->name);
		iterator = listHead(this->edges);
		current = listGetCurrent(this->edges);
		while (iterator != NULL)
		{
			EdgeWrite(current, f);
			iterator = listNext(this->edges);
			current = listGetCurrent(this->edges);
			index++;
		}
	}
	fprintf(f, "\n");
}

List* VertexGetEdges(Vertex* this)
{
	return this->edges;
}
