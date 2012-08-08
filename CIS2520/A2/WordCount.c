/*
 * ------------------------------------------------------------------
 *  WordCount.c - Word counting application
 *      - SPECIFICATION:
 *          o counts words separated by blanks or end-of-line
 *          o does not count words that begin with '*'
 *          o zeros current count of word beginning with '-'
 *          o assumes '*' and '-' never occur in isolation
 * ------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "adt/Table.h"

#define QUIT_STRING "quit"
#define IGNORE_CHAR '*'
#define DELETE_CHAR '-'
#define DELIM " \n\t"
#define BUF_SIZE 1024
#define INIT_TABLE 11
#define MAX_CHAR_HASH 15


/*
 * prototypes for hashing/comparison functions
 */
int stringhash   (void *);
int stringequals (void *, void*);


int main(int argc, char *argv[])
{
    FILE *fp_in = NULL;         /* source file for word count */
    char filename[BUF_SIZE];    /* name of source file */
    Table *symtable;            /* symbol table - a Table ADT */
    char inbuf[BUF_SIZE];       /* buffered line of input */
    char *word;                 /* individual words (keys for Table) */
    char *new_word;             /* new word allocation */
    int *count;                 /* word count from/for table entries */ 

    printf("\n- Word Count Utility -");

    /* obtain a file name that exists */
    while (!fp_in)
    {
        printf("\nEnter file name (\"quit\" to exit): ");
        fgets(filename, BUF_SIZE, stdin);
        filename[strlen(filename)-1] = '\0';  /* remove newline */

        if (strcmp(filename,QUIT_STRING) == 0)
            exit(0);

        if (!(fp_in = fopen(filename,"r")))
        {
            fprintf(stderr,">>> cannot open %s <<<", filename);
            continue;
        }

        if (!(symtable = tableCreate(INIT_TABLE, stringhash, stringequals)))
        {
            fprintf(stderr,"\n>>> fatal error allocating table <<<\n");
            exit(1);
        }

        while(fgets(inbuf,BUF_SIZE,fp_in))
        {
            if ((word = strtok(inbuf,DELIM)))
            {
                while(word)
                {
                    if (word[0] == DELETE_CHAR)
                    {
                        /*
                         * delete symbol if it exists
                         */
                        tableRemoveEntry(symtable,&word[1]);
                    }
                    else if (word[0] != IGNORE_CHAR)
                    {
                        /*
                         * if symbol already in table then incremenet
                         * count, otherwise initial count is 1
                         */
                        if ((count = (int *)tableGetEntry(symtable,word)))
                        {
                            (*count)++;
                        }
                        else
                        {
                            if (!(count = malloc(sizeof(int))))
                            {
                                fprintf(stderr, ">>> fatal: memory <<<\n");
                                exit(1);
                            }
                            new_word = strdup(word);
                            *count = 1;
                            tableAddEntry(symtable,new_word,count);
                        }

                    }
                    /*
                     * otherwise first char was IGNORE_CHAR
                     */

                    word = strtok(NULL,DELIM);
                }
            }
        }

        fclose(fp_in);

        /*
         * output resulting table with word counts (not
         * necessarily ordered of course)
         */
        printf("\nWord count for %s:\n", filename);

        tableWrite(symtable, stdout);
        printf("\n");

        printf("Summary:\n--------\n");
        printf("table entries = %d\n", tableEntries(symtable));
        printf("table size = %d\n", tableSize(symtable));
        printf("load factor = %f\n", tableLoad(symtable));
    }

    /*
     * to be "clean", we should be going through deleting all keys and
     * data members, but c'est la vie
     */
    tableDelete(symtable);

    return(0);

} /* WordCount utility program */ 



int stringhash(void *key)
{
    int i;              /* loop index */
    char *str;          /* character reference to key */
    int val = 0;        /* hashed string value */

    str = key;

    for (i = 0; (i < MAX_CHAR_HASH) && (str[i]); i++)
        val += (int)str[i];

    return(val);

} /* stringhash */



int stringequals(void *key1, void *key2)
{
    return(!strcmp(key1,key2));

} /* stringequals */
