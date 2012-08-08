#ifndef ACTION_H
#define ACTION_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "adt/List.h"
#include "Bucket.h"

typedef enum ActionType_t
{
	POUR_A_TO_B,
	POUR_B_TO_A,
	FILL_A,
	FILL_B,
	EMPTY_A,
	EMPTY_B
} ActionType;

typedef struct Action_t
{
	int level_a, level_b;
	ActionType actionType;
} Action;

void ActionTypePrint(ActionType actionType, FILE* f);
void ActionRemember(List* actions, Bucket* a, Bucket* b, ActionType actionType);
void ActionUndo(List* actions, Bucket* a, Bucket* b);
bool ActionWasPerformed(List* actions, Bucket* a, Bucket* b, ActionType actionType);
void ActionPrintAll(List* actions, FILE* f);
void PourAtoB(List* actions, Bucket* a, Bucket* b);
void PourBtoA(List* actions, Bucket* a, Bucket* b);
void FillA(List* actions, Bucket* a, Bucket* b);
void FillB(List* actions, Bucket* a, Bucket* b);
void EmptyA(List* actions, Bucket* a, Bucket* b);
void EmptyB(List* actions, Bucket* a, Bucket* b);

#endif
