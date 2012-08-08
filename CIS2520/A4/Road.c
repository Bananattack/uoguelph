#include <stdlib.h>
#include <stdio.h>
#include "adt/Graph.h"

#define BUFFER_SIZE 81

int main()
{
	Graph* roadGraph;
	FILE* f;
	char buffer[BUFFER_SIZE];

	printf("Please enter the file to read from: ");
	fgets(buffer, BUFFER_SIZE, stdin);
	buffer[strlen(buffer) - 1] = '\0';
	f = fopen(buffer, "r");
	if(f)
	{
		roadGraph = GraphRead(f);
		if(!roadGraph)
		{
			printf("Fatal error: Problem occurred when reading '%s'!\n", buffer);
			return -1;
		}
		if(!GraphFindMinimumSpanningTree(roadGraph))
		{
			printf("Error: Road graph must be undirected! Minimum spanning tree cannot be found!\n");
		}
		GraphWrite(roadGraph, stdout);
		GraphFree(roadGraph);
	}
	else
	{
		printf("Fatal error: Can't read file '%s'!\n", buffer);
	}
	fclose(f);
	return 0;
}

