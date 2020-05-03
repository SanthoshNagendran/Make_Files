/*******************************************************************************************
* File Name          : Assignment 2.c
* Description        : Assignment 2: Task executive, process management
*					   This program uses function pointer to hold and print a function holding a data.
*					   The users can add tasks, kill them, assign some states for them and switch between the tasks
*					   consist of maximum 10 tasks.
*
* Author:              Santhosh Nagendran
* Date:                10-02-2019
********************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

static int32_t killValue,AddValue,dataValue,pendValue,readyValue;
static int32_t currentTask;
typedef enum{
	Task_ready ,
	Task_pending,
	Task_running,
	Task_inactive,
	Task_interrupted,
	Task_state_max
}TaskState_e;

typedef struct Task_s {
	void (*f)(void *data); /* Task function */
	void *data;  /* Private data pointer for this task */
	TaskState_e state;
}Task_t;

Task_t Array[10];
// FUNCTION      : sum()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void sum(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task 1 is running with data %ld\n",myValue);
	return;
}
// FUNCTION      : mul()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void mul(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task 2 is running with data %ld\n",myValue);
	return;
}
// FUNCTION      : task3()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void task3(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task 3 is running with data %ld\n",myValue);
	return;
}
// FUNCTION      : task4()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void task4(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task 4 is running with data %ld\n",myValue);
	return;
}
// FUNCTION      : task5()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void task5(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task 5 is running with data %ld\n",myValue);
	return;
}
// FUNCTION      : task6()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void task6(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task 6 is running with data %ld\n",myValue);
	return;
}
// FUNCTION      : task7()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void task7(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task 7 is running with data %ld\n",myValue);
	return;
}
// FUNCTION      : task8()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void task8(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task 8 is running with data %ld\n",myValue);
	return;
}
// FUNCTION      : task9()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void task9(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task 9 is running with data %ld\n",myValue);
	return;
}
// FUNCTION      : task10()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void task10(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task 10 is running with data %ld\n",myValue);
	return;
}
// FUNCTION      : inactive()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void inactive(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task %ld is inactive\n",myValue);
	return;
}
// FUNCTION      : pending()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void pending(void *a)
{
	int32_t myValue = (int32_t)a;
	printf("task %ld is in pending state\n",myValue);
	return;
}
// FUNCTION      : ready()
// DESCRIPTION   : This function takes an input of Data and prints the same 
// PARAMETERS    : void *a	- variable to store address of a which carries a value  
// RETURNS       : nothing
void ready(void *a)
{
	int32_t myValue = (int32_t)a;
	Array[myValue].state = myValue;
	printf("task %ld is ready\n",myValue);
	return;
}
/* Find the next task to run */
// FUNCTION      : tasknext()
// DESCRIPTION   : This function is used to point the next runnable task in the array list
// PARAMETERS    : nothing  
// RETURNS       : i - value that has the next task pointed to.
static int32_t TaskNext(void)
{
	int32_t i=0;
	uint32_t count=0;
	i = currentTask;
	do 
		{
		i = (i + 1) % 10;
		count++;
		} while((Array[i].f == NULL) && 			(Array[i].state != Task_ready)
			&& (count <= 10));
	return (count <= 10) ? i : -1;
}
// FUNCTION      : taskshow()
// DESCRIPTION   : This function is used to display all the list of tasks with the position and data in it. 
// PARAMETERS    : nothing 
// RETURNS       : nothing
void TaskShow()
{
	for(int i=0;i<10;i++)
	{
		if(Array[i].f != NULL)
		{
		(*Array[i].f)(Array[i].data);
		}
		else
		printf("No task available in this list\n");
	}
}
// FUNCTION      : taskswitcher()
// DESCRIPTION   : This function used to switch between tasks 
// PARAMETERS    : nothing 
// RETURNS       : nothing
void TaskSwitcher()
{
	currentTask = TaskNext();
	Array[currentTask].f(Array[currentTask].data);
}
// FUNCTION      : taskkill()
// DESCRIPTION   : This function is used to assign a particular task to an inactive state 
// PARAMETERS    : nothing 
// RETURNS       : nothing
void TaskKill()
{
	for(int i=0;i<=10;i++)
	{
		if(i==(killValue-1))
		{
		Array[i].f    = inactive;
		Array[i].data = (void *)i+1;
		printf("task %ld has been killed\n",killValue);
		}
	}
printf("The new list is\n");
TaskShow();
}
// FUNCTION      : taskpending()
// DESCRIPTION   : This function makes a certian task to a pending state 
// PARAMETERS    : none  
// RETURNS       : nothing
void TaskPending()
{
	for(int i=0;i<=10;i++)
	{
		if(i==(pendValue-1))
		{
		Array[i].f    = pending;
		Array[i].data = (void *)i+1;
		}
	}
printf("The new list is\n");
TaskShow();
}
// FUNCTION      : taskready()
// DESCRIPTION   : This function makes a particular task to a ready state 
// PARAMETERS    : none  
// RETURNS       : nothing
void TaskReady()
{
	for(int i=0;i<=10;i++)
	{
		if(i==(readyValue-1))
		{
		Array[i].f    = ready;
		Array[i].data = (void *)i+1;
		}
	}
printf("The new list is\n");
TaskShow();
}
// FUNCTION      : taskkillall()
// DESCRIPTION   : This function will make all the tasks to inactive state 
// PARAMETERS    : none  
// RETURNS       : nothing
void TaskKillAll()
{
	for(int i=0;i<10;i++)
	{
	Array[i].f    = inactive;
	Array[i].data = (void *)i+1;
	}
printf("All the tasks are killed\n");
printf("The new list is\n");
TaskShow();
}
// FUNCTION      : taskadd()
// DESCRIPTION   : This function will make a particular task to add with a function and a data 
// PARAMETERS    : none  
// RETURNS       : nothing
int32_t TaskAdd()
{
	if(AddValue==1)
		{Array[0].f    = sum;
		Array[0].data = (void *)dataValue;
		printf("task %ld has been added successfully\n",AddValue);}
	else if(AddValue==2)
		{Array[1].f    = mul;
		Array[1].data = (void *)dataValue;
		printf("task %ld has been added successfully\n",AddValue);}
	else if(AddValue==3)
		{Array[2].f    = task3;
		Array[2].data = (void *)dataValue;
		printf("task %ld has been added successfully\n",AddValue);}
	else if(AddValue==4)
		{Array[3].f    = task4;
		Array[3].data = (void *)dataValue;
		printf("task %ld has been added successfully\n",AddValue);}
	else if(AddValue==5)
		{Array[4].f    = task5;
		Array[4].data = (void *)dataValue;
		printf("task %ld has been added successfully\n",AddValue);}
	else if(AddValue==6)
		{Array[5].f    = task6;
		Array[5].data = (void *)dataValue;
		printf("task %ld has been added successfully\n",AddValue);}
	else if(AddValue==7)
		{Array[6].f    = task7;
		Array[6].data = (void *)dataValue;
		printf("task %ld has been added successfully\n",AddValue);}
	else if(AddValue==8)
		{Array[7].f    = task8;
		Array[7].data = (void *)dataValue;
		printf("task %ld has been added successfully\n",AddValue);}
	else if(AddValue==9)
		{Array[8].f    = task9;
		Array[8].data = (void *)dataValue;
		printf("task %ld has been added successfully\n",AddValue);}
	else if(AddValue==10)
		{Array[9].f    = task10;
		Array[9].data = (void *)dataValue;
		printf("task %ld has been added successfully\n",AddValue);}
	else
		{printf("No slots available\n");}

return -1;
}
// FUNCTION      : taskaddall()
// DESCRIPTION   : This function will add all the tasks with different assigned functions with same data value 
// PARAMETERS    : none  
// RETURNS       : nothing
int32_t TaskAddAll()
{
		Array[0].f    = sum;
		Array[0].data = (void *)dataValue;
		Array[1].f    = mul;
		Array[1].data = (void *)dataValue;
		Array[2].f    = task3;
		Array[2].data = (void *)dataValue;
		Array[3].f    = task4;
		Array[3].data = (void *)dataValue;
		Array[4].f    = task5;
		Array[4].data = (void *)dataValue;
		Array[5].f    = task6;
		Array[5].data = (void *)dataValue;
		Array[6].f    = task7;
		Array[6].data = (void *)dataValue;
		Array[7].f    = task8;
		Array[7].data = (void *)dataValue;
		Array[8].f    = task9;
		Array[8].data = (void *)dataValue;
		Array[9].f    = task10;
		Array[9].data = (void *)dataValue;
		printf("All the tasks have been added successfully with same data value\n");
return -1;
}

/* Parser commands to access to the differnt functions mentioned above */

ParserReturnVal_t CmdTaskShow(int mode)
{
 	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	TaskShow();
  	return CmdReturnOk;
}
ADD_CMD("taskshow",CmdTaskShow,"            Task_Executive")

ParserReturnVal_t CmdTaskadd(int mode)
{
  	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&AddValue);
	fetch_int32_arg(&dataValue);
	TaskAdd();
  	return CmdReturnOk;
}
ADD_CMD("taskadd",CmdTaskadd,"            Task_Executive")

ParserReturnVal_t CmdTaskKill(int mode)
{
  	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&killValue);
	TaskKill();
  	return CmdReturnOk;
}
ADD_CMD("taskkill",CmdTaskKill,"            Task_Executive")

ParserReturnVal_t CmdTaskSwitcher(int mode)
{
  	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	TaskSwitcher();
  	return CmdReturnOk;
}
ADD_CMD("taskswitch",CmdTaskSwitcher,"            Task_Executive")

ParserReturnVal_t CmdTaskaddall(int mode)
{
  	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&dataValue);
	TaskAddAll();
  	return CmdReturnOk;
}
ADD_CMD("taskaddall",CmdTaskaddall,"           Task_Executive")

ParserReturnVal_t CmdTaskkillall(int mode)
{
 	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	TaskKillAll();
  	return CmdReturnOk;
}
ADD_CMD("taskkillall",CmdTaskkillall,"      Task_Executive")

ParserReturnVal_t CmdTaskpend(int mode)
{

  	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&pendValue);
	TaskPending();
  	return CmdReturnOk;
}
ADD_CMD("taskpend",CmdTaskpend,"            Task_Executive")

ParserReturnVal_t CmdTaskready(int mode)
{
  	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&readyValue);
	TaskReady();
  	return CmdReturnOk;
}
ADD_CMD("taskready",CmdTaskready,"          Task_Executive")	
