#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

void TIM4_IRQHandler1(void);//remove the number 1 in the function name

volatile uint32_t myTicks = 0;

void DCMotorInit(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = (GPIO_PIN_0| GPIO_PIN_1);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = (GPIO_PIN_10| GPIO_PIN_11);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0| GPIO_PIN_1,0);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10| GPIO_PIN_11,0);

	printf("DC motor initialisation successfull\n");
/////////////this is for the interrupt initialisation
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	TIM4->PSC = 72;
	TIM4->ARR = 1000;
	TIM4->CR1 |= TIM_CR1_URS;
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->EGR |= TIM_EGR_UG;

	NVIC_EnableIRQ(TIM4_IRQn);

}
void MotorDirction(char dir)
{
	switch(dir)
	{
	case'f':
		{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,1);
		printf("Spinning front\n");
		break;
		}
	case'b':
		{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,1);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,0);
		printf("Spinning Back\n");
		break;
		}
	case's':
		{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,0);
		printf("Motor stopped\n");
		break;
		}
	default :
		{
		printf("wrong direction entered plese enter 'f' for front and 'b' for back\n");
		}
	}
	
}

void MotorOnTime(char dir,int32_t ms)
{
	TIM4->CR1 |= TIM_CR1_CEN;
	switch(dir)
	{
	case'f':
		{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,1);
		printf("Spinning front\n");
		myTicks=ms;
		break;
		}
	case'b':
		{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,1);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,0);
		printf("Spinning Back\n");
		myTicks=ms;
		break;
		}
	case's':
		{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,0);
		printf("Motor stopped\n");
		myTicks=ms;
		break;
		}
	default :
		{
		printf("wrong direction entered plese enter 'f' for front and 'b' for back\n");
		}
	}
	
}
void TIM4_IRQHandler1(void)//remove the number 1 in the function name
{
	if(myTicks)
	{
	myTicks--;
		if(myTicks==0)
		{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,0);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,0);
		TIM4->CR1 &= ~TIM_CR1_CEN;
		}
	}
	
	TIM4->SR &= ~TIM_SR_UIF;
}

ParserReturnVal_t CmdDCmotorinit(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	DCMotorInit();
  return CmdReturnOk;
}

ADD_CMD("dcmotorinit",CmdDCmotorinit,"            DC motor initialisation")

ParserReturnVal_t CmdMotorSet(int mode)
{
  char *dir;
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_string_arg(&dir);//////////used to fetch a character
	MotorDirction(*dir);
  return CmdReturnOk;
}

ADD_CMD("motordirection",CmdMotorSet,"            This is to set the DC motor direction")

ParserReturnVal_t CmdMotorOnTime(int mode)
{
  	char *dir;
	int32_t ms;
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_string_arg(&dir);//////////used to fetch a character (direction)
	fetch_int32_arg(&ms);
	MotorOnTime(*dir, ms);
  return CmdReturnOk;
}

ADD_CMD("motordirection",CmdMotorOnTime,"            This is to set the DC motor direction for a certain time using interrupt")
