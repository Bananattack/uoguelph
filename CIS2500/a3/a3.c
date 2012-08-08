/*
	Assignment #3 - Structures on Disk

	Andrew Crowell
	0545826
	
*/

/* External dependencies */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
	Info related to a employees for a company.
*/
struct employeeRecord
{
	char* name;
	char* address;
	short addressLength, nameLength;
	int phoneNumber;
};

/*
	Saves a record to disk.

	Pass: file pointer, record to save
*/
void WriteRecord(FILE *f, struct employeeRecord *record)
{
	fwrite(record, sizeof(struct employeeRecord), 1, f);
	fwrite(record->name, sizeof(char), record->nameLength, f);
	fwrite(record->address, sizeof(char), record->addressLength, f);
}

/*
	Reads a record from disk and stores it in memory.

	Pass: file pointer
	Return: record
*/
struct employeeRecord* ReadRecord(FILE *f)
{
	struct employeeRecord *record = malloc(sizeof(struct employeeRecord));
	fread(record, sizeof(struct employeeRecord), 1, f);
	if (feof(f))
	{
		free(record);
		return NULL;
	}
	return record;
}

/*
	Reads a string of a specific length from a file,
	and stores it in memory.

	Pass: file pointer, string length
	Return: string
*/
char* ReadString(FILE *f, int length)
{
	char *s = malloc(sizeof(char) * (length + 1));
	fread(s, sizeof(char), length, f);
	s[length] = '\0';
	if (feof(f))
	{
		free(s);
		return NULL;
	}
	return s;
}

/*
	Prompts the user for information,
	and storing that in a record, and then to disk.

	Pass: name of file to save to
*/
void NewRecordMenu(char *filename)
{
	struct employeeRecord *record;
	char buffer[1024];
	FILE *f;

	record = malloc(sizeof(struct employeeRecord));

	/* Prompt for a name */
	printf("Enter the name: ");
	fgets(buffer, 1024, stdin);

	/* Remove the \n from the string */
	buffer[strlen(buffer) - 1] = '\0';

	/* Initialize the name length */
	record->nameLength = strlen(buffer);

	/* Initialize the name */
	record->name = malloc(sizeof(char) * (record->nameLength + 1));
	strcpy(record->name, buffer);

	/* Prompt for an address */
	printf("Enter the address: ");
	fgets(buffer, 1024, stdin);

	/* Remove the \n from the string */
	buffer[strlen(buffer) - 1] = '\0';

	/* Initialize the address length */
	record->addressLength = strlen(buffer);

	/* Initialize the address */
	record->address = malloc(sizeof(char) * (record->addressLength + 1));
	strcpy(record->address, buffer);

	/* Prompt for telephone number */
	printf("Enter telephone number: ");
	fgets(buffer, 1024, stdin);

	/* Convert input to int and initialize phone number */
	sscanf(buffer, "%d", &(record->phoneNumber));


	/* Write the record to disk */
	f = fopen(filename, "a");
	WriteRecord(f, record);
	fclose(f);

	free(record);
}

/*
	Prints a record out on the screen.

	Pass: record to print out
*/
void PrintRecord(struct employeeRecord *record)
{
	printf("Name: %s\n", record->name);
	printf("Address: %s\n", record->address);
	printf("Phone Number: %d\n", record->phoneNumber);
}

/*
	Prompts the user for a name to search for.
	Performs a case-sensitive string search
	for a record by that name on disk, and prints out
	related data when found, or a failure message
	if not found.

	Pass: name of file to load from
*/
void SearchByNameMenu(char *filename)
{
	FILE *f;
	struct employeeRecord *record;
	char buffer[1024];
	bool found = false;

	f = fopen(filename, "r");
	if (!f)
	{
		printf("File not found. Add some records first!\n");
		return;
	}

	printf("Enter the name: ");
	fgets(buffer, 1024, stdin);

	/* Remove the \n from the string */
	buffer[strlen(buffer) - 1] = '\0';

	while(!feof(f) && !found)
	{
		record = ReadRecord(f);
		if (record == NULL)
		{
			break;
		}

		record->name = ReadString(f, record->nameLength);
		if (record->name != NULL)
		{
			if(!strcmp(buffer,record->name))
			{
				found = 1;
				break;
			}
			free(record->name);
		}
		fseek(f, sizeof(char) * record->addressLength, SEEK_CUR);
		free(record);
	}

	if (found)
	{
		printf("Record found!\n");

		record->address = ReadString(f, record->addressLength);
		PrintRecord(record);

		fclose(f);
		free(record->name);
		free(record);
		return;
	}

	fclose(f);
	printf("Record not found in file.\n");
}

/*
	Prompts the user for an index to search for.
	Iterates through records stored on-disk and prints out
	related data when index is found, or a failure message
	if not found.

	Pass: name of file to load from
*/
void SearchByIndexMenu(char *filename)
{
	FILE *f;
	struct employeeRecord *record;
	char buffer[1024];
	int index, count = 0;
	bool found = false;

	f = fopen(filename, "r");
	if (!f)
	{
		printf("File not found. Add some records first!\n");
		return;
	}

	printf("Enter a record number: ");
	fgets(buffer, 1024, stdin);
	sscanf(buffer, "%d", &index);

	while(!feof(f) && !found)
	{
		record = ReadRecord(f);
		if (record == NULL)
		{
			break;
		}
		if(count == index)
		{
			found = 1;
			break;
		}

		fseek(f, sizeof(char) * (record->nameLength + record->addressLength), SEEK_CUR);
		free(record);
		count++;
	}

	if (found)
	{
		printf("Record found!\n");

		record->name = ReadString(f, record->nameLength);
		record->address = ReadString(f, record->addressLength); 
		PrintRecord(record);

		fclose(f);
		free(record->name);
		free(record->address);
		free(record);
		return;
	}

	fclose(f);
	printf("Couldn't find the record index %d. There are only %d records.\n", index, count);
}

/*
	Prompts the user for a choice related to
	the program's capabilities.

	Pass: name of file to interface with.
*/
void MainMenu(char *filename)
{
	int done = 0;
	char buffer[1024];
	int choice;
	while (!done)
	{
		choice = 0;
		printf("1. Enter a new record.\n");
		printf("2. Retrieve a record by name.\n");
		printf("3. Retrieve a record by number.\n");
		printf("4. Exit\n");
		printf("\nEnter a number from 1-4: ");
		fgets(buffer, 1024, stdin);
		sscanf(buffer, "%d", &choice);
		printf("\n");
		switch (choice)
		{
			case 1:
				NewRecordMenu(filename);
				break;			
			case 2:
				SearchByNameMenu(filename);
				break;
			case 3:
				SearchByIndexMenu(filename);
				break;
			case 4:
				done = 1;
				break;
		}
		printf("\n");
	}
}


/*
	The embodiment of all awesomeness, created with epic win.
	Returns: 0 upon success, something else that's bad on failure.
*/
int main()
{
	MainMenu("employeeRecords");
	return 0;
}
