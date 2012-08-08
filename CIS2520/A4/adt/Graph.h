#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "Vertex.h"
#include "String.h"

#define DELIMITERS " :\n\t"
typedef struct Graph_t
{
	List* vertices;
	bool directed;
} Graph;

typedef struct AdjacencyMatrix_t
{
	int** matrix;
	bool directed;
	int size;
} AdjacencyMatrix;

Graph* GraphNew(bool directed);
void GraphFree(Graph* this);
Graph* GraphInsertVertex(Graph* this, char* name);
Graph* GraphRemoveVertex(Graph* this, char* name);
Graph* GraphInsertEdge(Graph* this, char* startName, char* endName, int weight);
Graph* GraphRemoveEdge(Graph* this, char* startName, char* endName);
int GraphGetVertexCount(Graph* this);
Vertex* _GraphGetVertexByName(Graph* this, char* name);
Vertex* _GraphGetVertexByIndex(Graph* this, int index);
void _GraphRenumberVertices(Graph* this);
void GraphWrite(Graph* this, FILE* f);
Graph* GraphRead(FILE* f);
void GraphClearEdges(Graph* this);
AdjacencyMatrix* GraphToAdjacencyMatrix(Graph* this);
void GraphFromAdjacencyMatrix(Graph* this, AdjacencyMatrix* adj);
void AdjacencyMatrixFree(AdjacencyMatrix* this);
int AdjacencyMatrixGetSize(AdjacencyMatrix* this);
int AdjacencyMatrixGetEdgeCount(AdjacencyMatrix* this);
AdjacencyMatrix* AdjacencyMatrixClearEdges(AdjacencyMatrix* this);
int AdjacencyMatrixGetEdge(AdjacencyMatrix* this, int y, int x);
void AdjacencyMatrixSetEdge(AdjacencyMatrix* this, int y, int x, int weight);
void AdjacencyMatrixGetMinimumEdge(AdjacencyMatrix* this, int* y, int* x);
Graph* GraphFindMinimumSpanningTree(Graph* this);
Graph* GraphFindTransitiveClosure(Graph* this);
Vertex** _GraphDijkstra(Graph* this, Vertex* src, Vertex* dest);
List* GraphFindShortestPath(Graph* this, char* srcName, char* destName);

#endif
