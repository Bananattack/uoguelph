#include "adt/List.h"
#include "Action.h"
#include "Bucket.h"

bool TwoBucket(List* actions, Bucket* a, Bucket* b, int desired_level)
{
	if((BucketGetLevel(a) == desired_level && BucketGetLevel(b) == 0)
		|| (BucketGetLevel(a) == 0 && BucketGetLevel(b) == desired_level))
	{
		ActionPrintAll(actions, stdout);
		printf("A = %d, B = %d\n\n", BucketGetLevel(a), BucketGetLevel(b));
		return true;
	}
	else
	{
		if(BucketGetLevel(a) && !BucketIsFull(b))
		{
			if(!ActionWasPerformed(actions, a, b, POUR_A_TO_B))
			{
				PourAtoB(actions, a, b);
				if(!TwoBucket(actions, a, b, desired_level))
				{
					ActionUndo(actions, a, b);
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		if(BucketGetLevel(b) && !BucketIsFull(a))
		{
			if(!ActionWasPerformed(actions, a, b, POUR_B_TO_A))
			{
				PourBtoA(actions, a, b);
				if(!TwoBucket(actions, a, b, desired_level))
				{
					ActionUndo(actions, a, b);
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		if(!BucketGetLevel(a))
		{
			if(!ActionWasPerformed(actions, a, b, FILL_A))
			{
				FillA(actions, a, b);
				if(!TwoBucket(actions, a, b, desired_level))
				{
					ActionUndo(actions, a, b);
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		if(!BucketGetLevel(b))
		{
			if(!ActionWasPerformed(actions, a, b, FILL_B))
			{
				FillB(actions, a, b);
				if(!TwoBucket(actions, a, b, desired_level))
				{
					ActionUndo(actions, a, b);
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		if(BucketGetLevel(a))
		{
			if(!ActionWasPerformed(actions, a, b, EMPTY_A))
			{
				EmptyA(actions, a, b);
				if(!TwoBucket(actions, a, b, desired_level))
				{
					ActionUndo(actions, a, b);
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		if(BucketGetLevel(b))
		{
			if(!ActionWasPerformed(actions, a, b, EMPTY_B))
			{
				EmptyB(actions, a, b);
				if(!TwoBucket(actions, a, b, desired_level))
				{
					ActionUndo(actions, a, b);
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		return false;
	}
}

int main()
{
	List* actions = listCreate();
	Bucket* a = BucketNew(3);
	Bucket* b = BucketNew(5);
	int desired_level = 4;
 
	if (TwoBucket(actions, a, b, desired_level))
	{
		printf("Solution found!\n");
	}
	else
	{
		printf("No solution.\n");
	}

	BucketFree(a);
	BucketFree(b);
	listDelete(actions);
}
