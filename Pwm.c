#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"
TIM_HandleTypeDef tim1;
volatile uint32_t flag5 = 0;
volatile uint32_t count = 0;

void PWM_Init(void)
{
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Pin = (GPIO_PIN_8| GPIO_PIN_9 |GPIO_PIN_10);
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 6;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	tim1.Instance = TIM1;
	tim1.Init.Prescaler = HAL_RCC_GetPCLK2Freq() / 1000000;
	tim1.Init.CounterMode   = TIM_COUNTERMODE_UP;
	tim1.Init.Period        = 1000;
	tim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim1.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&tim1);

	TIM_OC_InitTypeDef sConfig;
	sConfig.OCMode       = TIM_OCMODE_PWM1;
	sConfig.Pulse        = 0;
	sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	sConfig.OCNPolarity  = TIM_OCNPOLARITY_LOW;
	sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
	sConfig.OCNIdleState =TIM_OCNIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&tim1,&sConfig,TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&tim1,&sConfig,TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&tim1,&sConfig,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&tim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&tim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&tim1,TIM_CHANNEL_3);
	timerinit();
	printf("PWM initialised Successfully\n");

}
void PwmIncrement(void)
{

//////////this is for the first timer
	HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn,0,0);
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	TIM1->DIER |= TIM_DIER_UIE;
	TIM1->CCR1=100;
}
////////this below function <interrupt handler> is used to make the dc motor ramp the speed up(100%) and down to 0%
void TIM1_UP_TIM16_IRQHandler(void)
{
if(count>=0)
{
	if(flag5==0)
	{
	if(TIM1->SR & TIM_SR_UIF)
		{
		if(TIM1->CCR1 >= 1000)
			{
			flag5=1;
			count=0;
			} 
		else if(count<25)
			{
			count++;
			}
		else if(count==25)
			{
			count=0;
		  	TIM1->CCR1++;
			}		
		TIM1->SR &= ~TIM_SR_UIF;
		}
	}
	else if(flag5==1)
	{
	if(TIM1->SR & TIM_SR_UIF)
		{
		if(TIM1->CCR1 == 100)
			{
		 	HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
			flag5=0;
			count=0;
			}
		else if(count<25)
			{
			count++;
			} 
		else  if(count==25)
			{
			count=0;
		  	TIM1->CCR1--;
			}		
		TIM1->SR &= ~TIM_SR_UIF;
		}
	}
}
}
void PWM_Set(int32_t channel,int32_t value)
{
	if(value>100)
	{
	printf("the value should be  -100 .. 100 and setting it to 100\n");
	value=100;
	}
	value = (value*10);
	switch(channel)
	{
	case 1:
	{
	TIM1->CCR1 = value;
	//TIM1->ARR = value;
	for(int j=0,count1=0;j<2000;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		TIM1->CCR1 = value++;
		//TIM1->ARR = value++;
		if( j==1999)
		{
		TIM1->CCR1 = 0;
		//TIM1->ARR = 0;
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
		TIM1->CCR1 = value--;
		//TIM1->ARR = value--;
		if( value==0)
		{
		TIM1->CCR1 = 0;
		}
  	}
	TIM1->CCR1 = 0;
	break;
	}
	case 2:
	{
	TIM1->CCR2 = value;
	for(int j=0,count1=0;j<1000;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		TIM1->CCR2 = value++;
		if( j==999)
		{
		TIM1->CCR2 = 0;
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
		TIM1->CCR2 = value--;
		if( value==0)
		{
		TIM1->CCR2 = 0;
		}
  	}
	TIM1->CCR2 = 0;
	break;
	}
	case 3:
	{
	TIM1->CCR3 = value;
	for(int j=0,count1=0;j<1000;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		TIM1->CCR3 = value++;
		if( j==999)
		{
		TIM1->CCR3 = 0;
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
		TIM1->CCR3 = value--;
		if( value==0)
		{
		TIM1->CCR3 = 0;
		}
  	}
	TIM1->CCR3 = 0;
	break;
	}
	default:
	{
	printf("please enter a valid channel number from 1..3\n");
	}
	}
}
void PWM_Set1(int32_t channel,int32_t value,int32_t delay)
{
	if(value>100)
	{
	printf("the value should be  -100 .. 100 and setting it to 100\n");
	value=100;
	}
	value = (value*10);
	switch(channel)
	{
	case 1:
	{
	TIM1->CCR1 = value;
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
		TIM1->CCR1 = 0;
		}
  	}
	break;
	}
	case 2:
	{
	TIM1->CCR2 = value;
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
		TIM1->CCR2 = 0;
		}
  	}
	break;
	}
	case 3:
	{
	TIM1->CCR3 = value;
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
		TIM1->CCR3 = 0;
		}
  	}
	break;
	}
	default:
	{
	printf("please enter a valid channel number from 1..3\n");
	}
	}
}
void PWM_Set2(int32_t channel,int32_t value)
{
	if(value>100)
	{
	printf("the value should be  -100 .. 100 and setting it to 100\n");
	value=100;
	}
	value = (value*10);
	switch(channel)
	{
	case 1:
	{
	TIM1->CCR1 = value;
	for(int j=0,count1=0;j<500;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 500) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( j==499)
		{
		TIM1->CCR1 = 0;
		}
  	}
	for(int j=0,count1=0;j<500;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 500) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( j==499)
		{
		TIM1->CCR1 = value;
		}
  	}
	for(int j=0,count1=0;j<500;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 500) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( j==499)
		{
		TIM1->CCR1 = 0;
		}
  	}
	break;
	}
	case 2:
	{
	TIM1->CCR2 = value;
	for(int j=0,count1=0;j<500;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 500) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( j==499)
		{
		TIM1->CCR2 = 0;
		}
  	}
	for(int j=0,count1=0;j<500;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 500) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( j==499)
		{
		TIM1->CCR2 = value;
		}
  	}
	for(int j=0,count1=0;j<500;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 500) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( j==499)
		{
		TIM1->CCR2 = 0;
		}
  	}
	break;
	}
	case 3:
	{
	TIM1->CCR3 = value;
	for(int j=0,count1=0;j<500;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 500) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( j==499)
		{
		TIM1->CCR3 = 0;
		}
  	}
	for(int j=0,count1=0;j<500;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 500) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( j==499)
		{
		TIM1->CCR3 = value;
		}
  	}
	for(int j=0,count1=0;j<500;j++)
	{
		TIM17->CNT = 0;       /* Reset counter */
		while(TIM17->CNT < 500) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( j==499)
		{
		TIM1->CCR3 = 0;
		}
  	}	
	break;
	}
	default:
	{
	printf("please enter a valid channel number from 1..3\n");
	}
	}
}

void PWM_Set3(int32_t channel,int32_t value)
{
	if(value>100)
	{
	printf("the value should be  -100 .. 100 and setting it to 100\n");
	value=100;
	}
	value = (value*10);
	switch(channel)
	{
	case 1:
	{
	TIM1->CCR1 = value;

	break;
	}
	case 2:
	{
	TIM1->CCR2 = value;
	
	break;
	}
	case 3:
	{
	TIM1->CCR3 = value;
	
	break;
	}
	default:
	{
	printf("please enter a valid channel number from 1..3\n");
	}
	}
return;
}

void PWM_Set4(int32_t channel,int32_t value)
{

	switch(channel)
	{
	case 1:
	{
	TIM1->CCR1 = value;

	break;
	}
	case 2:
	{
	TIM1->CCR2 = value;
	
	break;
	}
	case 3:
	{
	TIM1->CCR3 = value;
	
	break;
	}
	default:
	{
	printf("please enter a valid channel number from 1..3\n");
	}
	}
return;
}


ParserReturnVal_t CmdPwminit(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	PWM_Init();
  return CmdReturnOk;
}

ADD_CMD("pwminit",CmdPwminit,"            pwm initialisation")

ParserReturnVal_t CmdpwmSet(int mode)
{
  int32_t value,channel;
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&channel);
	fetch_int32_arg(&value);
	PWM_Set(channel,value);
  return CmdReturnOk;
}

ADD_CMD("pwmset",CmdpwmSet,"            Enables the pwm autonomously")

ParserReturnVal_t CmdpwmSet2(int mode)
{
  int32_t value,channel;
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&channel);
	fetch_int32_arg(&value);
	PWM_Set2(channel,value);
  return CmdReturnOk;
}

ADD_CMD("pwmblink",CmdpwmSet2,"            Enables the pwm autonomously in blinking mode")

ParserReturnVal_t CmdpwmSet1(int mode)
{
  int32_t value,channel,delay;
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&channel);
	fetch_int32_arg(&value);
	fetch_int32_arg(&delay);
	PWM_Set1(channel,value,delay);
  return CmdReturnOk;
}

ADD_CMD("pwmsetvalue",CmdpwmSet1,"            Enables the pwm to set value in % for a user defined time")

ParserReturnVal_t CmdpwmSet3(int mode)
{
  int32_t value,channel;
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&channel);
	fetch_int32_arg(&value);
	PWM_Set3(channel,value);
  return CmdReturnOk;
}

ADD_CMD("pwmpermanent",CmdpwmSet3,"            Enables the pwm to set value in % for permanent time")

ParserReturnVal_t CmdPwmIncrement(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	PwmIncrement();
  return CmdReturnOk;
}

ADD_CMD("pwmauto",CmdPwmIncrement,"            increments the ch1 itself using interrupts")
