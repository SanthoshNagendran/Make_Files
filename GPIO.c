
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

static int32_t GPIO_PIN_PinNumber;
static int32_t State;

//this function initializes the gpio pins and also execute the led operation according to the user's wish
void gpioinitialisation(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = (GPIO_PIN_0| GPIO_PIN_1 |GPIO_PIN_5);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	printf("GPIO initialisation successfull\n");
}
void gpioint(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = (GPIO_PIN_0| GPIO_PIN_1 |GPIO_PIN_5);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	switch(GPIO_PIN_PinNumber)
		{
	case 0:
	{
		printf("The pin number u have selected is %ld\n",GPIO_PIN_PinNumber);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,State);
		break;
	}
	case 1:
	{
		printf("The pin number u have selected is %ld\n",GPIO_PIN_PinNumber);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,State);
		break;
	}
	case 5:
	{
		printf("The pin number u have selected is %ld\n",GPIO_PIN_PinNumber);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,State);
		break;
	}
		default:
	{
		printf("ENter a valid pin number\n");
	}
		}
	if(State==0)
	{
		printf("The On board LED is OFF\n");
	}
	else if(State==1)
	{
		printf("The On board LED is ON\n");
	}
	else
	{
		printf("Enter only 1 or 0 for ON and OFF\n");
	}
}
ParserReturnVal_t CmdGPIOinit(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	gpioinitialisation();
  return CmdReturnOk;
}

ADD_CMD("gpioinit",CmdGPIOinit,"            GPIO initialisation")

ParserReturnVal_t CmdGPIO(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&GPIO_PIN_PinNumber);
	fetch_int32_arg(&State);
	gpioint();
  return CmdReturnOk;
}

ADD_CMD("gpio",CmdGPIO,"            GPIO Status")
