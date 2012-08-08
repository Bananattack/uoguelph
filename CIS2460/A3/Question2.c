#if 1
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
#define CUSTOMER_MEAN 5
#define CUSTOMER_TELLER_PURCHASE_RESISTANCE 0.5
#define CUSTOMER_SOLO_PURCHASE_RESISTANCE 0.35

#define CASHIER_MIN_TIME 5
#define CASHIER_EXTRA_TIME 10

#define TELLER_COUNT 3
#define TELLER_MIN_TIME 5
#define TELLER_MEAN_EXTRA_TIME 10

#define TIME_IN_HOURS 8
#define TIME_IN_MINUTES 60

typedef struct
{
	rand_stream arrivalStream;
	rand_stream decisionStream;
	rand_stream serviceStream;

	int totalDaysToRun;
	int tellerMinutesLeft[TELLER_COUNT];
	int tellerHelpingExpensive[TELLER_COUNT];

	int expensiveQueue;
	int cashoutQueue;

	int sumOfExpensiveQueue;
	int sumOfCashoutQueue;

	int tellerIdleTime;

	int totalCustomers;
	int minutesUntilNextCustomer;

	int minutes;
	int hours;
	int days;
} Simulation;

#define IntRand simRand
#define UniformRandom simUniformRand

double ExponentialRandom(rand_stream* stream, int lambda)
{
	return (-1.0 / (double) lambda) * log(1.0 - UniformRandom(stream));
}

Simulation* SimulationNew(int totalDaysToRun)
{
	int i;
	Simulation* self = malloc(sizeof(Simulation));

	self->arrivalStream = simSeed(time(NULL));
	self->decisionStream = simSeed(IntRand(&(self->arrivalStream)));
	self->serviceStream = simSeed(IntRand(&(self->decisionStream)));
	self->totalDaysToRun = totalDaysToRun;

	for(i = 0; i < TELLER_COUNT; i++)
	{
		self->tellerMinutesLeft[i] = 0;
		self->tellerHelpingExpensive[i] = 0;
	}

	self->expensiveQueue = 0;
	self->cashoutQueue = 0;
	self->sumOfExpensiveQueue = 0;
	self->sumOfCashoutQueue = 0;

	self->totalCustomers = 0;
	self->minutesUntilNextCustomer = ExponentialRandom(&(self->arrivalStream), CUSTOMER_MEAN) * 60;

	self->tellerIdleTime = 0;

	self->minutes = 0;
	self->hours = 0;
	self->days = 0;

	return self;
}

rand_stream* SimulationGetArrivalStream(Simulation* self)
{
	return &(self->arrivalStream);
}

rand_stream* SimulationGetDecisionStream(Simulation* self)
{
	return &(self->decisionStream);
}

rand_stream* SimulationGetServiceStream(Simulation* self)
{
	return &(self->serviceStream);
}

void SimulationPrintTime(Simulation* self)
{
#if DEBUG
	printf("day %d at %d:%02d\n", self->days,  self->hours, self->minutes);
#endif
}

void SimulationTellerTakeExpensiveCustomer(Simulation* self, int tellerIndex)
{
	self->tellerMinutesLeft[tellerIndex] = TELLER_MIN_TIME + ExponentialRandom(
						SimulationGetServiceStream(self), TELLER_MEAN_EXTRA_TIME) * 60;
	self->tellerHelpingExpensive[tellerIndex] = 1;
#if DEBUG
	printf("\tTeller #%d is trying charm a customer into buying something (will take %d minutes).\n", tellerIndex + 1, self->tellerMinutesLeft[tellerIndex]);
#endif
}

void SimulationTellerTakeCashoutCustomer(Simulation* self, int tellerIndex)
{
	self->tellerMinutesLeft[tellerIndex] = CASHIER_MIN_TIME + UniformRandom(SimulationGetServiceStream(self)) * CASHIER_EXTRA_TIME;
	self->tellerHelpingExpensive[tellerIndex] = 0;
#if DEBUG
	printf("\tTeller #%d is cashing out a customer (will take %d minutes).\n", tellerIndex + 1, self->tellerMinutesLeft[tellerIndex]);
#endif
}

void SimulationCustomerPurchasingDecision(Simulation* self, double resistance)
{
	int i;
	if(UniformRandom(SimulationGetDecisionStream(self)) < resistance)
	{
#if DEBUG
			printf("The customer successfully avoided being swindled into purchasing something.\n\n");
#endif
	}
	else
	{
#if DEBUG
		printf("The customer wants to buy something!\n\n");
#endif
		for(i = 0; i < TELLER_COUNT; i++)
		{
			if(!self->tellerMinutesLeft[i])
			{
				SimulationTellerTakeCashoutCustomer(self, i);
				return;
			}
		}
#if DEBUG
		printf("\tThe tellers aren't free, so the customer is waiting.\n");
#endif
		self->cashoutQueue++;
	}
}

void SimulationAddCustomer(Simulation* self)
{
	int i;
#if DEBUG
	printf("A new customer arrived on ");
#endif
	SimulationPrintTime(self);
	if(UniformRandom(SimulationGetDecisionStream(self)) < 0.5)
	{
		for(i = 0; i < TELLER_COUNT; i++)
		{
			if(!self->tellerMinutesLeft[i])
			{
				SimulationTellerTakeExpensiveCustomer(self, i);
				return;
			}
		}
#if DEBUG
		printf("\tThere were no tellers free, so the customer is waiting for one.\n");
#endif
		self->expensiveQueue++;
	}
	else
	{
#if DEBUG
		printf("\tThe customer is helping himself.\n");
#endif
		SimulationCustomerPurchasingDecision(self, CUSTOMER_SOLO_PURCHASE_RESISTANCE);
	}
}

void SimulationStep(Simulation* self)
{
	int i;
	int idled = 0;

	self->sumOfExpensiveQueue += self->expensiveQueue;
	self->sumOfCashoutQueue += self->cashoutQueue;
	/* Has a full hour has passed? */
	if(self->minutes == TIME_IN_MINUTES - 1)
	{
		self->minutes = 0;
		/* Has a full day has passed? */
		if(self->hours == TIME_IN_HOURS - 1)
		{
			self->hours = 0;
			self->days++;
		}
		else
		{
			self->hours++;
		}
	}
	else
	{
		self->minutes++;
	}

	for(i = 0; i < TELLER_COUNT; i++)
	{
		if(self->tellerMinutesLeft[i])
		{
			self->tellerMinutesLeft[i]--;
			if(!self->tellerMinutesLeft[i])
			{
#if DEBUG
				printf("\tTeller #%d finished %s on ", i + 1, self->tellerHelpingExpensive[i] ? "trying to swindle a customer" : "cashing out a customer" );
#endif
				self->tellerHelpingExpensive[i] = 0;
				SimulationPrintTime(self);
				SimulationCustomerPurchasingDecision(self, CUSTOMER_TELLER_PURCHASE_RESISTANCE);
				if(self->expensiveQueue)
				{
#if DEBUG
					printf("\t\t...But there's still another customer wating to buy expensive stuff!\n");
#endif
					SimulationTellerTakeExpensiveCustomer(self, i);
					self->expensiveQueue--;
				}
				else if(self->cashoutQueue)
				{
#if DEBUG
					printf("\t\t...But there's still another customer wating for the cashout!\n", i);
#endif
					SimulationTellerTakeCashoutCustomer(self, i);
					self->cashoutQueue--;
				}
			}
		}
		else if(!idled)
		{
			self->tellerIdleTime++;
			idled = 1;
		}
	}

	if(self->minutesUntilNextCustomer)
	{
		self->minutesUntilNextCustomer--;
	}
	/* A new customer! */
	else
	{
		self->minutesUntilNextCustomer = ExponentialRandom(
					SimulationGetArrivalStream(self), CUSTOMER_MEAN) * 60;
		SimulationAddCustomer(self);

		self->totalCustomers++;
	}
	
}

int SimulationIsDone(Simulation* self)
{
	return self->minutes == 0
		&& self->hours == 0
		&& self->days == self->totalDaysToRun;
}

double SimulationGetCombinedQueueAverage(Simulation* self)
{
	return (double) (self->sumOfExpensiveQueue + self->sumOfCashoutQueue) / (double) (self->totalDaysToRun * TIME_IN_HOURS * TIME_IN_MINUTES);
}

double SimulationGetExpensiveQueueAverage(Simulation* self)
{
	return (double) self->sumOfExpensiveQueue / (double) (self->totalDaysToRun * TIME_IN_HOURS * TIME_IN_MINUTES);
}

double SimulationGetCashoutQueueAverage(Simulation* self)
{
	return (double) self->sumOfCashoutQueue / (double) (self->totalDaysToRun * TIME_IN_HOURS * TIME_IN_MINUTES);
}

double SimulationGetProbabilityOfIdleTeller(Simulation* self)
{
	return (double) self->tellerIdleTime / (double) (self->totalDaysToRun * TIME_IN_HOURS * TIME_IN_MINUTES);
}

int main(int argc, char** argv)
{
	Simulation* simulation;

	if(argc <= 1)
	{
		printf("Usage: %s <time in days>\n", argv[0]);
		exit(-1);	
	}
	printf("Running for %d days!\n", atoi(argv[1]));

	simulation = SimulationNew(atoi(argv[1]));
	
	while(!SimulationIsDone(simulation))
	{
		SimulationStep(simulation);
	}

	printf("\n");
	printf(">> average wait for customers who got help on the floor: %f\n", SimulationGetExpensiveQueueAverage(simulation));
	printf(">> average wait for customers who just wanted to be cashed out: %f\n", SimulationGetCashoutQueueAverage(simulation));
	printf(">> average wait for both types of customers: %f\n", SimulationGetCombinedQueueAverage(simulation));
	printf(">> probability of an idle teller: %f\n", SimulationGetProbabilityOfIdleTeller(simulation));
	printf(">> Total customers %d\n", simulation->totalCustomers);
}

#ifdef __cplusplus
}
#endif
#endif

