#ifndef EDGE_H
#define EDGE_H

#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "__EdgeVertexPrototypes.h"
#include "Vertex.h"

Edge* EdgeNew(Vertex* start, Vertex* end, int weight);
void EdgeFree(Edge* this);
Vertex* EdgeGetStart(Edge* this);
Vertex* EdgeGetEnd(Edge* this);
int EdgeGetWeight(Edge* this);
void EdgeWrite(Edge* this, FILE* f);

#endif
