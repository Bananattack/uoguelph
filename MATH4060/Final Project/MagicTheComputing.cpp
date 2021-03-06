#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "Mersenne.h"
#include "Combatant.h"

#define RANDOM_SEED 45169
#define MIN_RUN 0
#define MAX_RUN 30
#define POPULATION_SIZE 500
#define MATING_EVENTS_TOTAL 10000
#define SAMPLING_EVENT_DENSITY 100
#define SUBFOLDER	"./" 

Combatant* population[POPULATION_SIZE];
FILE* summary;
FILE* runfile;
long drawCount;
long matingEventCount;

double ttld, ttlp;

void Initialize(int);
void Cleanup();
void Report(int);
void PerformMatingEvent();
void SavePopulation(char*);

Combatant* adversary = NULL;

int main()
{
	int run;
	char filename[64];

	Mersenne::SetRandomSeed(RANDOM_SEED);

//	adversary = new Combatant();
//	adversary->readGene("FIWEFIWE-AAMAA");

	printf(SUBFOLDER "\n");
	summary = fopen(SUBFOLDER"Runsummary.dat", "w");
	fprintf(summary, "SWD		SHLD		FIR		FSW		FSH		ICE		ISW		ISH		WND		WSW		WSH		ETH		ESW		ESH		ATK		DEF		MRP		Sword/Shield		Draw\n");
	for(run = MIN_RUN; run < MAX_RUN; run++)
	{
		// Initialize global draw counters
		ttlp = ttld = 0.0;

		sprintf(filename, SUBFOLDER"run%d.dat",run+1);
		runfile = fopen(filename, "w");
		fprintf(runfile, "SWD		SHLD		FIR		FSW		FSH		ICE		ISW		ISH		WND		WSW		WSH		ETH		ESW		ESH		ATK		DEF		MRP		Sword/Shield\n");

		Initialize(run);
		drawCount = 0;
		for(matingEventCount = 0; (matingEventCount < MATING_EVENTS_TOTAL) && (drawCount < 100000); matingEventCount++)
		{
			if(matingEventCount % SAMPLING_EVENT_DENSITY == 0)
			{
				CombatDebug("Mating event %ld / %d (run %d)\n", matingEventCount, MATING_EVENTS_TOTAL, run);
				CombatDebug("Draws: %ld ", drawCount);
				drawCount = 0;
				Report(0); 
			}
			PerformMatingEvent();
		}
		ttld += drawCount;

		fclose(runfile);

		printf("Run %d complete\n", run);

		sprintf(filename, SUBFOLDER"pop%d.dat", run+1);
		SavePopulation(filename);
		Report(1);
	}
	fclose(summary);
	Cleanup();

//	delete adversary;

	return 0;
}

void Initialize(int run)
{
	int i;

	drawCount = 0;
	if(run != MIN_RUN)
	{
		Cleanup();
	}
	for(i = 0; i < POPULATION_SIZE; i++)
	{
		population[i] = new Combatant();
	}
}

void Cleanup()
{
	int i;
	for(i = 0; i < POPULATION_SIZE; i++)
	{
		delete population[i];
	}
}

void PerformMatingEvent()
{
	CombatResult result;
	int combatantA, combatantB;
	int victorA, victorB;
	int loserA, loserB;

	CombatDebug("Mating...\n");
	// Select first contestant
	do
	{
		combatantA = Mersenne::RandomInt() % POPULATION_SIZE;
		// Select second contestant
		do
		{
			combatantB = Mersenne::RandomInt() % POPULATION_SIZE;
		} while(combatantA == combatantB);
		population[combatantA]->prepare();
		population[combatantB]->prepare();

		// Fight
		result = ProcessCombat(population[combatantA], population[combatantB]);	

		ttlp++;
		if(result.outcome == OUTCOME_DRAW)
		{
			drawCount++;
		}
	} while(result.outcome == OUTCOME_DRAW);

	// First winners and losers
	if(result.outcome == OUTCOME_A_WIN)
	{
		victorA = combatantA;
		loserA = combatantB;
	}
	else
	{
		victorA = combatantB;
		loserA = combatantA;
	}

	do
	{
		combatantA = Mersenne::RandomInt() % POPULATION_SIZE;
		// Select second contestant.
		do
		{
			combatantB = Mersenne::RandomInt() % POPULATION_SIZE;
		} while(combatantA == combatantB);
		population[combatantA]->prepare();
		population[combatantB]->prepare();

		// Fight
		result = ProcessCombat(population[combatantA], population[combatantB]);

		ttlp++;
		if(result.outcome == OUTCOME_DRAW)
		{
			drawCount++;
		}
	} while(result.outcome == OUTCOME_DRAW);

	// Second winners and losers
	if(result.outcome == OUTCOME_A_WIN)
	{
		victorB = combatantA;
		loserB = combatantB;
	}
	else
	{
		victorB = combatantB;
		loserB = combatantA;
	}

	// Copy victors over losers.
	population[victorA]->copyTo(population[loserA]);
	population[victorB]->copyTo(population[loserB]);

	// Crossover the copies.
	population[loserA]->crossover(population[loserB]);

	// Mutate the results.
	population[loserA]->mutate();
	population[loserB]->mutate();
}
/*
void PerformMatingEvent()
{
	int i;
	CombatResult result;
	int winner = -1;
	int winner2 = -1;
	int loser = -1;
	int loser2 = -1;
	int r = 0;
	for(i = 0; i < POPULATION_SIZE; i++)
	{
		result = ProcessCombat(population[i], adversary);
		//printf("Result %d rounds %d outcome", result.rounds, result.outcome);
		if(result.outcome == OUTCOME_A_WIN)
		{
			population[i]->setFitness(result.rounds);
			if(winner == -1 || population[winner]->getFitness() <= population[i]->getFitness())
			{
				winner = i;
				winner2 = winner;
			}
		}
		else
		{
			population[i]->setFitness(0);
			loser = i;
 			loser2 = loser;
		}
	}

	if(winner == -1 || winner2 == -1)
	{
		//printf("We don't have two winners in the population! Crossover at random.");
		r = 1;
	}
	else if (loser == -1 || loser2 == -1)
	{
		//printf("We don't have losers in the population! Crossover at random.");
		r = 1;
	}
	if(!r)
	{
		// Copy victors over losers.
		population[winner]->copyTo(population[loser]);
		population[winner2]->copyTo(population[loser2]);
		// Crossover the copies.
		population[loser]->crossover(population[loser2]);
		// Mutate the results.
		population[loser]->mutate();
		population[loser2]->mutate();
	}
	else
	{
		do
		{
			loser = Mersenne::RandomInt() % POPULATION_SIZE;
			loser2 = Mersenne::RandomInt() % POPULATION_SIZE;
		} while(loser == loser2);

		// Crossover the randoms.
		population[loser]->crossover(population[loser2]);

		// Mutate the results.
		population[loser]->mutate();
		population[loser2]->mutate();		
	}
}*/

void Report(int filep)
{
	int i, j;
	int swordCount = 0;
	int shieldCount = 0;
	int elementCount[ELEMENT_TOTAL];
	int elementSwordCount[ELEMENT_TOTAL];
	int elementShieldCount[ELEMENT_TOTAL];
	int attackCount = 0;
	int defendCount = 0;
	int morphCount = 0;

	for(i = 0; i < ELEMENT_TOTAL; i++)
	{
		elementCount[i] = 0;
		elementSwordCount[i] = 0;
		elementShieldCount[i] = 0;
	}
	for(i = 0; i < POPULATION_SIZE; i++)
	{
		swordCount += population[i]->getSwordCount();
		shieldCount += population[i]->getShieldCount();
		for(j = 0; j < ELEMENT_TOTAL; j++)
		{
			elementCount[j] += population[i]->getElementCount(j);
			elementSwordCount[j] += population[i]->getElementSwordCount(j);
			elementShieldCount[j] += population[i]->getElementShieldCount(j);
		}
		attackCount += population[i]->getActionCount(ACTION_ATTACK);
		defendCount += population[i]->getActionCount(ACTION_DEFEND);
		morphCount += population[i]->getActionCount(ACTION_MORPH);
	}
	
	if(filep)
	{
//		fprintf(summary, "SWD		SHLD		FIR		FSW		FSH		ICE		ISW		ISH		WND		WSW		WSH		ETH		ESW		ESH		ATK		DEF		MRP		Sword/Shield		Draw\n");
		fprintf(summary, "%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f",
				((double)swordCount) / POPULATION_SIZE,
				((double)shieldCount) / POPULATION_SIZE,
				((double)elementCount[ELEMENT_FIRE]) / POPULATION_SIZE,
					((double)elementSwordCount[ELEMENT_FIRE]) / POPULATION_SIZE,
					((double)elementShieldCount[ELEMENT_FIRE]) / POPULATION_SIZE,
				((double)elementCount[ELEMENT_ICE]) / POPULATION_SIZE,
					((double)elementSwordCount[ELEMENT_ICE]) / POPULATION_SIZE,
					((double)elementShieldCount[ELEMENT_ICE]) / POPULATION_SIZE,
				((double)elementCount[ELEMENT_WIND]) / POPULATION_SIZE,
					((double)elementSwordCount[ELEMENT_WIND]) / POPULATION_SIZE,
					((double)elementShieldCount[ELEMENT_WIND]) / POPULATION_SIZE,
				((double)elementCount[ELEMENT_EARTH]) / POPULATION_SIZE,
					((double)elementSwordCount[ELEMENT_EARTH]) / POPULATION_SIZE,
					((double)elementShieldCount[ELEMENT_EARTH]) / POPULATION_SIZE,
				((double)attackCount) / POPULATION_SIZE,
				((double)defendCount) / POPULATION_SIZE,
				((double)morphCount) / POPULATION_SIZE,
				((double) swordCount) / shieldCount
				);
		if(ttlp > 0)
		{
			fprintf(summary, "	%f\n", ttld / ttlp*100);
		}
		else
		{
			fprintf(summary, "	Undefined\n");
		}
	}
	else
	{
		fprintf(runfile, "%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f\n",
				((double)swordCount) / POPULATION_SIZE,
				((double)shieldCount) / POPULATION_SIZE,
				((double)elementCount[ELEMENT_FIRE]) / POPULATION_SIZE,
					((double)elementSwordCount[ELEMENT_FIRE]) / POPULATION_SIZE,
					((double)elementShieldCount[ELEMENT_FIRE]) / POPULATION_SIZE,
				((double)elementCount[ELEMENT_ICE]) / POPULATION_SIZE,
					((double)elementSwordCount[ELEMENT_ICE]) / POPULATION_SIZE,
					((double)elementShieldCount[ELEMENT_ICE]) / POPULATION_SIZE,
				((double)elementCount[ELEMENT_WIND]) / POPULATION_SIZE,
					((double)elementSwordCount[ELEMENT_WIND]) / POPULATION_SIZE,
					((double)elementShieldCount[ELEMENT_WIND]) / POPULATION_SIZE,
				((double)elementCount[ELEMENT_EARTH]) / POPULATION_SIZE,
					((double)elementSwordCount[ELEMENT_EARTH]) / POPULATION_SIZE,
					((double)elementShieldCount[ELEMENT_EARTH]) / POPULATION_SIZE,
				((double)attackCount) / POPULATION_SIZE,
				((double)defendCount) / POPULATION_SIZE,
				((double)morphCount) / POPULATION_SIZE,
				((double) swordCount) / shieldCount
				);
	}
}

void SavePopulation(char* filename)
{
	FILE* f;
	int i;

	f = fopen(filename, "w");
	for(i = 0; i < POPULATION_SIZE; i++)
	{
		population[i]->writeGene(f);
	}
	fclose(f);
}

