#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include"common.h"

static volatile uint32_t ticks;
int elapseTime=0;

void Time_Elapse() {
  ticks++;
}

void timeDisplay()
{
	elapseTime = ticks;
	printf("the elapsed time is %d\n",elapseTime);
}

ParserReturnVal_t CmdElapseTime(int mode)
{
 if(mode != CMD_INTERACTIVE) return CmdReturnOk;

 timeDisplay();
 

 return CmdReturnOk;
}
 ADD_CMD("elapse", CmdElapseTime, "displays the elapse time") 
