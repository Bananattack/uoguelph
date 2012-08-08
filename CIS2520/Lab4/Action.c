#include "Action.h"

void ActionTypePrint(ActionType actionType, FILE* f)
{
	switch(actionType)
	{
		case POUR_A_TO_B:
			fprintf(f, "Pour A into B\n");
			break;
		case POUR_B_TO_A:
			fprintf(f, "Pour B into A\n");
			break;
		case FILL_A:
			fprintf(f, "Fill A\n");
			break;
		case FILL_B:
			fprintf(f, "Fill B\n");
			break;
		case EMPTY_A:
			fprintf(f, "Empty A\n");
			break;
		case EMPTY_B:
			fprintf(f, "Empty B\n");
			break;
		default:
			fprintf(f, "Unknown action!\n");
			break;
	}
}

void ActionRemember(List* actions, Bucket* a, Bucket* b, ActionType actionType)
{
	Action* action = malloc(sizeof(Action));
	action->level_a = BucketGetLevel(a);
	action->level_b = BucketGetLevel(b);
	action->actionType = actionType;
	listTail(actions);
	listAddAfter(actions, action);
}

void ActionUndo(List* actions, Bucket* a, Bucket* b)
{
	Action* action;

	listTail(actions);
	action = listDelCurrent(actions);

	BucketSetLevel(a, action->level_a);
	BucketSetLevel(b, action->level_b);
	free(action);
}

void ActionPrintAll(List* actions, FILE* f)
{
	List* iterator;
	Action* current;

	iterator = listHead(actions);
	current = listGetCurrent(actions);
	while (iterator != NULL)
	{
		fprintf(f, "A = %d, B = %d\n", current->level_a, current->level_b);
		ActionTypePrint(current->actionType, f);
		iterator = listNext(actions);
		current = listGetCurrent(actions);
	}
}

bool ActionWasPerformed(List* actions, Bucket* a, Bucket* b, ActionType actionType)
{
	List* iterator;
	Action* current;

	iterator = listHead(actions);
	current = listGetCurrent(actions);
	while (iterator != NULL)
	{
		if(current->level_a == BucketGetLevel(a) && current->level_b == BucketGetLevel(b) && current->actionType == actionType)
		{
			return true;
		}
		iterator = listNext(actions);
		current = listGetCurrent(actions);
	}
	return false;	
}

void PourAtoB(List* actions, Bucket* a, Bucket* b)
{
	ActionRemember(actions, a, b, POUR_A_TO_B);
	BucketPour(a, b);
}

void PourBtoA(List* actions, Bucket* a, Bucket* b)
{
	ActionRemember(actions, a, b, POUR_B_TO_A);
	BucketPour(b, a);
}

void FillA(List* actions, Bucket* a, Bucket* b)
{
	ActionRemember(actions, a, b, FILL_A);
	BucketFill(a);
}

void FillB(List* actions, Bucket* a, Bucket* b)
{
	ActionRemember(actions, a, b, FILL_B);
	BucketFill(b);
}

void EmptyA(List* actions, Bucket* a, Bucket* b)
{
	ActionRemember(actions, a, b, EMPTY_A);
	BucketEmpty(a);
}

void EmptyB(List* actions, Bucket* a, Bucket* b)
{
	ActionRemember(actions, a, b, EMPTY_B);
	BucketEmpty(b);
}
