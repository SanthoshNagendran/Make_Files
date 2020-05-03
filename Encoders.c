#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

TIM_HandleTypeDef tim3;

void EncoderInit(void)
{
	HAL_StatusTypeDef rc;
	TIM_Encoder_InitTypeDef encoderConfig;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Pin = (GPIO_PIN_6| GPIO_PIN_7);
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	__HAL_RCC_TIM3_CLK_ENABLE();
	tim3.Instance = TIM3;
	tim3.Init.Prescaler = 0;
	tim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim3.Init.Period = 0xffff;
	tim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim3.Init.RepetitionCounter = 0;
	rc = HAL_TIM_Base_Init(&tim3);
	if(rc != HAL_OK) 
		{
		printf("Failed to initialize Timer 3 Base, "         			"rc=%u\n",rc);
		return;
		}
	
	encoderConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	encoderConfig.IC1Polarity = 0;
	encoderConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	encoderConfig.IC1Prescaler = 0;
	encoderConfig.IC1Filter = 3;
	encoderConfig.IC2Polarity = 0;
	encoderConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	encoderConfig.IC2Prescaler = 0;
	encoderConfig.IC2Filter = 3;

	rc = HAL_TIM_Encoder_Init(&tim3,&encoderConfig);
	if(rc != HAL_OK) 
		{
		printf("Failed to initialize Timer 3 Encoder,"         			"rc=%u\n",rc);
		return;
		}
	rc = HAL_TIM_Encoder_Init(&tim3,&encoderConfig);
	if(rc != HAL_OK) 
		{
		printf("Failed to initialize Timer 3 Encoder,"         			"rc=%u\n",rc);
		return;
		}
	rc = HAL_TIM_Encoder_Start(&tim3, TIM_CHANNEL_1);
	if(rc != HAL_OK) 
		{
		printf("Failed to start Timer 3 Encoder, "         			"rc=%u\n",rc);
		return;
		}
	rc = HAL_TIM_Encoder_Start(&tim3, TIM_CHANNEL_2);
	if(rc != HAL_OK) 
		{
		printf("Failed to start Timer 3 Encoder, "          			"rc=%u\n",rc);
		return;
		}

	printf("Encoders initialised\n");
}

int16_t encoders(void)
{
	int16_t a = 0;
	a = TIM3->CNT;
	return a;
}

ParserReturnVal_t CmdEncoderinit(int mode)
{

  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	EncoderInit();
  return CmdReturnOk;
}

ADD_CMD("encoderinit",CmdEncoderinit,"            measures the speed and direction of rotating motor")

ParserReturnVal_t CmdEncoders(int mode)
{
  int16_t a = 0;
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	a = encoders();
	printf("the encoder value is %d\n",a);
  return CmdReturnOk;
}

ADD_CMD("encoder",CmdEncoders,"            measures the speed and direction of rotating motor")

