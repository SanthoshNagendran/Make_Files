/* mytest.c:
 *
 *  Test C to assembly interface 
 */

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"


int mytest( int x );
static int32_t a;
ParserReturnVal_t CmdTest(int action)
{

  if(action==CMD_SHORT_HELP) return CmdReturnOk;
  if(action==CMD_LONG_HELP) {
    printf("testasm\n\n"
	   "This command tests the C to assembler interface\n"
	   );

    return CmdReturnOk;
  }
  printf("ret val = %d\n", mytest( 77  ) );

  return CmdReturnOk;
}

ADD_CMD("cmdtest",CmdTest,"Test C to asm")
ParserReturnVal_t Cmdsanthosh(int action)
{
  if(action==CMD_SHORT_HELP) return CmdReturnOk;
  if(action==CMD_LONG_HELP) {
    printf("testasm\n\n"
	   "This command tests the C to assembler interface\n"
	   );

    return CmdReturnOk;
  }
  printf("Hi there\n");
fetch_int32_arg(&a);
printf("The square of value you entered is %ld\n",a*a);
  return CmdReturnOk;
}

ADD_CMD("santhosh",Cmdsanthosh,"Hellow world")

// simple function that toggles the USR LED the STM Nucleo64 board
void USR_LED_Toggle( void )
{
  static uint32_t LedState = 0;		// keep track of the led state in a private variable

  LedState = LedState ? 0 : 1;		// toggle the led state value
  HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,LedState);

  return;
}
