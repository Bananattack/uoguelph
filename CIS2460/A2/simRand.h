#define RAND_STREAM_GENERATOR_COUNT 3
#define RAND_STREAM_MAXIMUM (1 << 31) - 1
#define RAND_STREAM_MAXIMUM2 (1 << 31) - 7
#define RAND_STREAM_MAXIMUM3 (1 << 31) - 15

typedef struct 
{
	int value;
	long increment;
	long multiplier;
	long max;
} lcg_stream;

typedef struct
{
	lcg_stream streams[RAND_STREAM_GENERATOR_COUNT];
} rand_stream;


int lcgMod(int value, int max);
int lcgRand(lcg_stream* stream);
rand_stream simSeed(int seed);
long simRandMax(rand_stream* stream);
long simRandPeriodicity(rand_stream* stream);
double simUniformRand(rand_stream* stream);
int simRand(rand_stream* stream);