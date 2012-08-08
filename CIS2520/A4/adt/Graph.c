#include "Graph.h"

Graph* GraphNew(bool directed)
{
	Graph* this = malloc(sizeof(Graph));
	if(this != NULL)
	{
		this->vertices = listCreate();
		this->directed = directed;
	}
	return this;
}

void GraphFree(Graph* this)
{
	Vertex* current;
	if(this != NULL)
	{
		if(!listIsEmpty(this->vertices))
		{
			listHead(this->vertices);
			current = listGetCurrent(this->vertices);
			while(current != NULL)
			{
				listDelCurrent(this->vertices);
				VertexFree(current);
				current = listGetCurrent(this->vertices);
			}
		}
		listDelete(this->vertices);
		free(this);
	}
}

Graph* GraphInsertVertex(Graph* this, char* name)
{
	Vertex* vertex;
	if(this != NULL)
	{
		if(_GraphGetVertexByName(this, name) != NULL || !strlen(name))
		{
			return NULL;
		}
		vertex = VertexNew(name);
		listTail(this->vertices);
		listAddAfter(this->vertices, vertex);
		return this;
	}
	return NULL;
}

Graph* GraphRemoveVertex(Graph* this, char* name)
{
	List* iterator;
	Vertex* current;
	Vertex* vertexToRemove = NULL;

	if(this != NULL)
	{
		iterator = listHead(this->vertices);
		current = listGetCurrent(this->vertices);
		while (iterator != NULL)
		{
			if(strcmp(name, VertexGetName(current)) == 0)
			{
				listDelCurrent(this->vertices);
				vertexToRemove = current;
				break;
			}
			iterator = listNext(this->vertices);
			current = listGetCurrent(this->vertices);
		}
		if(vertexToRemove != NULL)
		{
			iterator = listHead(this->vertices);
			current = listGetCurrent(this->vertices);
			while(iterator != NULL)
			{
				VertexRemoveEdge(current, vertexToRemove);
				iterator = listNext(this->vertices);
				current = listGetCurrent(this->vertices);
			}
			VertexFree(vertexToRemove);
			return this;
		}
	}
	return NULL;
}

Graph* GraphInsertEdge(Graph* this, char* startName, char* endName, int weight)
{
	Vertex* start;
	Vertex* end;
	Vertex* result = NULL;
	Vertex* result2 = NULL;

	if(this != NULL)
	{
		start = _GraphGetVertexByName(this, startName);
		end = _GraphGetVertexByName(this, endName);
		if(start != NULL && end != NULL)
		{
			result = VertexInsertEdge(start, end, weight);
			if(!this->directed)
			{
				result2 = VertexInsertEdge(end, start, weight);
			}
			/*
				If it added one of the edges successfully, return this.
				Otherwise, NULL, because an edge must already exist.
			*/
			return (result || result2) ? this : NULL;
		}
	}
	return NULL;
}

Graph* GraphRemoveEdge(Graph* this, char* startName, char* endName)
{
	Vertex* start;
	Vertex* end;

	if(this != NULL)
	{
		start = _GraphGetVertexByName(this, startName);
		end = _GraphGetVertexByName(this, endName);
		if(start != NULL && end != NULL)
		{
			VertexRemoveEdge(start, end);
			if(!this->directed)
			{
				VertexRemoveEdge(end, start);
			}
		}
	}
	return NULL;
}

int GraphGetVertexCount(Graph* this)
{
	if(this != NULL)
	{
		return listLength(this->vertices);
	}
	return 0;
}

Vertex* _GraphGetVertexByName(Graph* this, char* name)
{
	List* iterator;
	Vertex* current;
	if(this != NULL)
	{
		iterator = listHead(this->vertices);
		current = listGetCurrent(this->vertices);
		while (iterator != NULL)
		{
			if(strcmp(name, VertexGetName(current)) == 0)
			{
				return current;
			}
			iterator = listNext(this->vertices);
			current = listGetCurrent(this->vertices);
		}
	}
	return NULL;
}

Vertex* _GraphGetVertexByIndex(Graph* this, int index)
{	
	_GraphRenumberVertices(this);
	if(this != NULL)
	{
		listMoveToNth(this->vertices, index + 1);
		return listGetCurrent(this->vertices);
	}
	return NULL;
}

void _GraphRenumberVertices(Graph* this)
{
	List* iterator;
	Vertex* current;
	int index = 0;

	if(this != NULL)
	{
		iterator = listHead(this->vertices);
		current = listGetCurrent(this->vertices);
		while (iterator != NULL)
		{
			VertexSetIndex(current, index);
			iterator = listNext(this->vertices);
			current = listGetCurrent(this->vertices);
			index++;
		}
	}
}

void GraphWrite(Graph* this, FILE* f)
{
	List* iterator;
	Vertex* current;

	_GraphRenumberVertices(this);
	if(this != NULL)
	{
		fprintf(f, "%d %d\n", GraphGetVertexCount(this), this->directed);
		iterator = listHead(this->vertices);
		current = listGetCurrent(this->vertices);
		while (iterator != NULL)
		{
			VertexWrite(current, f);
			iterator = listNext(this->vertices);
			current = listGetCurrent(this->vertices);
		}
	}
}

Graph* GraphRead(FILE* f)
{
	int i, vertices, directed, startIndex, endIndex, weight;
	char buffer[4096];
	char* word;
	Graph* this;
	Vertex* startVertex;
	Vertex* endVertex;

	fgets(buffer, 4096, f);
	/* Number of Vertices */
	word = strtok(buffer, DELIMITERS);
	if (!word || sscanf(word, "%d", &vertices) == 0)
	{
		printf("Bad vertex count '%s' supplied! Must be an int.\n", word);
		return NULL;
	}
	/* Optional: Directed or undirected? */
	word = strtok(NULL, DELIMITERS);
	if(word && strlen(word))
	{
		if(sscanf(word, "%d", &directed) == 0)
		{
			printf("Bad directed/undirected indicator '%s' supplied! Must be an int.\n", word);
			return NULL;
		}
	}
	else
	{
		directed = 0;
	}

	/* Instantiate the graph. */
	this = GraphNew(directed);
	if(!this)
	{
		printf("Couldn't allocate the memory for the graph. What?\n");
		return NULL;
	}
	/* Allocate the vertex list space */
	for(i = 0; i < vertices; i++)
	{
		listAddAfter(this->vertices, VertexNew(StringNew("")));
	}
	/* Initialize vertex data */
	while(fgets(buffer, 4096, f) != NULL)
	{
		word = strtok(buffer, DELIMITERS);
		/* TODO: INDEX OF VERTEX TO INITIALIZE */
		if(!word)
		{
			continue;
		}
		else if(sscanf(word, "%d", &startIndex) == 0 || startIndex < 0 || startIndex >= vertices)
		{
			printf("Bad vertex index '%s' supplied! Must be an int between 0..%d\n", word, vertices);
			GraphFree(this);
			return NULL;
		}

		startVertex = _GraphGetVertexByIndex(this, startIndex);
		word = strtok(NULL, DELIMITERS);
		if(!word)
		{
			printf("Vertex %d is missing a name.\n", startIndex);
			GraphFree(this);
			return NULL;
		}
		VertexSetName(startVertex, StringNew(word));
		word = strtok(NULL, DELIMITERS);
		i = 0;
		while(word && strlen(word))
		{
			if(sscanf(word, "%d", &endIndex) == 0 || endIndex < 0 || endIndex >= vertices)
			{
				printf("Bad edge destination vertex index '%s' supplied! Must be an int between 0..%d\n", word, vertices);
				GraphFree(this);
				return NULL;
			}
			endVertex = _GraphGetVertexByIndex(this, endIndex);
			word = strtok(NULL, DELIMITERS);
			if(!word || sscanf(word, "%d", &weight) == 0)
			{
				printf("Bad edge weight '%s' supplied! Must be an int.\n", word);
				GraphFree(this);
				return NULL;
			}

			VertexInsertEdge(startVertex, endVertex, weight);
			if(!this->directed)
			{
				VertexInsertEdge(endVertex, startVertex, weight);
			}
			word = strtok(NULL, DELIMITERS);
		}
	}

	return this;
}

void GraphClearEdges(Graph* this)
{
	List* iterator;
	Vertex* current;
	if(this != NULL)
	{
		iterator = listHead(this->vertices);
		current = listGetCurrent(this->vertices);
		while (iterator != NULL)
		{
			VertexClearEdges(current);
			iterator = listNext(this->vertices);
			current = listGetCurrent(this->vertices);
		}
	}
}

AdjacencyMatrix* GraphToAdjacencyMatrix(Graph* this)
{
	int i, j;
	int size = GraphGetVertexCount(this);
	List* edges;
	Edge* edge;
	AdjacencyMatrix* adj;

	_GraphRenumberVertices(this);

	adj = malloc(sizeof(AdjacencyMatrix));
	adj->size = size;
	adj->directed = this->directed;
	adj->matrix = malloc(sizeof(int) * size);
	for(i = 0; i < size; i++)
	{
		adj->matrix[i] = malloc(sizeof(int) * size);
		for(j = 0; j < size; j++)
		{
			adj->matrix[i][j] = 0;
		}
	}
	
	if(listHead(this->vertices))
	{
		do
		{
			edges = VertexGetEdges(listGetCurrent(this->vertices));
			if(listHead(edges))
			{
				do
				{
					edge = listGetCurrent(edges);
					adj->matrix[VertexGetIndex(EdgeGetStart(edge))][VertexGetIndex(EdgeGetEnd(edge))] = EdgeGetWeight(edge);
				}	
				while(listNext(edges));
			}
		} while(listNext(this->vertices));
	}

	return adj;
}

void GraphFromAdjacencyMatrix(Graph* this, AdjacencyMatrix* adj)
{
	int i, j;
	Vertex* u;
	Vertex* v;

	GraphClearEdges(this);
	for(i = 0; i < adj->size; i++)
	{
		for(j = 0; j < adj->size; j++)
		{
			if(adj->matrix[i][j])
			{
				u = _GraphGetVertexByIndex(this, i);
				v = _GraphGetVertexByIndex(this, j);
				if(u && v)
				{
					GraphInsertEdge(this, VertexGetName(u), VertexGetName(v), adj->matrix[i][j]);
				}
			}
		}
	}
}

void AdjacencyMatrixFree(AdjacencyMatrix* this)
{
	int i;
	for(i = 0; i < this->size; i++)
	{
		free(this->matrix[i]);
	}
	free(this->matrix);
	free(this);
}

int AdjacencyMatrixGetSize(AdjacencyMatrix* this)
{
	return this->size;
}

AdjacencyMatrix* AdjacencyMatrixClearEdges(AdjacencyMatrix* this)
{
	int i, j;
	for(i = 0; i < this->size; i++)
	{
		for(j = 0; j < this->size; j++)
		{
			this->matrix[i][j] = 0;
		}
	}
	return this;
}

int AdjacencyMatrixGetEdge(AdjacencyMatrix* this, int y, int x)
{
	return this->matrix[y][x];
}

int AdjacencyMatrixGetEdgeCount(AdjacencyMatrix* this)
{
	int i, j;
	int count = 0;
	for(i = 0; i < this->size; i++)
	{
		for(j = 0; j < this->size; j++)
		{
			if(this->matrix[i][j])
			{
				count++;
			}
		}
	}
	if(!this->directed)
	{
		return count / 2;
	}
	return count;
}

void AdjacencyMatrixSetEdge(AdjacencyMatrix* this, int y, int x, int weight)
{
	this->matrix[y][x] = weight;
	if(!this->directed)
	{
		this->matrix[x][y] = weight;
	}
}

void AdjacencyMatrixGetMinimumEdge(AdjacencyMatrix* this, int* y, int* x)
{
	int i, j;

	*y = 0;
	*x = 0;

	for(i = 0; i < this->size; i++)
	{
		for(j = 0; j < this->size; j++)
		{
			/* Find the first weighted edge on first pass, then find the minimal edge weight in the graph */
			if((this->matrix[*y][*x] == 0 || this->matrix[i][j] < this->matrix[*y][*x]) && this->matrix[i][j] != 0)
			{
				*y = i;
				*x = j;
			}
		}
	}
}

Graph* GraphFindMinimumSpanningTree(Graph* this)
{
	int y, x;
	int i;
	int size = GraphGetVertexCount(this);
	List* components;
	List* componentsRequiringDeletion;
	List* current;
	AdjacencyMatrix* remainingEdges;
	AdjacencyMatrix* spanTree;
	List* u;
	List* v;
	int edgeCount = 0;

	/* Must be undirected. */
	if(!this || this->directed)
	{
		return NULL;
	}

	/* Convert graph into matrix to make minimum edge search simpler*/
	remainingEdges = GraphToAdjacencyMatrix(this);

	/*
		Initialize component list C
		for all v in V
			C(v) := v
	*/
	components = listCreate();
	componentsRequiringDeletion = listCreate();
	listHead(this->vertices);
	for(i = 0; i < size; i++)
	{
		listTail(components);
		current = listCreate();
		listAddAfter(current, listGetCurrent(this->vertices));
		listAddAfter(components, current);
		listAddAfter(componentsRequiringDeletion, current);
		listNext(this->vertices);
	}

	/* initialize T (edge set) empty */
	spanTree = AdjacencyMatrixClearEdges(GraphToAdjacencyMatrix(this));
	
	/* while |T| < (n-1) */
	while(edgeCount < size - 1)
	{
		/* choose minimum (y,x) remain */
		AdjacencyMatrixGetMinimumEdge(remainingEdges, &y, &x);
		listMoveToNth(components, y + 1);
		u = listGetCurrent(components);
		listMoveToNth(components, x + 1);
		v = listGetCurrent(components);
		/* if (C(y) != C(x)) */
		if(u != v)
		{
		        /* add (y,x) to T */
			AdjacencyMatrixSetEdge(spanTree, y, x, AdjacencyMatrixGetEdge(remainingEdges, y, x));

			/* merge C(y) and C(x)*/
			/* For all I in C(x), append I to C(y) */
			listHead(v);
			do
			{
				listTail(u);
				listAddAfter(u, listGetCurrent(v));
			} while(listNext(v));

			/* For all I in C(x), C(x) := C(y) */
			listHead(components);
			do
			{
				if(listGetCurrent(components) == v)
				{
					listSetCurrent(components, u);
				}
			} while(listNext(components));
			/* Done merge */

			u = listGetCurrent(components);
			listMoveToNth(components, x + 1);
			v = listGetCurrent(components);
		}
		AdjacencyMatrixSetEdge(remainingEdges, y, x, 0);
		edgeCount++;
	}

	/* Cleanup remaining component lists */
	listHead(componentsRequiringDeletion);
	do
	{
		current = listGetCurrent(componentsRequiringDeletion);
		listDelete(current);
		listDelCurrent(componentsRequiringDeletion);
	} while(current != NULL);
	listDelete(componentsRequiringDeletion);
	listDelete(components);
	GraphFromAdjacencyMatrix(this, spanTree);
	AdjacencyMatrixFree(spanTree);
	AdjacencyMatrixFree(remainingEdges);

	return this;
}

Graph* GraphFindTransitiveClosure(Graph* this)
{
	int i, j, k;
	AdjacencyMatrix* adj;

	/* Must be directed. */
	if(!this || !this->directed)
	{
		return NULL;
	}

	/* Convert graph into matrix to make transitive closure search simpler*/
	adj = GraphToAdjacencyMatrix(this);
	
	for(k = 0; k < adj->size; k++)
	{
		for(i = 0; i < adj->size; i++)
		{
			for(j = 0; j < adj->size; j++)
			{
				if(AdjacencyMatrixGetEdge(adj, i, k) && AdjacencyMatrixGetEdge(adj, k, j))
				{
					AdjacencyMatrixSetEdge(adj, i, j, 1);
				}
			}
		}
	}
	GraphFromAdjacencyMatrix(this, adj);
	AdjacencyMatrixFree(adj);

	return this;
}

Vertex** _GraphDijkstra(Graph* this, Vertex* src, Vertex* dest)
{
	int i;
	int size = GraphGetVertexCount(this);
	int* distances;
	Vertex** moves;
	int vertexIndex;
	int smallest;
	int smallestDistanceIndex, edgeEndIndex;
	Vertex* vertexSmallest = NULL;
	List* remaining;
	List* edgesSmallest;
	Edge* edge;

	/* No bad vertex references, thanks! */
	if(this == NULL || src == NULL || dest == NULL)
	{
		return NULL;
	}
	_GraphRenumberVertices(this);

	distances = malloc(sizeof(int) * size);
	moves = malloc(sizeof(Vertex*) * size);
	vertexIndex = VertexGetIndex(src);
	/* D[v] := 0 */
	distances[vertexIndex] = 0;
	/*
	for all u in V such that u != v
		D[v] := MAX
	*/
	for (i = 0; i < size; i++)
	{
		if(i != vertexIndex)
		{
			distances[i] = 99999;
		}
		moves[i] = NULL;
	}

	/* Q := V */
	remaining = listCreate();
	if(listHead(this->vertices))
	{
		do
		{
			listTail(remaining);
			listAddAfter(remaining, listGetCurrent(this->vertices));
		} while(listNext(this->vertices));
	}

	/*while Q not empty */
	while(!listIsEmpty(remaining))
	{
		/* u := remove q in Q, min D[q] */
		listHead(remaining);
		vertexSmallest = listGetCurrent(remaining);
		smallestDistanceIndex = VertexGetIndex(vertexSmallest);
		smallest = i = 0;
		do
		{
			if(distances[VertexGetIndex(listGetCurrent(remaining))] < distances[smallestDistanceIndex])
			{
				smallest = i;
				vertexSmallest = listGetCurrent(remaining);
				smallestDistanceIndex = VertexGetIndex(vertexSmallest);
			}
			i++;
		} while(listNext(remaining));
		listMoveToNth(remaining, smallest+1);
		listDelCurrent(remaining);
		/*for all z adjacent to u*/
		edgesSmallest = VertexGetEdges(vertexSmallest);
		if(listHead(edgesSmallest))
		{
			do
			{
				edge = listGetCurrent(edgesSmallest);
				edgeEndIndex = VertexGetIndex(EdgeGetEnd(edge));
				/* if (D[u] + w(u,z) < D[z]) */
				if(distances[smallestDistanceIndex] + EdgeGetWeight(edge) < distances[edgeEndIndex])
				{
					/* D[z] := D[u] + w(u,z) */
					distances[edgeEndIndex] = distances[smallestDistanceIndex] + EdgeGetWeight(edge);
					moves[edgeEndIndex] = vertexSmallest;
				}
	
			} while(listNext(edgesSmallest));
		}
	}
	listDelete(remaining);
	return moves;
}

List* GraphFindShortestPath(Graph* this, char* srcName, char* destName)
{
	List* path;
	Vertex* src;
	Vertex* dest;
	Vertex* vertex;
	Vertex** moves;

	src = _GraphGetVertexByName(this, srcName);
	dest = _GraphGetVertexByName(this, destName);
	if(this != NULL && src != NULL && dest != NULL)
	{
		path = listCreate();

		/* Follow the path until we are at the dest, then return the path. */
		moves = _GraphDijkstra(this, src, dest);
		if(moves == NULL)
		{
			listDelete(path);
			return NULL;
		}
		vertex = dest;
		if(moves[VertexGetIndex(vertex)])
		{
			while(moves[VertexGetIndex(vertex)])
			{
				listHead(path);
				listAddBefore(path, vertex);
				vertex = moves[VertexGetIndex(vertex)];
			}
			listHead(path);
			listAddBefore(path, src);
			free(moves);
			return path;
		}
		else
		{
			free(moves);
			listDelete(path);
		}
	}
	return NULL;
}
