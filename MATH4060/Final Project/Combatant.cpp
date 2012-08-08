#include "Combatant.h"

Combatant::Combatant()
{
	int i;

	setHP(COMBATANT_MAX_HP);
	setMaxHP(COMBATANT_MAX_HP);

	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		setEquipment(i, Mersenne::RandomInt() % EQUIPMENT_TOTAL + 1);
	}

	actionIndex = 0;
	for(i = 0; i < COMBATANT_ACTION_SLOT_COUNT; i++)
	{
		setAction(i, Mersenne::RandomInt() % ACTION_TOTAL + 1); 
	}
}

Combatant::~Combatant()
{
}

int Combatant::getFitness()
{
	return fitness;
}

void Combatant::setFitness(int fitness)
{
	this->fitness = fitness;
}

void Combatant::writeGene(FILE* f)
{
	int i;
	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++) 
	{
		switch(getStartingEquipment(i))
		{
			case EQUIPMENT_FIRE_SWORD:
				fprintf(f, "F");
				break;
			case EQUIPMENT_FIRE_SHIELD:
				fprintf(f, "f");
				break;
			case EQUIPMENT_ICE_SWORD:
				fprintf(f, "I");
				break;
			case EQUIPMENT_ICE_SHIELD:
				fprintf(f, "i");
				break;
			case EQUIPMENT_WIND_SWORD:
				fprintf(f, "W");
				break;
			case EQUIPMENT_WIND_SHIELD:
				fprintf(f, "w");
				break;
			case EQUIPMENT_EARTH_SWORD:
				fprintf(f, "E");
				break;
			case EQUIPMENT_EARTH_SHIELD:
				fprintf(f, "e");
				break;
			case EQUIPMENT_NONE:
				fprintf(f, "_");
				break;
		}
	}
	fprintf(f, "-");
	for(i = 0; i < COMBATANT_ACTION_SLOT_COUNT; i++) 
	{
		switch(getAction(i))
		{
			case ACTION_ATTACK:
				fprintf(f, "A");
				break;
			case ACTION_DEFEND:
				fprintf(f, "D");
				break;
			case ACTION_MORPH:
				fprintf(f, "M");
				break;
			case ACTION_NONE:
				fprintf(f, "_");
				break;
		}
	}
	fprintf(f, "\n");
}

void Combatant::readGene(char* s)
{
	bool abilityMode = false;
	unsigned int i;
	int index = 0;
	for(i = 0; i < strlen(s); i++)
	{
		if(!abilityMode)
		{
			switch(s[i])
			{
				case 'F':
					setStartingEquipment(index, EQUIPMENT_FIRE_SWORD);
					index++;
					break;
				case 'I':
					setStartingEquipment(index, EQUIPMENT_ICE_SWORD);
					index++;
					break;
				case 'W':
					setStartingEquipment(index, EQUIPMENT_WIND_SWORD);
					index++;
					break;
				case 'E':
					setStartingEquipment(index, EQUIPMENT_EARTH_SWORD);
					index++;
					break;
				case 'f':
					setStartingEquipment(index, EQUIPMENT_FIRE_SHIELD);
					index++;
					break;
				case 'i':
					setStartingEquipment(index, EQUIPMENT_ICE_SHIELD);
					index++;
					break;
				case 'w':
					setStartingEquipment(index, EQUIPMENT_WIND_SHIELD);
					index++;
					break;
				case 'e':
					setStartingEquipment(index, EQUIPMENT_EARTH_SHIELD);
					index++;
					break;
				case '_':
					setStartingEquipment(index, EQUIPMENT_NONE);
					index++;
					break;
				case '-':
					index = 0;
					abilityMode = true;
					break;
			}
		}
		else
		{
			switch(s[i])
			{
				case 'A':
					setAction(index, ACTION_ATTACK);
					break;
				case 'D':
					setAction(index, ACTION_DEFEND);
					break;
				case 'M':
					setAction(index, ACTION_MORPH);
					break;
				case '_':
					setAction(index, ACTION_NONE);
					break;
			}
		}
	}
}

void Combatant::readGene(FILE* f)
{
	char buffer[4096];
	fgets(buffer, 4096, f);
	readGene(buffer);
}

void Combatant::prepare()
{
	int i;
	setHP(getMaxHP());
	actionIndex = 0;
	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		setEquipment(i, getStartingEquipment(i));
	}
}

void Combatant::copyTo(Combatant* target)
{
	int i;

	target->setHP(getHP());
	target->setMaxHP(getMaxHP());

	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		setEquipment(i, Mersenne::RandomInt() % EQUIPMENT_TOTAL + 1);
	}

	actionIndex = 0;
	for(i = 0; i < COMBATANT_ACTION_SLOT_COUNT; i++)
	{
		setAction(i, Mersenne::RandomInt() % ACTION_TOTAL + 1); 
	}
}

void Combatant::hurt(int amount)
{
	hp -= amount;
	if(hp < 0)
	{
		hp = 0;
	}
}

void Combatant::heal(int amount)
{
	hp += amount;
	if(hp > maxHP)
	{
		hp = maxHP;
	}
}

bool Combatant::isDead()
{
	return hp <= 0 || getActionCount(ACTION_NONE) == COMBATANT_ACTION_SLOT_COUNT;
}

int Combatant::getHP()
{
	return hp;
}

void Combatant::setHP(int hp)
{
	this->hp = hp;
}

int Combatant::getMaxHP()
{
	return maxHP;
}

void Combatant::setMaxHP(int maxHP)
{
	this->maxHP = maxHP;
}

bool Combatant::isDisqualified()
{
	return getActionCount(ACTION_DEFEND) + getActionCount(ACTION_MORPH) == COMBATANT_ACTION_SLOT_COUNT;
}

int Combatant::getSwordCount()
{
	int count = 0;
	for(int i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		switch(getEquipment(i))
		{
			case EQUIPMENT_FIRE_SWORD:
			case EQUIPMENT_ICE_SWORD:
			case EQUIPMENT_WIND_SWORD:
			case EQUIPMENT_EARTH_SWORD:
				count++;
 				break;
			default:
				break;
		}
	}
	return count;
}

int Combatant::getShieldCount()
{
	int count = 0;
	for(int i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		switch(getEquipment(i))
		{
			case EQUIPMENT_FIRE_SHIELD:
			case EQUIPMENT_ICE_SHIELD:
			case EQUIPMENT_WIND_SHIELD:
			case EQUIPMENT_EARTH_SHIELD:
				count++;
 				break;
			default:
				break;
		}
	}
	return count;
}


int Combatant::getElementCount(Element element)
{
	int count = 0;
	for(int i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		switch(element)
		{
			case ELEMENT_FIRE:
				switch(getEquipment(i))
				{
					case EQUIPMENT_FIRE_SWORD:
					case EQUIPMENT_FIRE_SHIELD:
						count++;
		 				break;
					default:
						break;
				}
				break;
			case ELEMENT_ICE:
				switch(getEquipment(i))
				{
					case EQUIPMENT_ICE_SWORD:
					case EQUIPMENT_ICE_SHIELD:
						count++;
		 				break;
					default:
						break;
				}
				break;
			case ELEMENT_WIND:
				switch(getEquipment(i))
				{
					case EQUIPMENT_WIND_SWORD:
					case EQUIPMENT_WIND_SHIELD:
						count++;
		 				break;
					default:
						break;
				}
				break;
			case ELEMENT_EARTH:
				switch(getEquipment(i))
				{
					case EQUIPMENT_EARTH_SWORD:
					case EQUIPMENT_EARTH_SHIELD:
						count++;
		 				break;
					default:
						break;
				}
				break;
		}
	}
	return count;
}

int Combatant::getElementSwordCount(Element element)
{
	int count = 0;
	for(int i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		switch(element)
		{
			case ELEMENT_FIRE:
				if(getEquipment(i) == EQUIPMENT_FIRE_SWORD)
				{
					count++;
				}
				break;
			case ELEMENT_ICE:
				if(getEquipment(i) == EQUIPMENT_ICE_SWORD)
				{
					count++;
				}
				break;
			case ELEMENT_WIND:
				if(getEquipment(i) == EQUIPMENT_WIND_SWORD)
				{
					count++;
				}
				break;
			case ELEMENT_EARTH:
				if(getEquipment(i) == EQUIPMENT_EARTH_SWORD)
				{
					count++;
				}
				break;
		}
	}
	return count;
}


int Combatant::getElementShieldCount(Element element)
{
	int count = 0;
	for(int i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		switch(element)
		{
			case ELEMENT_FIRE:
				if(getEquipment(i) == EQUIPMENT_FIRE_SHIELD)
				{
					count++;
				}
				break;
			case ELEMENT_ICE:
				if(getEquipment(i) == EQUIPMENT_ICE_SHIELD)
				{
					count++;
				}
				break;
			case ELEMENT_WIND:
				if(getEquipment(i) == EQUIPMENT_WIND_SHIELD)
				{
					count++;
				}
				break;
			case ELEMENT_EARTH:
				if(getEquipment(i) == EQUIPMENT_EARTH_SHIELD)
				{
					count++;
				}
				break;
		}
	}
	return count;
}

Element Combatant::getDominantAttackElement()
{
	int i;
	int count = 0;
	Element element = ELEMENT_FIRE;
	for(i = 0; i < ELEMENT_TOTAL; i++)
	{
		if(getElementSwordCount(i) > count)
		{
			count = getElementSwordCount(i);
			element = i;
		}
	}
	return element;
}

int Combatant::getActionCount(Action a)
{
	int count = 0;
	for(int i = 0; i < COMBATANT_ACTION_SLOT_COUNT; i++)
	{
		if(getAction(i) == a)
		{
			count++;
		}
	}
	return count;
}

Equipment Combatant::getEquipment(int index)
{
	return equipment[index];
}

void Combatant::breakEquipment(int index)
{
	equipment[index] = EQUIPMENT_NONE;
}

void Combatant::setEquipment(int index, Equipment e)
{
	equipment[index] = e;
}

Equipment Combatant::getStartingEquipment(int index)
{
	return startingEquipment[index];
}

void Combatant::setStartingEquipment(int index, Equipment e)
{
	startingEquipment[index] = e;
}

Action Combatant::getAction(int index)
{
	if(actionIndex < 0 || actionIndex > COMBATANT_ACTION_SLOT_COUNT)
	{
		return ACTION_MORPH;
	}
	return action[index];
}

Action Combatant::getCurrentAction()
{
	return getAction(actionIndex);
}

void Combatant::setAction(int index, Action a)
{
	action[index] = a;
}

void Combatant::think(Combatant* target)
{
	CombatDebug("Performing action\n");
/*	if (actionIndex >= COMBATANT_ACTION_SLOT_COUNT)
	{
		morph(target);
		actionIndex = 0;
	}
	else
	{
		switch(getCurrentAction())
		{
			case ACTION_ATTACK:
				attack(target);
				break;
			case ACTION_DEFEND:
				defend();
				break;
			case ACTION_MORPH:
				morph(target);
				break;
		}
		actionIndex++;
	}*/

	switch(getCurrentAction())
	{
		case ACTION_ATTACK:
			attack(target);
			break;
		case ACTION_DEFEND:
			defend();
			break;
		case ACTION_MORPH:
			morph(target);
			break;
	}
	actionIndex++;
	if(actionIndex >= COMBATANT_ACTION_SLOT_COUNT)
	{
		actionIndex = 0;
	}
	CombatDebug("Action finished.\n");
}

void Combatant::attack(Combatant* target)
{
	int i;
	int count = 0;
	int breakCount = 0;
	int damage = 0;
	Element attackType;

	CombatDebug("Attacking!\n");

	// Select elemental of the attack at random
	//attackType = Mersenne::RandomInt() % ELEMENT_TOTAL;
	// Select dominant attack element
	attackType = getDominantAttackElement();

	// Count the pieces of weaponry this combatant has,
	// which is identical to the amount of damage done if the attack is unblocked.
	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		switch(attackType)
		{
			case ELEMENT_FIRE:
				if(getEquipment(i) == EQUIPMENT_FIRE_SWORD)
				{
					count++;
				}
				break;
			case ELEMENT_ICE:
				if(getEquipment(i) == EQUIPMENT_ICE_SWORD)
				{
					count++;
				}
				break;
			case ELEMENT_WIND:
				if(getEquipment(i) == EQUIPMENT_WIND_SWORD)
				{
					count++;
				}
				break;
			case ELEMENT_EARTH:
				if(getEquipment(i) == EQUIPMENT_EARTH_SWORD)
				{
					count++;
				}
				break;
		}
	}
	damage = count;

	// Adjust damage according to the target's elemental shields.
	// The target need only defend with as many shields as the player has swords.
	// Opposing shields are counted after same element shields run out.

	// First, same shields. They cancel one unit of damage.
	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		if(count == 0)
		{
			break;
		}
		switch(attackType)
		{
			case ELEMENT_FIRE:
				if(target->getEquipment(i) == EQUIPMENT_FIRE_SHIELD)
				{
					damage--;
					count--;
				}
				break;
			case ELEMENT_ICE:
				if(target->getEquipment(i) == EQUIPMENT_ICE_SHIELD)
				{
					damage--;
					count--;
				}
				break;
			case ELEMENT_WIND:
				if(target->getEquipment(i) == EQUIPMENT_WIND_SHIELD)
				{
					damage--;
					count--;
				}
				break;
			case ELEMENT_EARTH:
				if(target->getEquipment(i) == EQUIPMENT_EARTH_SHIELD)
				{
					damage--;
					count--;
				}
				break;
		}
	}


	// Now, shields of the opposite element. They cause 1 extra unit of damage per sword they try to block.
	// When defending, these opposing shields will break 1 sword.
	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		if(count == 0)
		{
			break;
		}
		switch(attackType)
		{
			case ELEMENT_FIRE:
				if(target->getEquipment(i) == EQUIPMENT_ICE_SHIELD)
				{
					damage++;
					count--;
					if(target->getCurrentAction() == ACTION_DEFEND)
					{
						breakCount++;
					}
				}
				break;
			case ELEMENT_ICE:
				if(target->getEquipment(i) == EQUIPMENT_FIRE_SHIELD)
				{
					damage++;
					count--;
					if(target->getCurrentAction() == ACTION_DEFEND)
					{
						breakCount++;
					}
				}
				break;
			case ELEMENT_WIND:
				if(target->getEquipment(i) == EQUIPMENT_EARTH_SHIELD)
				{
					damage++;
					count--;
					if(target->getCurrentAction() == ACTION_DEFEND)
					{
						breakCount++;
					}
				}
				break;
			case ELEMENT_EARTH:
				if(target->getEquipment(i) == EQUIPMENT_WIND_SHIELD)
				{
					damage++;
					count--;
					if(target->getCurrentAction() == ACTION_DEFEND)
					{
						breakCount++;
					}
				}
				break;
		}
	}

	// Damage is divided by two and rounded down on defending
	if(target->getCurrentAction() == ACTION_DEFEND)
	{
		damage /= 2;
	}

	// If anything needs breaking, do it!
	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		if(breakCount == 0)
		{
			break;
		}
		switch(attackType)
		{
			case ELEMENT_FIRE:
				if(getEquipment(i) == EQUIPMENT_FIRE_SWORD)
				{
					breakEquipment(i);
					breakCount--;
				}
				break;
			case ELEMENT_ICE:
				if(getEquipment(i) == EQUIPMENT_ICE_SWORD)
				{
					breakEquipment(i);
					breakCount--;
				}
				break;
			case ELEMENT_WIND:
				if(getEquipment(i) == EQUIPMENT_WIND_SWORD)
				{
					breakEquipment(i);
					breakCount--;
				}
				break;
			case ELEMENT_EARTH:
				if(getEquipment(i) == EQUIPMENT_EARTH_SWORD)
				{
					breakEquipment(i);
					breakCount--;
				}
				break;
		}
	}

	// Always do at least 1 damage.
	if(damage == 0)
	{
		damage = 1;
	}
	// Okay, that's everything. Now apply the damage to the opponent if any.
	target->hurt(damage);
	CombatDebug("Attack dealt for %d damage\n", damage);
}

void Combatant::defend()
{
	CombatDebug("Defending!\n");
}
/*
void Combatant::morph(Combatant* target)
{
	int i, j;
	CombatDebug("Morphing!\n");

	// We prefer to populate empty slots when morphing the performing combatant,
	// followed by mutatation of random slots if their equipment's full.
	// Try to find an empty slot
	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		if(getEquipment(i) == EQUIPMENT_NONE)
		{
			break;
		}
	}
	// Found one
	if(i < COMBATANT_EQUIPMENT_SLOT_COUNT)
	{
		// Try to find a second empty slot
		for(j = i; j < COMBATANT_EQUIPMENT_SLOT_COUNT; j++)
		{
			if(getEquipment(j) == EQUIPMENT_NONE)
			{
				break;
			}
		}

		// Didn't find a second empty, so keep trying randoms
		// until j is different from i.
		if(j == COMBATANT_EQUIPMENT_SLOT_COUNT)
		{
		}
		else
		{
			do
			{
				j = Mersenne::RandomInt() % COMBATANT_EQUIPMENT_SLOT_COUNT;
			} while(i == j);
		}

	}
	// Otherwise, try random slots.
	else
	{
		do
		{
			i = Mersenne::RandomInt() % COMBATANT_EQUIPMENT_SLOT_COUNT;
			j = Mersenne::RandomInt() % COMBATANT_EQUIPMENT_SLOT_COUNT;
		} while(i == j);
	}

	// MORPH USER!
	setEquipment(i, Mersenne::RandomInt() % EQUIPMENT_TOTAL + 1);
	setEquipment(j, Mersenne::RandomInt() % EQUIPMENT_TOTAL + 1);

	// We prefer to populate random filled slots when morphing the target,
	// followed by simply adding something if everything's empty.
	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		if(getEquipment(i) != EQUIPMENT_NONE)
		{
			break;
		}
	}
	// We found a filled slot, so that means there's at least one slot we can select randomly.
	if(i < COMBATANT_EQUIPMENT_SLOT_COUNT)
	{
		do
		{
			i = Mersenne::RandomInt() % COMBATANT_EQUIPMENT_SLOT_COUNT;
		} while(getEquipment(i) == EQUIPMENT_NONE);
	}
	// Otherwise, everything's empty, may as well fill the first slot
	else
	{
		i = 0;
	}
	// MORPH TARGET!
	setEquipment(i, Mersenne::RandomInt() % EQUIPMENT_TOTAL + 1);

	// It costs health to use this action.
	hurt(3);
}*/

void Combatant::morph(Combatant* target)
{
	int i;
	CombatDebug("Morphing!\n");

	// Morph variation that converts the player's swords into shields, shields into swords,
	// fire into ice, ice into fire, wind into earth, earth into wind.
	for(i = 0; i < COMBATANT_EQUIPMENT_SLOT_COUNT; i++)
	{
		switch(getEquipment(i))
		{
			case EQUIPMENT_FIRE_SWORD:
				setEquipment(i, EQUIPMENT_ICE_SHIELD);
				break;
			case EQUIPMENT_ICE_SWORD:
				setEquipment(i, EQUIPMENT_FIRE_SHIELD);
				break;
			case EQUIPMENT_WIND_SWORD:
				setEquipment(i, EQUIPMENT_EARTH_SHIELD);
				break;
			case EQUIPMENT_EARTH_SWORD:
				setEquipment(i, EQUIPMENT_WIND_SHIELD);
				break;
			case EQUIPMENT_FIRE_SHIELD:
				setEquipment(i, EQUIPMENT_ICE_SWORD);
				break;
			case EQUIPMENT_ICE_SHIELD:
				setEquipment(i, EQUIPMENT_FIRE_SWORD);
				break;
			case EQUIPMENT_WIND_SHIELD:
				setEquipment(i, EQUIPMENT_EARTH_SWORD);
				break;
			case EQUIPMENT_EARTH_SHIELD:
				setEquipment(i, EQUIPMENT_WIND_SWORD);
				break;
		}
	}

	// It costs health to use this action.
	hurt(4);
}


void Combatant::mutate()
{
	bool mutated;

	// Forbid disqualified gene mutations, recover from bad crossover too.
	do
	{
		mutated = false;
		// 50% chance of mutating an action
		if(Mersenne::RandomInt() % 2)
		{
			setAction(Mersenne::RandomInt() % COMBATANT_ACTION_SLOT_COUNT, Mersenne::RandomInt() % ACTION_TOTAL + 1);
			mutated = true;
		}
		// 50% chance of mutating an element or both an action and element.
		if(!mutated || Mersenne::RandomInt() % 2)
		{
			setStartingEquipment(Mersenne::RandomInt() % COMBATANT_EQUIPMENT_SLOT_COUNT, Mersenne::RandomInt() % EQUIPMENT_TOTAL + 1);
		}
	} while(isDisqualified());
}


void Combatant::crossover(Combatant* target)
{
	int i, a, b, tmp;
	Equipment eq;
	Action act;
	bool crossovered;

	// 50% chance of crossing over an action
	if(Mersenne::RandomInt() % 2)
	{
		a = Mersenne::RandomInt() % COMBATANT_ACTION_SLOT_COUNT;
		do
		{
			b = Mersenne::RandomInt() % COMBATANT_ACTION_SLOT_COUNT;
		} while(a == b);
		if(a > b)
		{
			tmp = a;
			a = b;
			b = tmp;
		}
		for(i = a; i < b; i++)
		{
			act = getAction(i);
			setAction(i, target->getAction(i));
			target->setAction(i, act);
		}
		crossovered = true;
	}
	// 50% chance of crossing over an element or both an action and element.
	if(!crossovered || Mersenne::RandomInt() % 2)
	{
		a = Mersenne::RandomInt() % COMBATANT_EQUIPMENT_SLOT_COUNT;
		do
		{
			b = Mersenne::RandomInt() % COMBATANT_EQUIPMENT_SLOT_COUNT;
		} while(a == b);
		if(a > b)
		{
			tmp = a;
			a = b;
			b = tmp;
		}
		for(i = a; i < b; i++)
		{
			eq = getStartingEquipment(i);
			setStartingEquipment(i, target->getStartingEquipment(i));
			target->setStartingEquipment(i, eq);
		}
	}
}


CombatResult ProcessCombat(Combatant* a, Combatant* b)
{
	CombatResult result;
	bool loseA, loseB = false;

	loseA = a->isDisqualified();
	loseB = b->isDisqualified();

	result.rounds = 0;
	if(!loseA && loseB)
	{
		result.outcome = OUTCOME_A_WIN;
		return result;
	}
	if(loseA && !loseB)
	{
		result.outcome = OUTCOME_B_WIN;
		return result;
	}
	if(loseA && loseB)
	{
		result.outcome = OUTCOME_DRAW;
		return result;
	}
	while(!a->isDead() && !b->isDead())
	{
		result.rounds++;
		if(Mersenne::RandomInt() % 2)
		{
			CombatDebug("A goes first\n");
			a->think(b);
			if(!a->isDead() && !b->isDead())
			{
				CombatDebug("B goes second\n");
				b->think(a);
			}
		}
		else
		{
			CombatDebug("B goes first\n");
			b->think(a);
			if(!a->isDead() && !b->isDead())
			{
				CombatDebug("A goes second\n");
				a->think(b);
			}
		}
	}
	if(a->isDead())
	{
		result.outcome = OUTCOME_A_WIN;
		return result;
	}
	if(b->isDead())
	{
		result.outcome = OUTCOME_B_WIN;
		return result;
	}
	result.outcome = OUTCOME_DRAW;
	return result;
}
