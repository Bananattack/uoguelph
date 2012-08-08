/*
	CIS*3110 Assignment 1
	Memory Measurement!
	Andrew Crowell
	0545826
*/
#include <stdio.h>
#include <setjmp.h> 
#include <signal.h>
 
/* Used to recover from segmentation faults and escape from a violating program */
jmp_buf mem_test_env;
/* Denotes how much memory is accessible by this program */
int mem_accessible;
/* Denotes how much accessible memory is read-only */
int mem_read_only;
 
/*
	A signal handler called upon failure to access a block of memory,
	which essentially jumps to a sigsetjmp() call prior to the
	violation.
	Arguments:	none
	Return:		none
*/
void MemTestFailure()
{
	siglongjmp(mem_test_env, 1);
}

/*
	Performs tests on the address stored in p.
	Arguments:	an unsigned address index p
	Return:		0 if invalid address
			1 if read-only address
			2 if fully accessible address

*/
int MemTestAddress(unsigned int p)
{
	int v;

	/* Set the handlers */
	signal(SIGSEGV, MemTestFailure);
	signal(SIGBUS, MemTestFailure);

	/* Going to test for readable memory, if it's inaccessible jump here */
	if (sigsetjmp(mem_test_env, 1) == 1)
	{
		return 0;
	}
	
	/* Try to read */
	v = *(int*) p;

	/* Going to test for writable memory, if it's read-only jump here */
	if (sigsetjmp(mem_test_env, 1) == 1)
	{
		return 1;
	}

	/* Write the value we read out of this address and stuff it back in. 
	   This prevents undesirable behaviour like overwriting the
	   memory counter. */
	*(int*) p = v;
	return 2;
}
 
/*
	Attempts to reference all the address-space of the active process
	and measures how much memory is actual associated with the process.
	Assumes:	32-bit architecture with word size of 4 bytes.
	Arguments:	reference to size (accessible memory)
			reference to rom (read-only memory)
	Return:		none
*/
void memory(int* size, int* rom)
{
	unsigned int i = 0;
	
	mem_read_only = 0;
	mem_accessible = 0;
	/* Loop until we overflow the counter and end up where we started */
	do
	{
		switch(MemTestAddress(i))
		{
			case 1:
				mem_read_only += 4;
				/* intentional fall-through, read-only memory is also accessible. It's just not writable. */
			case 2:
				mem_accessible += 4;
				break;
		}
		i += 4;
	} while(i != 0);

	*rom = mem_read_only;
	*size = mem_accessible;
}
