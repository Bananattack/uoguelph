#ifndef __COMBATANT_H
#define __COMBATANT_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Mersenne.h"

#define CombatDebug(...) //printf(__VA_ARGS__)

// Various constants that affect combatants
#define COMBATANT_MAX_HP 30
#define COMBATANT_EQUIPMENT_SLOT_COUNT 8
#define COMBATANT_ACTION_SLOT_COUNT 5

// Element definition
#define ELEMENT_TOTAL 4

#define ELEMENT_FIRE 0
#define ELEMENT_ICE 1
#define ELEMENT_WIND 2
#define ELEMENT_EARTH 3
typedef int Element;

// Equipment definition
#define EQUIPMENT_TOTAL	8

#define EQUIPMENT_NONE 0
#define EQUIPMENT_FIRE_SWORD 1
#define EQUIPMENT_FIRE_SHIELD 2
#define EQUIPMENT_ICE_SWORD 3
#define EQUIPMENT_ICE_SHIELD 4
#define EQUIPMENT_WIND_SWORD 5
#define EQUIPMENT_WIND_SHIELD 6
#define EQUIPMENT_EARTH_SWORD 7
#define EQUIPMENT_EARTH_SHIELD 8
typedef int Equipment;


// Action definition
#define ACTION_TOTAL 3

#define ACTION_NONE 0
#define ACTION_ATTACK 1
#define ACTION_DEFEND 2
#define ACTION_MORPH 3
typedef int Action;

// Combatant definition
class Combatant
{
	public:
		Combatant();
		~Combatant();

		int getFitness();
		void setFitness(int fitness);

		void writeGene(FILE* f);
		void readGene(char* s);
		void readGene(FILE* f);
		void prepare();
		void copyTo(Combatant* target);

		void hurt(int amount);
		void heal(int amount);
		bool isDead();
		int getHP();
		void setHP(int hp);
		int getMaxHP();
		void setMaxHP(int maxHP);

		bool isDisqualified();
		int getSwordCount();
		int getShieldCount();
		int getElementCount(Element element);
		int getElementSwordCount(Element element);
		int getElementShieldCount(Element element);
		Element getDominantAttackElement();
		int getActionCount(Action a);

		Equipment getEquipment(int index);
		void breakEquipment(int index);
		void setEquipment(int index, Equipment e);
		Equipment getStartingEquipment(int index);
		void setStartingEquipment(int index, Equipment e);
		Action getAction(int index);
		Action getCurrentAction();
		void setAction(int index, Action a);

		void think(Combatant* target);
		void attack(Combatant* target);
		void defend();
		void morph(Combatant* target);

		void mutate();
		void crossover(Combatant* target);
	private:
		int fitness;
		int hp, maxHP;
		int actionIndex;
		Equipment equipment[COMBATANT_EQUIPMENT_SLOT_COUNT];
		Action action[COMBATANT_ACTION_SLOT_COUNT];
		Equipment startingEquipment[COMBATANT_EQUIPMENT_SLOT_COUNT];
};

#define OUTCOME_A_WIN 0
#define OUTCOME_B_WIN 1
#define OUTCOME_DRAW 2

struct CombatResult
{
	int rounds;
	int outcome;
};

CombatResult ProcessCombat(Combatant* a, Combatant* b);

#endif
