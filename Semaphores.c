#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "common.h"

#define MAX_SEMAPHORES 10

typedef enum SemaphoreState_e 
{
	SEMAPHORE_FREE,
	SEMAPHORE_ALLOCATED,
} SemaphoreState_t;

typedef struct Semaphore_s 
{
	SemaphoreState_t state; /* State of semaphore */
	int32_t count;          /* Semaphore Count */	
	int32_t taskID;         /* Task ID of task */
	char *name;             /* Name of semaphore */
} Semaphore_t;Semaphore_t Semaphores[MAX_SEMAPHORES];

void SemaphoreInit(void)
{
uint32_t i;/* Initialize all semaphores to free */
	for(i=0; i<MAX_SEMAPHORES; i++) 
	{
		Semaphores[i].state = SEMAPHORE_FREE;
		Semaphores[i].count  = 0;
		Semaphores[i].taskID = -1;
		Semaphores[i].name   = NULL;
	}
printf("Semaphores are initialized\n");
}

uint32_t SemaphoreNew(int32_t count,char *name)
{
uint32_t i;
uint32_t *semaphoreHandle;
/* Find an unallocated semaphore */
	for(i = 0; i<MAX_SEMAPHORES; i++) 
	{
		if(Semaphores[i].state == SEMAPHORE_FREE) 			{
		Semaphores[i].state  = SEMAPHORE_ALLOCATED;
		Semaphores[i].count  = count;
		Semaphores[i].taskID = -1;
		Semaphores[i].name   = name;
		*semaphoreHandle = i;
		printf("semaphore allocated with count %ld and name %s with semaphorehandle %ld\n",Semaphores[i].count,Semaphores[i].name,i);
		return 0;
		}
	}

/* No semaphores left */
printf("Error: Out of semaphores to allocate ""in SemaphoreNew()\n");
return 1;
}

uint32_t SemaphorePost(uint32_t semaphoreHandle)
{
	if(semaphoreHandle >= MAX_SEMAPHORES) 
	{
	printf("Error: semaphoreHandle is out ""of range in SemaphorePost()\n");
	return 1;
	}
	if(Semaphores[semaphoreHandle].state == SEMAPHORE_FREE) 	{
	printf("Error: trying to post to an ""unallocated ""semaphore in SemaphorePost()\n");
	return 1;
	}	
/* Post the semaphore */
Semaphores[semaphoreHandle].count++;
/* Can we unblock a task now */	
printf("value of taskid is %ld and value of count is %ld\n",Semaphores[semaphoreHandle].taskID,Semaphores[semaphoreHandle].count);				if((Semaphores[semaphoreHandle].count > 0)&& (Semaphores[semaphoreHandle].taskID != -1)) 
	{
	/* Yes, count is greater than 0, 
	* and there is a task waiting*/
	Semaphores[semaphoreHandle].count--;
	//TaskReady(Semaphores[semaphoreHandle].taskID);
	printf("task with id %ld is set ready\n",Semaphores[semaphoreHandle].taskID);
	Semaphores[semaphoreHandle].taskID = -1;
	}
return 0;
}

uint32_t SemaphoreAcquire(uint32_t semaphoreHandle)
{
	if(semaphoreHandle >= MAX_SEMAPHORES) 
	{
	printf("Error: semaphoreHandle is out of ""range in SemaphoreAcquire()\n");
	return 1;
	}
	if(Semaphores[semaphoreHandle].state ==SEMAPHORE_FREE) 		{
	printf("Error: trying to acquire an ""unallocated ""semaphore in SemaphoreAcquire()\n");
	return 1;
	}
	if(Semaphores[semaphoreHandle].taskID != -1) 
	{
	printf("Error: There is a task already waiting ""on the semaphore in ""SemaphoreAcquire()\n");
	return 1;
	}
/* Check if we need to block */Semaphores[semaphoreHandle].count--;
printf("value of taskid is %ld and value of count is %ld\n",Semaphores[semaphoreHandle].taskID,Semaphores[semaphoreHandle].count);
	if(Semaphores[semaphoreHandle].count <= 0) 
	{
	/* Yes, there are no counts left */ 
	//Semaphores[semaphoreHandle].taskID =TaskCurrent();
	//TaskPending(TaskCurrent());
	printf("task with id %ld is set pending\n",Semaphores[semaphoreHandle].taskID);
	return 1;
	}
return 0;
}

ParserReturnVal_t Cmdsemainit(int mode)
{
  	if(mode != CMD_INTERACTIVE) 
	return CmdReturnOk;
	SemaphoreInit();
  	return CmdReturnOk;
}
ADD_CMD("semaphoreinit",Cmdsemainit,"            Semaphores")

ParserReturnVal_t CmdsemaAllocate(int mode)
{
char *name;
int32_t count;
  	if(mode != CMD_INTERACTIVE) 
	return CmdReturnOk;
	fetch_int32_arg(&count);
	fetch_string_arg(&name);
	SemaphoreNew(count,name);
  	return CmdReturnOk;
}
ADD_CMD("semaphoreallocate",CmdsemaAllocate,"        Semaphores")

ParserReturnVal_t CmdsemaPost(int mode)
{
uint32_t semaphoreHandle;
  	if(mode != CMD_INTERACTIVE) 
	return CmdReturnOk;
	fetch_uint32_arg(&semaphoreHandle);
	SemaphorePost(semaphoreHandle);
  	return CmdReturnOk;
}
ADD_CMD("semaphorepost",CmdsemaPost,"        Semaphores")

ParserReturnVal_t CmdsemaAcquire(int mode)
{
uint32_t semaphoreHandle;
  	if(mode != CMD_INTERACTIVE) 
	return CmdReturnOk;
	fetch_uint32_arg(&semaphoreHandle);
	SemaphoreAcquire(semaphoreHandle);
  	return CmdReturnOk;
}
ADD_CMD("semaphoreacquire",CmdsemaAcquire,"        Semaphores")
