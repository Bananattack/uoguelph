#include "Bucket.h"

Bucket* BucketNew(int capacity)
{
	Bucket* new = malloc(sizeof(Bucket));
	new->capacity = capacity;
	new->level = 0;
}

void BucketFree(Bucket* this)
{
	free(this);
}

void BucketFill(Bucket* this)
{
	this->level = this->capacity;
}

void BucketEmpty(Bucket* this)
{
	this->level = 0;
}

bool BucketIsFull(Bucket* this)
{
	return this->level == this->capacity;
}

int BucketGetLevel(Bucket* this)
{
	return this->level;
}

void BucketSetLevel(Bucket* this, int level)
{
	this->level = level;
}

int BucketGetCapacity(Bucket* this)
{
	return this->capacity;
}

void BucketPour(Bucket* this, Bucket* dest)
{
	if(dest->level + this->level <= dest->capacity)
	{
		dest->level += this->level;
		this->level = 0;
	}
	else
	{
		this->level -= dest->capacity - dest->level;
		dest->level = dest->capacity;
	}
}

int BucketPrint(Bucket* this, FILE* f)
{
	fprintf(f, "Bucket %d / %d\n", this->level, this->capacity);
}
