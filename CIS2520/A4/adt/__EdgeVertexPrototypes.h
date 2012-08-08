#ifndef __EDGE_VERTEX_PROTOTYPES_H
#define __EDGE_VERTEX_PROTOTYPES_H
#include <stdbool.h>
#include "List.h"

typedef struct Vertex_t
{
	int index;
	char* name;
	List* edges;
} Vertex;

typedef struct Edge_t
{
	Vertex* start;
	Vertex* end;
	int weight;
} Edge;

#endif
