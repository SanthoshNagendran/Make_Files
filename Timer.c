#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

TIM_HandleTypeDef tim17;
static int32_t value,value1;

void timerinit(void)
{
	__HAL_RCC_TIM17_CLK_ENABLE();
	tim17.Instance = TIM17;
	tim17.Init.Prescaler =HAL_RCC_GetPCLK2Freq() / 1000000 - 1;
	tim17.Init.CounterMode   =TIM_COUNTERMODE_UP;
	tim17.Init.Period = 0xffff;
	tim17.Init.ClockDivision =TIM_CLOCKDIVISION_DIV1;
	tim17.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&tim17);
	HAL_TIM_Base_Start(&tim17);
	printf("Timer initialisation successfull\n");
}
void delay(void)
{
	if(value>6000)
	{
	value1 = value/2;
	printf("LED is ON\n");
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
		for(int i=0;i<value1;i++)
		{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
  		}
		for(int i=0,count=0;i<value1;i++)
		{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
		count++;
		if(count==(value1-1))
		{
		printf("LED OFF\n");
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		}
  		}
	}
	else
	{
	printf("LED is ON\n");
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
		for(int i=0,count=0;i<value;i++)
		{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
		count++;
		if(count==(value-1))
		{
		printf("LED OFF\n");
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		}
  		}
		for(int j=0,count1=0;j<2000;j++)
		{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( count1==1999)
		{
		printf("LED ON after 2 seconds\n");
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
		}
  		}
		for(int j=0,count1=0;j<1000;j++)
		{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( count1==999)
		{
		printf("LED OFF\n");
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		}
  		}
	}
}
void timerDelayValue(int32_t delayVal)
{
	TIM17->CNT = 0;       /* Reset counter */
	while(TIM17->CNT < delayVal) 
	{
	asm volatile ("nop\n");
	}
	delay();	
}

ParserReturnVal_t CmdTimerinit(int mode)
	{
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		timerinit();
  		return CmdReturnOk;
	}

	ADD_CMD("timerinit",CmdTimerinit,"            Initialises 	the timer")

ParserReturnVal_t Cmddelayvalue(int mode)
	{
  	
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		fetch_int32_arg(&value);
		timerDelayValue(value);
		return CmdReturnOk;
	}
	
	ADD_CMD("delay",Cmddelayvalue,"            produces delay")

