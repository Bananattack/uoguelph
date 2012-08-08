#ifdef __cplusplus
extern "C"
{
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "simRand.h"

/*
#define DEBUG 0
Define this as nonzero if you want debug messages
*/
#define IntRandom simRand
#define UniformRandom simUniformRand
double ExponentialRandom(rand_stream* stream, int lambda);

typedef struct
{
	int isInBerth;
	double timestamp;
} Ship;

Ship* ShipNew(double timestamp);
void ShipFree(Ship* self);
int ShipIsInBerth(Ship* self);
void ShipSetIsInBerth(Ship* self, int value);
double ShipGetTimeStamp(Ship* self);


#define SHIP_ARRIVAL_MEAN 1.25
#define SHIP_MAXIMUM 256
#define BERTH_COUNT 2
#define BERTH_UNLOAD_MINIMUM_TIME 0.5

#define TIME_INCREMENT 0.0001
#define TIME_IN_HOURS 8
#define TIME_IN_MINUTES 60

typedef struct
{
	rand_stream arrivalStream;
	rand_stream unloadStream;

	double berthUnloadingTime[BERTH_COUNT];
	int berthShipIndex[BERTH_COUNT];
	int berthQueue;

	double minShipTime;
	double maxShipTime;
	double sumShipTime;
	int shipsTotal;

	double berthUsage[BERTH_COUNT];
	double craneUsage[BERTH_COUNT];

	Ship* ship[SHIP_MAXIMUM];

	int totalDaysToRun;

	double arrivalTime;

	double time;
} Simulation;

Simulation* SimulationNew(int totalDaysToRun);
rand_stream* SimulationGetArrivalStream(Simulation* self);
rand_stream* SimulationGetUnloadStream(Simulation* self);
void SimulationStep(Simulation* self);
int SimulationIsDone(Simulation* self);
double SimulationGetMinimumTime(Simulation* self);
double SimulationGetMaximumTime(Simulation* self);
double SimulationGetAverageTime(Simulation* self);\


#ifdef __cplusplus
}
#endif
