#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

static int32_t Counter;
static int32_t Temp;

void TaskCounter(void)
{
	if(Counter!=0)
	{
		printf("The down Count is %ld\n",Counter);
		Counter--;
	}
}
void Increment()
{
	if(Counter==0)
	{
		if(Temp<10)
		{
			Temp++;
			printf("The up Count is %ld\n",Temp);
		}
	}
}


ParserReturnVal_t CmdCount(int mode)
	{
  
	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&Counter);
	Temp=Counter;
	return CmdReturnOk;
	}

ADD_CMD("count",CmdCount,"            Count Value")


