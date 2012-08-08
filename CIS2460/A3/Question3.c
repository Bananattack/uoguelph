#if 1
#ifdef __cplusplus
extern "C"
{
#endif
#include "Question3.h"

double ExponentialRandom(rand_stream* stream, int lambda)
{
	return (-1.0 / (double) lambda) * log(1.0 - UniformRandom(stream));
}

Ship* ShipNew(double timestamp)
{
	Ship* self = malloc(sizeof(Ship));

	self->isInBerth = 0;
	self->timestamp = timestamp;
	return self;
}

void ShipFree(Ship* self)
{
	free(self);
}

int ShipIsInBerth(Ship* self)
{
	return self->isInBerth;
}

void ShipSetIsInBerth(Ship* self, int value)
{
	self->isInBerth = value;
}

double ShipGetTimeStamp(Ship* self)
{
	return self->timestamp;
}


Simulation* SimulationNew(int totalDaysToRun)
{
	int i;
	Simulation* self = malloc(sizeof(Simulation));

	self->arrivalStream = simSeed(time(NULL));
	self->unloadStream = simSeed(IntRandom(&(self->arrivalStream)));

	self->totalDaysToRun = totalDaysToRun;

	for(i = 0; i < BERTH_COUNT; i++)
	{
		self->berthUnloadingTime[i] = 0;
		self->berthShipIndex[i] = -1;
		self->berthUsage[i] = 0;
		self->craneUsage[i] = 0;
	}

	self->berthQueue = 0;

	self->minShipTime = -1.0;
	self->maxShipTime = 0;
	self->sumShipTime = 0;
	self->shipsTotal = 0;

	for(i = 0; i < SHIP_MAXIMUM; i++)
	{
		self->ship[i] = NULL;
	}

	self->time = 0.0;

	return self;
}

rand_stream* SimulationGetArrivalStream(Simulation* self)
{
	return &(self->arrivalStream);
}

rand_stream* SimulationGetUnloadStream(Simulation* self)
{
	return &(self->unloadStream);
}

void SimulationStep(Simulation* self)
{
	int shipIndex, berthIndex;
	double timeDifference;

	self->time += TIME_INCREMENT;

	for(berthIndex = 0; berthIndex < BERTH_COUNT; berthIndex++)
	{
		/* In the middle of an unload */
		if(self->berthUnloadingTime[berthIndex] > 0)
		{
			/* Decrement remaining time */
			self->berthUnloadingTime[berthIndex] -= TIME_INCREMENT;
			/* Update usage time */
			self->berthUsage[berthIndex] += TIME_INCREMENT;
			self->craneUsage[berthIndex] += TIME_INCREMENT;
			/* !(i & 1) == 0 if odd, == 1 if even */
			/* Uses secondary crane if only one berth is occupied. */
			if(self->berthUnloadingTime[berthIndex] > 0
				&& self->berthUnloadingTime[!(berthIndex & 1)] <= 0)
			{
				self->berthUnloadingTime[berthIndex] -= TIME_INCREMENT;
				self->craneUsage[!(berthIndex & 1)] += TIME_INCREMENT;
			}
			/* Finished unloading a ship, prepare for a fresh ship */
			if(self->berthUnloadingTime[berthIndex] <= 0 && self->berthShipIndex[berthIndex] != -1)
			{
				shipIndex = self->berthShipIndex[berthIndex]; 
				timeDifference = self->time - ShipGetTimeStamp(self->ship[shipIndex]);
				if(self->minShipTime == -1 || timeDifference < self->minShipTime)
				{
					self->minShipTime = timeDifference;
				}
				if(timeDifference > self->maxShipTime)
				{
					self->maxShipTime = timeDifference;
				}
				self->sumShipTime += timeDifference;
#if DEBUG
				printf("Removing ship %d from berth #%d at %f\n", shipIndex, berthIndex, self->time);
				printf("(It was in the harbour for %f days)\n", timeDifference);
#endif
				ShipFree(self->ship[shipIndex]);
				self->ship[shipIndex] = NULL;

				self->berthShipIndex[berthIndex] = -1;
			}
		}
		/* Take an active ship handle that is not being helped yet from the queue */
		if(self->berthUnloadingTime[berthIndex] <= 0 && self->berthQueue)
		{
			for(shipIndex = 0; shipIndex < SHIP_MAXIMUM; shipIndex++)
			{
				if(self->ship[shipIndex] != NULL
						&& !ShipIsInBerth(self->ship[shipIndex]))
				{
					self->berthShipIndex[berthIndex] = shipIndex;
					self->berthUnloadingTime[berthIndex] = BERTH_UNLOAD_MINIMUM_TIME + UniformRandom(SimulationGetUnloadStream(self));
#if DEBUG
					printf("Ship %d from queue is now in berth #%d (will take %f days) at %f!\n", shipIndex, berthIndex, self->berthUnloadingTime[berthIndex], self->time);	
#endif
					ShipSetIsInBerth(self->ship[shipIndex], 1);

					self->berthQueue--;
					break;
				}
			}
		}
	}

	/* Decreasing time until arrival */
	if(self->arrivalTime > 0)
	{
		self->arrivalTime -= TIME_INCREMENT;
	}
	/* A new ship! */
	else
	{
		self->arrivalTime = ExponentialRandom(
					SimulationGetArrivalStream(self), SHIP_ARRIVAL_MEAN);

		self->shipsTotal++;

		/* Find an unused ship handle and populate it. */
		for(shipIndex = 0; shipIndex < SHIP_MAXIMUM; shipIndex++)
		{
			if(self->ship[shipIndex] == NULL)
			{
				self->ship[shipIndex] = ShipNew(self->time);
				for(berthIndex = 0; berthIndex < BERTH_COUNT; berthIndex++)
				{
					if(self->berthUnloadingTime[berthIndex] <= 0)
					{

						self->berthUnloadingTime[berthIndex] = BERTH_UNLOAD_MINIMUM_TIME + UniformRandom(SimulationGetUnloadStream(self));
						self->berthShipIndex[berthIndex] = shipIndex;
#if DEBUG
						printf("Ship %d is now in berth #%d (will take %f days) at %f!\n", shipIndex, berthIndex, self->berthUnloadingTime[berthIndex], self->time);				
#endif
						ShipSetIsInBerth(self->ship[shipIndex], 1);
						break;
					}
				}
				if(!ShipIsInBerth(self->ship[shipIndex]))
				{
#if DEBUG
					printf("Ship %d is now in queue at %f.\n", shipIndex, self->time);
#endif
					self->berthQueue++;
				}
				break;
			}
		}
	}
	
}

int SimulationIsDone(Simulation* self)
{
	return self->time >= self->totalDaysToRun;
}

double SimulationGetMinimumTime(Simulation* self)
{
	return self->minShipTime;
}


double SimulationGetMaximumTime(Simulation* self)
{
	return self->maxShipTime;
}

double SimulationGetAverageTime(Simulation* self)
{
	return self->sumShipTime / (double) self->shipsTotal;
}

double SimulationGetBerthUsage(Simulation* self, int index)
{
	return self->berthUsage[index] / (double) self->totalDaysToRun;
}

double SimulationGetCraneUsage(Simulation* self, int index)
{
	return self->craneUsage[index] / (double) self->totalDaysToRun;
}


int main(int argc, char** argv)
{
	int i;
	Simulation* simulation;

	if(argc <= 1)
	{
		printf("Usage: %s <time in days>\n", argv[0]);
		exit(-1);	
	}
	printf(">> Running for %d days... (may take a bit to run events)\n", atoi(argv[1]));

	simulation = SimulationNew(atoi(argv[1]));
	
	while(!SimulationIsDone(simulation))
	{
		SimulationStep(simulation);
	}
	printf("\n");
	printf(">> min time = %f\n", SimulationGetMinimumTime(simulation));
	printf(">> max time = %f\n", SimulationGetMaximumTime(simulation));
	printf(">> average time = %f\n", SimulationGetAverageTime(simulation));
	for(i = 0; i < BERTH_COUNT; i++)
	{
		printf(">> berth %d usage: %f\n", i + 1, SimulationGetBerthUsage(simulation, i));
		printf(">> crane %d usage: %f\n", i + 1, SimulationGetCraneUsage(simulation, i));
	}
	printf(">> total ships %d\n", simulation->shipsTotal);
}

#ifdef __cplusplus
}
#endif
#endif

