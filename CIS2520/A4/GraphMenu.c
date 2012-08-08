/*
	This is a test program for the graph menu. 
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "adt/Graph.h"
#include "adt/String.h"
#include "adt/Vertex.h"
#include "adt/Edge.h"
#include "adt/List.h"

#define BUFFER_SIZE 81

int main()
{
	Graph* testGraph;
	Graph* loadedGraph;
	int opt;
	char buffer[BUFFER_SIZE], buffer2[BUFFER_SIZE];
	char* name;
	char* name2;
	int weight;
	int i, j;
	FILE* f;
	List* path;
	AdjacencyMatrix* matrix;

	printf("Is this a directed graph (0 = no, 1 = yes)? ");
	fgets(buffer, BUFFER_SIZE, stdin);
	sscanf(buffer, "%d", &opt);

	if (!(testGraph = GraphNew(opt)))
	{
		exit(-1);
	}

	while(1)
	{
		printf("\n\n\nGraph (<ctrl-C> to quit):\n");
		printf("1 - GraphInsertVertex()\n");
		printf("2 - GraphRemoveVertex()\n");
		printf("3 - GraphInsertEdge()\n");
		printf("4 - GraphRemoveEdge()\n");
		printf("5 - GraphRead()\n");
		printf("6 - GraphWrite()\n");
		printf("7 - GraphToAdjacencyMatrix()\n");
		printf("8 - GraphClearEdges()\n");
		printf("9 - GraphFindMinimumSpanningTree()\n");
		printf("10 - GraphFindTransitiveClosure()\n");
		printf("11 - GraphFindShortestPaths()\n");

		printf("\n");
		printf("CHOICE: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		sscanf(buffer, "%d", &opt);

		switch(opt)
		{
			case 1:
				printf("Name of vertex to insert: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				/* Force the string to be one word long. */
				sscanf(buffer, "%s", buffer2);
				name = StringNew(buffer2);
				if(!GraphInsertVertex(testGraph, name))
				{
					printf("A vertex named '%s' is invalid, or it already exists in the graph!\n", name);
					StringFree(name);
				}
				else
				{
					printf("Added the vertex '%s'.\n", name);
				}
				break;
			case 2:
				printf("Name of vertex to remove: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				name = StringNew(buffer);
				name[strlen(name) - 1] = '\0';
				if(!GraphRemoveVertex(testGraph, name))
				{
					printf("No vertex named '%s' found!\n", name);
				}
				else
				{
					printf("Removed '%s'\n", name);
				}
				StringFree(name);
				break;
			case 3:
				printf("Name of start vertex of edge to insert: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				name = StringNew(buffer);
				name[strlen(name) - 1] = '\0';
				printf("Name of end vertex of edge to insert: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				name2 = StringNew(buffer);
				name2[strlen(name2) - 1] = '\0';
				printf("Numeric weight of directed edge: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				sscanf(buffer, "%d", &weight);
				if(!GraphInsertEdge(testGraph, name, name2, weight))
				{
					printf("An edge between '%s' and '%s' already exists.\n", name, name2);
				}
				else
				{
					printf("Created new edge between '%s' and '%s'\n", name, name2);
				}
				StringFree(name);
				StringFree(name2);
				break;
			case 4:
				printf("Name of start vertex of edge to remove: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				name = StringNew(buffer);
				name[strlen(name) - 1] = '\0';
				printf("Name of end vertex of edge to remove: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				name2 = StringNew(buffer);
				name2[strlen(name2) - 1] = '\0';
				if(!GraphRemoveEdge(testGraph, name, name2))
				{
					printf("An edge between '%s' and '%s' does not exist. Thus it cannot be removed.\n", name, name2);
				}
				else
				{
					printf("Removed edge between '%s' and '%s'\n", name, name2);
				}
				StringFree(name);
				StringFree(name2);
				break;
			case 5:
				printf("File to read from: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				buffer[strlen(buffer) - 1] = '\0';
				f = fopen(buffer, "r");
				if(f)
				{
					loadedGraph = GraphRead(f);
					if(loadedGraph)
					{
						testGraph = loadedGraph;
					}
					fclose(f);
				}
				else
				{
					printf("Can't read file '%s'!", buffer);
				}
				break;
			case 6:
				printf("File to save to: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				buffer[strlen(buffer) - 1] = '\0';
				f = fopen(buffer, "w");
				GraphWrite(testGraph, f);
				fclose(f);
				printf("Saved graph to '%s'.\n", buffer);
				break;
			case 7:
				matrix = GraphToAdjacencyMatrix(testGraph);
				for(i = 0; i < AdjacencyMatrixGetSize(matrix); i++)
				{
					for(j = 0; j < AdjacencyMatrixGetSize(matrix); j++)
					{
						printf("%d\t", AdjacencyMatrixGetEdge(matrix, i, j));
					}
					printf("\n");
				}
				AdjacencyMatrixFree(matrix);
				break;
			case 8:
				GraphClearEdges(testGraph);
				printf("Removed all edges between the vertices from the graph\n");
				break;
			case 9:
				if(!GraphFindMinimumSpanningTree(testGraph))
				{
					printf("Error: Cannot get the minimum spanning tree of a directed graph.\n");
				}
				break;
			case 10:
				if(!GraphFindTransitiveClosure(testGraph))
				{
					printf("Error: Cannot get the transitive closure of an undirected graph.\n");
				}
				break;
			case 11:
				printf("Name of source vertex: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				name = StringNew(buffer);
				name[strlen(name) - 1] = '\0';
				printf("Name of dest vertex: ");
				fgets(buffer, BUFFER_SIZE, stdin);
				name2 = StringNew(buffer);
				name2[strlen(name2) - 1] = '\0';

				path = GraphFindShortestPath(testGraph, name, name2);
				if(path && listHead(path))
				{
					printf("Path found:\n");
					do
					{
						printf("%s\n", VertexGetName((Vertex*) listGetCurrent(path)) );
					} while(listNext(path));
					printf("\n");
					listDelete(path);
				}
				else
				{
					printf("No path found between '%s' and '%s'! (make sure you spelled the nodes correctly, and the graph is connected)\n", name, name2);
				}

				StringFree(name);
				StringFree(name2);
				break;
			default:
				printf("\n>>> ILLEGAL SELECTION\n");
				break;
		}
		GraphWrite(testGraph, stdout);
		printf("\nHit <enter>...\n");
		fgets(buffer, BUFFER_SIZE, stdin);
	}

	GraphFree(testGraph);

}
