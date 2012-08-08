#ifndef VERTEX_H
#define VERTEX_H

#include <stdio.h>
#include <stdlib.h>
#include "String.h"
#include "List.h"
#include "__EdgeVertexPrototypes.h"
#include "Edge.h"

Vertex* VertexNew(char* name);
void VertexMarkAllEdgesUnvisited(Vertex* this);
void VertexFree(Vertex* this);
void VertexClearEdges(Vertex* this);
Vertex* VertexInsertEdge(Vertex* this, Vertex* end, int weight);
Vertex* VertexRemoveEdge(Vertex* this, Vertex* end);
char* VertexGetName(Vertex* this);
void VertexSetName(Vertex* this, char* name);
int VertexGetIndex(Vertex* this);
void VertexSetIndex(Vertex* this, int index);
void VertexWrite(Vertex* this, FILE* f);
List* VertexGetEdges(Vertex* this);

#endif
