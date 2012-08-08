#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct element element;
struct element
{
	int age;
	char *name;
	element *nextAge;
	element *nextName;
};

typedef struct linkedList linkedList;
struct linkedList
{
	element *nameRoot;
	element *ageRoot;
};

int __ItemElementCompareByAge(element *ls, element *rs)
{
	/* Left side is older */
	if (ls->age > rs->age)
	{
		return 1;
	}
	/* Right side is older */
	if (ls->age < rs->age)
	{
		return -1;
	}
	/* The elements are the same age, so compare by name. */
	return strcmp(ls->name, rs->name);
}

int __ItemElementCompareByName(element *ls, element *rs)
{
	int cmp = strcmp(ls->name, rs->name);

	/* Names are not identical. */
	if (cmp)
	{
		return cmp;
	}

	/* The elements are the same name, so compare by age. */
		/* Left side is older */
		if (ls->age > rs->age)
		{
			return 1;
		}
		/* Right side is older */
		if (ls->age < rs->age)
		{
			return -1;
		}
	return 0;
}

void __InsertElementByAge (linkedList *list, element *item)
{
	element *current, *previous;
	if (list->ageRoot == NULL)
	{
		list->ageRoot = item;
		return;
	}
	if (__ItemElementCompareByAge(item, list->ageRoot) < 0)
	{
		item->nextAge = list->ageRoot;
		list->ageRoot = item;
		return;
	}

	previous = list->ageRoot;
	current = previous->nextAge;
	while (current != NULL && __ItemElementCompareByAge(item, current) >= 0)
	{
		previous = current;
		current = current->nextAge;
	}
	item->nextAge = current;
	previous->nextAge = item;
}


void __InsertElementByName (linkedList *list, element *item)
{
	element *current, *previous;
	if (list->nameRoot == NULL)
	{
		list->nameRoot = item;
		return;
	}
	if (__ItemElementCompareByName(item, list->nameRoot) < 0)
	{
		item->nextName = list->nameRoot;
		list->nameRoot = item;
		return;
	}

	previous = list->nameRoot;
	current = previous->nextName;
	while (current != NULL && __ItemElementCompareByName(item, current) >= 0)
	{
		previous = current;
		current = current->nextName;
	}
	item->nextName = current;
	previous->nextName = item;
}

void InsertElement(linkedList *list, element *item)
{
	__InsertElementByAge(list, item);
	__InsertElementByName(list, item);
}

element* FindElementByName(linkedList *list, char *name)
{
	element *elem;

	for (elem = list->nameRoot; elem != NULL; elem = elem->nextName)
	{
		if (!strcmp(elem->name, name))
		{
			return elem;
		}	
	}
	return NULL;
}

element* FindElementByAge(linkedList *list, int age)
{
	element *elem;

	for (elem = list->ageRoot; elem != NULL; elem = elem->nextAge)
	{
		if (elem->age == age)
		{
			return elem;
		}	
	}
	return NULL;
}

bool RemoveElement(linkedList *list, element *item)
{
	element *previous;
	element *current;

	if (item == NULL)
	{
		return false;
	}

	if (list->nameRoot != NULL)
	{
		if (list->nameRoot == item)
		{
			list->nameRoot = item->nextName;
		}
		else
		{
			previous = list->nameRoot;
			current = previous->nextName;
			while (current != NULL)
			{
				if (current == item)
				{
					previous->nextName = current->nextName;
					break;
				}
				previous = current;
				current = current->nextName;
			}
		}
	}
	if (list->ageRoot != NULL)
	{
		if (list->ageRoot == item)
		{
			list->ageRoot = item->nextAge;
		}
		else
		{
			previous = list->ageRoot;
			current = previous->nextAge;
			while (current != NULL)
			{
				if (current == item)
				{
					previous->nextAge = current->nextAge;
					break;
				}
				previous = current;
				current = current->nextAge;
			}
		}
	}
	free(item);
	return true;
}

void PrintListByAge(linkedList *list)
{
	element *elem = NULL;
	printf("List sorted by age:\n");
	for (elem = list->ageRoot; elem != NULL; elem = elem->nextAge)
	{
		printf("\t%s (%d years old)\n", elem->name, elem->age);
	}
	printf("\n");
}

void PrintListByName(linkedList *list)
{
	element *elem = NULL;
	printf("List sorted by name:\n");
	for (elem = list->nameRoot; elem != NULL; elem = elem->nextName)
	{
		printf("\t%s (%d years old)\n", elem->name, elem->age);
	}
	printf("\n");
}

char* AllocateString(char *value)
{
	char *s = (char *) malloc(sizeof(char) * (strlen(value) + 1));
	strcpy(s, value);
	return s;
}

element* AllocateElement(int age, char *name)
{
	element *p = (element *) malloc(sizeof(element));
	p->age = age;
	p->name = AllocateString(name);
	p->nextAge = NULL;
	p->nextName = NULL;
	return p;
}

void AddStructureInterface(linkedList *list)
{
	char name[256];
	char ageBuffer[256];
	int age;

	printf("Enter name:\n");
	fgets(name, 256, stdin);
	name[strlen(name) - 1] = '\0';

	printf("Enter age:\n");
	fgets(ageBuffer, 256, stdin);
	sscanf(ageBuffer, "%d", &age);

	InsertElement(list, AllocateElement(age, name));
}

void RemoveStructureInterface(linkedList *list)
{
	char name[256];

	printf("Enter name:\n");
	fgets(name, 256, stdin);
	name[strlen(name) - 1] = '\0';

	if (!RemoveElement(list, FindElementByName(list, name)))
	{
		printf("Could not find the entry named '%s'\n", name);
	}
}

void MainInterface(linkedList *list)
{
	bool done = false;
	int choice;
	char buffer[256];

	while (!done)
	{
		printf("1. Add structure\n");
		printf("2. Remove structure\n");
		printf("3. Print names\n");
		printf("4. Print ages\n");
		printf("5. Exit\n");

		printf("\n>> ");
		fgets(buffer, 256, stdin);
		sscanf(buffer, "%d", &choice);
		printf("\n");
		switch (choice)
		{
			case 1:
				AddStructureInterface(list);
				break;
			case 2:
				RemoveStructureInterface(list);
				break;
			case 3:
				PrintListByName(list);
				break;
			case 4:
				PrintListByAge(list);
				break;
			case 5:
				done = true;
				break;
			default:
				printf("Please input a valid choice!\n\n");
				break;
		}
	}
}

linkedList *CreateList()
{
	linkedList *list = malloc(sizeof(linkedList));
	return list;
}

void FreeList(linkedList *list)
{
	element *elem;
	element *p = NULL;
	if (list == NULL)
	{
		return;
	}
	elem = list->ageRoot;
	while (elem != NULL)
	{
		p = elem;
		elem = elem->nextAge;
		free(p);
	}
	free(list);
}

int main()
{
	linkedList *list = CreateList();
	MainInterface(list);
	FreeList(list);
	return 0;
}
