#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

static int32_t value,step,delay,angle;

void steppermotorinit (void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Pin = (GPIO_PIN_3| GPIO_PIN_4 |GPIO_PIN_5 |				GPIO_PIN_8);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = (GPIO_PIN_14); 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	DACinitialization(); //used for VREF pin from PA4
	timerinit();//used for delay functions

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,1); //RST set high
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,1); //PS pin
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,1); //OE off
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0); //step pin
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,1); //FR high clockwise
	printf("Stepper motor initialised successfully\n");
}
void stepperenable(int32_t value) 
{
	if(value)
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,0); //OE ON
	//printf("Output Enabled\n");
	}
	else
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,1); //OE off
	//printf("Output disabled\n");
	}
return;
}
void Delay(int32_t delayVal)
{
	TIM17->CNT = 0;       /* Reset counter */
	while(TIM17->CNT < delayVal) 
	{
	asm volatile ("nop\n");
	}	
}
void Step(int32_t step,int32_t delay)
{
 int32_t i;
	if(step<0)
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,1);
	step=-step;
	}
	else
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,0);
	}
	for(i=0;i<step;i++)
	{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
	Delay(delay/2);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
	Delay(delay/2);
	}
}
void Angle(int32_t angle,int32_t delay)
{
 int32_t i;
	step = angle*4.44;
	if(step<0)
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,1);
	step=-step;
	}
	else
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,0);
	}
	for(i=0;i<step;i++)
	{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,1);
	Delay(delay/2);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,0);
	Delay(delay/2);
	}
return;
}
ParserReturnVal_t Cmdstepperinit(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	steppermotorinit ();
  return CmdReturnOk;
}

ADD_CMD("stepperinit",Cmdstepperinit,"            Initialies the motor funcs")
ParserReturnVal_t Cmdstepperenable(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&value);
	stepperenable (value);
  return CmdReturnOk;
}

ADD_CMD("stepperenable",Cmdstepperenable,"            Enables the motor")
ParserReturnVal_t Cmdstep(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&step);
	fetch_int32_arg(&delay);
	Step (step,delay);
  return CmdReturnOk;
}

ADD_CMD("step",Cmdstep,"            Enables the motor")
ParserReturnVal_t Cmdstepangle(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&angle);
	fetch_int32_arg(&delay);
	Angle (angle,delay);
  return CmdReturnOk;
}

ADD_CMD("angle",Cmdstepangle,"            Enables the motor")
