#ifdef __cplusplus
extern "C"
{
#endif
#include "simRand.h"

int lcgMod(int value, int max)
{
	value %= max;
	return (value < 0) ? (value + max - 1) % max : value; 
}

int lcgRand(lcg_stream* stream)
{
	return lcgMod(stream->multiplier * stream->value + stream->increment, stream->max);
}

rand_stream simSeed(int seed)
{
	int i;
	lcg_stream stream;
	rand_stream result;

	stream.value = seed;
	stream.increment = 0;
	stream.multiplier = 59;
	stream.max = RAND_STREAM_MAXIMUM;
	
	result.streams[0].max = RAND_STREAM_MAXIMUM;
	result.streams[1].max = RAND_STREAM_MAXIMUM2;
	result.streams[2].max = RAND_STREAM_MAXIMUM3;

	for(i = 0; i < RAND_STREAM_GENERATOR_COUNT; i++)
	{
		stream.value = result.streams[i].value = lcgRand(&stream);
	}
	
	result.streams[0].multiplier = 3 + 8 * 59;
	result.streams[1].multiplier = 5 + 8 * 244;
	result.streams[2].multiplier = 3 + 8 * 112;
	
	result.streams[0].increment = 0;
	result.streams[1].increment = 1;
	result.streams[2].increment = 4;
	return result;
}

long simRandMax(rand_stream* stream)
{
	return RAND_STREAM_MAXIMUM;
}

long simRandPeriodicity(rand_stream* stream)
{
	return (((long) RAND_STREAM_MAXIMUM - 1) * ((long) RAND_STREAM_MAXIMUM2 - 1) * ((long) RAND_STREAM_MAXIMUM3 - 1)) / 4;
}

double simUniformRand(rand_stream* stream)
{
	return ((double) simRand(stream)) / ((double) simRandMax(stream) - 1.0);
}

int simRand(rand_stream* stream)
{
	int i;
	int v = 0;
	for(i = 0; i < 3; i++)
	{
		stream->streams[i].value = lcgRand(&(stream->streams[i]));
		v += ((i & 1 == 0) ? -1 : +1) * stream->streams[i].value;
	}                                                                                                                                                                                                                                                                                                                        

	return lcgMod(v, simRandMax(stream));
}
#ifdef __cplusplus
}
#endif

