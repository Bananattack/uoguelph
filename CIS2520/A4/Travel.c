#include <stdlib.h>
#include <stdio.h>
#include "adt/Graph.h"
#include "adt/List.h"
#include "adt/String.h"

#define BUFFER_SIZE 81

int main()
{
	Graph* travelGraph;
	FILE* f;
	char buffer[BUFFER_SIZE];
	char* name;
	char* name2;
	List* path;

	printf("Please enter the file to read from: ");
	fgets(buffer, BUFFER_SIZE, stdin);
	buffer[strlen(buffer) - 1] = '\0';
	f = fopen(buffer, "r");
	if(f)
	{
		travelGraph = GraphRead(f);
		if(!travelGraph)
		{
			printf("Fatal error: Problem occurred when reading '%s'!\n", buffer);
			return -1;
		}

		GraphWrite(travelGraph, stdout);

		printf("Name of source airport: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		name = StringNew(buffer);
		name[strlen(name) - 1] = '\0';
		printf("Name of destination airport: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		name2 = StringNew(buffer);
		name2[strlen(name2) - 1] = '\0';

		path = GraphFindShortestPath(travelGraph, name, name2);
		if(path && listHead(path))
		{
			printf("Route found:\n");
			do
			{
				printf("%s\n", VertexGetName((Vertex*) listGetCurrent(path)) );
			} while(listNext(path));
			printf("\n");
			listDelete(path);
		}
		else
		{
			printf("No route found between '%s' and '%s'! (make sure you spelled the airports correctly, and the airports are connected in both directions (if directed graph file))\n", name, name2);
		}

		StringFree(name);
		StringFree(name2);

		GraphFree(travelGraph);
	}
	else
	{
		printf("Fatal error: Can't read file '%s'!\n", buffer);
	}
	fclose(f);
	return 0;
}

