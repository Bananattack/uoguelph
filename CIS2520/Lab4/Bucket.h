#ifndef BUCKET_H
#define BUCKET_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Bucket_t
{
	int capacity;
	int level;
} Bucket;

Bucket* BucketNew(int capacity);
void BucketFree(Bucket* this);
void BucketFill(Bucket* this);
void BucketEmpty(Bucket* this);
bool BucketIsFull(Bucket* this);
int BucketGetLevel(Bucket* this);
void BucketSetLevel(Bucket* this, int level);
int BucketGetCapacity(Bucket* this);
void BucketPour(Bucket* this, Bucket* dest);
#endif
