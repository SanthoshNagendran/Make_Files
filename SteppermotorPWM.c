#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

static int32_t value,step,delay;
TIM_HandleTypeDef tim15;
void steppermotorinit1 (void)
{
	__HAL_RCC_TIM15_CLK_ENABLE();
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
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//alternate function mode
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 1;//need to look at the data sheet for the correct alternate function value
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	DACinitialization(); //used for VREF pin from PA4
	timerinit();//used for delay functions
/* this is to initialise pwm*/	
	tim15.Instance = TIM15;
	tim15.Init.Prescaler = HAL_RCC_GetPCLK2Freq() / 1000000;
	tim15.Init.CounterMode   = TIM_COUNTERMODE_UP;
	tim15.Init.Period        = 1000;
	tim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim15.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&tim15);
	TIM_OC_InitTypeDef sConfig;
	sConfig.OCMode       = TIM_OCMODE_PWM1;
	sConfig.Pulse        = 0;
	sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	sConfig.OCNPolarity  = TIM_OCNPOLARITY_LOW;
	sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
	sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
        HAL_TIM_PWM_ConfigChannel(&tim15,&sConfig,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&tim15,TIM_CHANNEL_1);
	printf("pwm initialised successfully\n");
	

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,1); //RST set high
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,1); //PS pin
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,1); //OE off
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,1); //FR high clockwise
	printf("Stepper motor initialised successfully\n");
}
void stepperenable1(int32_t value) 
{
	if(value)
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,0); //OE ON
	printf("Output Enabled\n");
	}
	else
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,1); //OE off
	printf("Output disabled\n");
	}
}
void Step1(int32_t step,int32_t delay)
{
	if(step>100)
	{
	printf("the steps should be  -100 .. 100 and setting it to 100\n");
	step=100;
	}
	if(step<0)
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,1);
	step=-step;
	}
	else
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,0);
	}
	HAL_TIM_PWM_Start(&tim15,TIM_CHANNEL_1);
	step = 1485-(step*13.5);
	TIM15->CCR1 = step;
	TIM15->ARR = step;
	for(int j=0,count1=0;j<delay;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( count1==delay-1)
		{
		TIM15->CCR1 = 0;
		}
  	}
}
void Step2(int32_t step,int32_t delay)
{
	if(step>100)
	{
	printf("the steps should be  -100 .. 100 and setting it to 100\n");
	step=100;
	}
	if(step<0)
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,1);
	step=-step;
	}
	else
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,0);
	}
	HAL_TIM_PWM_Start(&tim15,TIM_CHANNEL_1);
	step = 1485-(step*13.5);
	TIM15->CCR1 = step;
	TIM15->ARR = step;
	for(int j=0,count1=0;j<delay;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		TIM15->ARR = step++;
		if( count1==delay-1)
		{
		TIM15->CCR1= 0;
		}
  	}
	
}

ParserReturnVal_t Cmdstepperinitpwm(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	steppermotorinit1 ();
  return CmdReturnOk;
}

ADD_CMD("stepperinitpwm",Cmdstepperinitpwm,"            Initialies the motor funcs in pwm mode")
ParserReturnVal_t Cmdstepperenablepwm(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&value);
	stepperenable1 (value);
  return CmdReturnOk;
}

ADD_CMD("stepperenablepwm",Cmdstepperenablepwm,"            Enables the motor in pwm")
ParserReturnVal_t Cmdsteppwm(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&step);
	fetch_int32_arg(&delay);
	Step1 (step,delay);
  return CmdReturnOk;
}

ADD_CMD("steppwm",Cmdsteppwm,"            Enables the motor in pwm")
ParserReturnVal_t Cmdstepspindownpwm(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&step);
	fetch_int32_arg(&delay);
	Step2 (step,delay);
  return CmdReturnOk;
}

ADD_CMD("stepspindown",Cmdstepspindownpwm,"            Enables the motor in pwm")
