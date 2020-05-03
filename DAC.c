#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

static DAC_HandleTypeDef hdac;
DAC_ChannelConfTypeDef DacConfig;
HAL_StatusTypeDef rc;
static int32_t value;
static float a;

void DACinitialization(void)
{
/* Turn on clocks to I/O */
	__HAL_RCC_GPIOA_CLK_ENABLE();
/* Configure GPIO pins */
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 

/* Enable the clock */
	__HAL_RCC_DAC1_CLK_ENABLE();

/* Initialize DAC */
	hdac.Instance=DAC1;
	rc = HAL_DAC_Init(&hdac);
	if(rc != HAL_OK) 
	{
		printf("Unable to initialize ""DAC, rc=%d\n",rc);
		return;
	}
	DacConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	DacConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	rc = HAL_DAC_ConfigChannel(&hdac,&DacConfig,DAC_CHANNEL_1);
	if(rc != HAL_OK) 
	{
		printf("Unable to configure DAC ""channel 1, rc=%d\n",rc);
		return;
	}
/* Enable the output */ 
	__HAL_DAC_ENABLE(&hdac,DAC_CHANNEL_1);
	printf("DAC has been initialized successfully\n");
}

void DAC_SetValue(void)
{
/* Set initial values */
	
	if(a>3.3)
	{
		printf("The limit is 0 to 3.3V...setting to 3.3V\n");
		a=3.3;
	}
	a=a/3.3;
	value=a*4095;
	rc = HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,value);
	if(rc != HAL_OK) 
	{
		printf("Unable to initial value on DAC ""channel 1, rc=%d\n",rc);
		return;
	}
	printf("Turned On\n");
}

void DAC_SetValue1(void)
{
/* Set initial values */
	
	if(value>4095)
	{
		printf("The limit is 0 to 4095...setting to 4095\n");
		value=4095;
	}
	rc = HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,value);
	if(rc != HAL_OK) 
	{
		printf("Unable to initial value on DAC ""channel 1, rc=%d\n",rc);
		return;
	}
}


ParserReturnVal_t CmdDACinit(int mode)
	{
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		DACinitialization();
  		return CmdReturnOk;
	}

	ADD_CMD("dacinit",CmdDACinit,"            DAC Status")

ParserReturnVal_t CmdDACvalue(int mode)
	{
  
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		fetch_float_arg(&a);
		DAC_SetValue();
  		return CmdReturnOk;
	}
	
	ADD_CMD("dac",CmdDACvalue,"            DAC Status")

ParserReturnVal_t CmdDACrawvalue(int mode)
	{
  
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		fetch_int32_arg(&value);
		DAC_SetValue1();
		return CmdReturnOk;
	}

	ADD_CMD("dacraw",CmdDACrawvalue,"            DAC Status")

