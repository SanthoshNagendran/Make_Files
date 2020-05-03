#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

void TIM4_IRQHandler0(void);//please remove the 0 in the last of interrupt handler function name both in definition and declaration

volatile uint32_t myTicks1 = 0;

void InterruptInit()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = (GPIO_PIN_0);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	printf("GPIO initialisation successfull\n");
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);

	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	TIM4->PSC = 72;
	TIM4->ARR = 1000;
	TIM4->CR1 |= TIM_CR1_URS;
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->EGR |= TIM_EGR_UG;

	NVIC_EnableIRQ(TIM4_IRQn);

	printf("Interrupt initialisation successful\n");
}

void GPIO(int32_t ms)
{
	TIM4->CR1 |= TIM_CR1_CEN;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
	myTicks1=ms;
}
void TIM4_IRQHandler0(void)///////////////i have changed the function by adding 0 at the last,if it isnt working remove the 0
{
	if(myTicks1)
	{
	myTicks1--;
		if(myTicks1==0)
		{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		TIM4->CR1 &= ~TIM_CR1_CEN;
		}
	}
	
	TIM4->SR &= ~TIM_SR_UIF;
}
ParserReturnVal_t CmdInterruptInit(int mode)
	{
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		InterruptInit();
  		return CmdReturnOk;
	}

	ADD_CMD("interruptinit",CmdInterruptInit,"            Initialises 	the internal interryupt timer")

ParserReturnVal_t Cmddelaygpio(int mode)
	{
	int32_t ms;
  	
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		fetch_int32_arg(&ms);
		GPIO(ms);
		return CmdReturnOk;
	}
	
	ADD_CMD("interruptgpio",Cmddelaygpio,"            produces delay for PA0")
