#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

TIM_HandleTypeDef tim3;
static int32_t position =0,Reading;
void EncoderInit(void *data)
{
	PWM_Init();
	DCMotorInit();
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
	int32_t degree;
	int16_t a = 0;
	a = TIM3->CNT;
	degree = (a*360)/4//this is to convert into degrees
	return a;
}
void Motor_Control(void *data)
{
	Reading = encoders();
	if(position<0)
	{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
	PWM_Set3(1,25);
		if(Reading<position)
		{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
		PWM_Set3(1,0);
		}
	}
	if(Reading<=position)
	{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);
	PWM_Set3(1,25);
	}
	else if (Reading>position)
	{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
	PWM_Set3(1,25);
	}
	else if (Reading==position)
	{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
	PWM_Set3(1,0);
	}

	if(position==0)
	{
	PWM_Set3(1,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
	
	}
	
}


ADD_TASK(Motor_Control,EncoderInit,NULL,10,"this is to call automatically for every 10ms by itself")//this is to call the functions automatically by itself


ParserReturnVal_t CmdEncoders(int mode)
{
  int16_t a = 0;
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	a = encoders();
	printf("the encoder value is %d\n",a);
  return CmdReturnOk;
}

ADD_CMD("encoder",CmdEncoders,"            measures the speed and direction of rotating motor")

ParserReturnVal_t CmdPositionControl(int mode)
{

  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&position);
  return CmdReturnOk;
}


ADD_CMD("position",CmdPositionControl,"            controls the motor to set to the desired position")
