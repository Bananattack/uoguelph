#include <stdio.h>
#include "ParameterManager.h"

#define MANAGER_SIZE 10
#define MANAGER_REQUIRED 1
#define MANAGER_OPTIONAL 0

int main()
{
	ParameterManager* manager;
	char* name;
	char* information;
	int hp;
	int mp;
	ParameterList* skills;
	char* skill;
	float kill_ratio;
	Boolean is_boss;


	manager = PM_create(MANAGER_SIZE);
	if(!manager)
	{
		printf("Failed to initialize the manager\n");
		return -1;
	}
	PM_manage(manager, "name", STRING_TYPE, MANAGER_REQUIRED);
	PM_manage(manager, "information", STRING_TYPE, MANAGER_OPTIONAL);
	PM_manage(manager, "hp", INT_TYPE, MANAGER_REQUIRED);
	PM_manage(manager, "mp", INT_TYPE, MANAGER_OPTIONAL);
	PM_manage(manager, "skills", LIST_TYPE, MANAGER_REQUIRED);
	PM_manage(manager, "is_boss", BOOLEAN_TYPE, MANAGER_OPTIONAL);
	PM_manage(manager, "kill_ratio", REAL_TYPE, MANAGER_REQUIRED);

	if (!PM_parseFrom(manager, stdin, '#'))
	{
		printf("Failed to parse monster file\n");
		return -1;
	}

	name = PM_getValue(manager, "name").str_val;
	information = PM_hasValue(manager, "information") ? PM_getValue(manager, "information").str_val : "";
	hp = PM_getValue(manager, "hp").int_val;
	mp = PM_hasValue(manager, "mp") ? PM_getValue(manager, "mp").int_val : 0;
	skills = PM_getValue(manager, "skills").list_val;
	is_boss = PM_hasValue(manager, "is_boss") ? PM_getValue(manager, "is_boss").bool_val : false;
	kill_ratio = PM_getValue(manager, "kill_ratio").real_val;

	printf("%s loaded!\n", (is_boss) ? "Ferocious boss" : "Ordinary monster");
	printf("Name: %s\n", name);
	printf("Information: %s\n", (strlen(information)) ? information : "(none)");
	printf("HP: %d\n", hp);
	printf("MP: %d\n", mp);
	printf("Kill ratio: %f\n", kill_ratio);
	printf("Skills:\n");
	while(skill = PL_next(skills))
	{
		printf("\t%s\n", skill);
	}

	skills = PM_getValue(manager, "skills").list_val;
	while(skill = PL_next(skills))
	{
		printf("2\t%s\n", skill);
	}

	PM_destroy(manager);

	return 0;
}
