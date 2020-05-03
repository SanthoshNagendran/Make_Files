#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

int factorial( int x );
static int32_t a;
ParserReturnVal_t CmdFact(int action)
{

  if(action==CMD_SHORT_HELP) return CmdReturnOk;
  if(action==CMD_LONG_HELP) {
    printf("testasm\n\n"
	   "This command tests the C to assembler interface\n"
	   );

    return CmdReturnOk;
  }
  fetch_int32_arg(&a);
  printf("the factorial of %ld is %d\n",a,factorial( a  ) );

  return CmdReturnOk;
}

ADD_CMD("fact",CmdFact,"factorial of a number")
