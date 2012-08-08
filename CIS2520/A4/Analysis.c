#include <stdlib.h>
#include <stdio.h>
#include "adt/Graph.h"

#define BUFFER_SIZE 81

int main()
{
	Graph* analysisGraph;
	FILE* f;
	char buffer[BUFFER_SIZE];

	printf("Please enter the file to read from: ");
	fgets(buffer, BUFFER_SIZE, stdin);
	buffer[strlen(buffer) - 1] = '\0';
	f = fopen(buffer, "r");
	if(f)
	{
		analysisGraph = GraphRead(f);
		if(!analysisGraph)
		{
			printf("Fatal error: Problem occurred when reading '%s'!\n", buffer);
			return -1;
		}
		if(!GraphFindTransitiveClosure(analysisGraph))
		{
			printf("Error: Graph is not directed! Transitive closure cannot be found.\n");
		}
		GraphWrite(analysisGraph, stdout);
		GraphFree(analysisGraph);
	}
	else
	{
		printf("Fatal error: Can't read file '%s'!\n", buffer);
	}
	fclose(f);
	return 0;
}

