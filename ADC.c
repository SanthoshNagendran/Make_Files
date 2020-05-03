#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

#define HAL_ADC_MODULE_ENABLED

ADC_HandleTypeDef hadc;
ADC_ChannelConfTypeDef config;

uint32_t val,val1;
float a=0;
static int32_t channel,channel1;
int flag=0;
HAL_StatusTypeDef rc;

void ADCinitialization(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = (GPIO_PIN_0| GPIO_PIN_1);
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


/* Initialize ADC */
	__HAL_RCC_ADC1_CLK_ENABLE();
	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.NbrOfConversion = 1;
	hadc.Init.DiscontinuousConvMode = DISABLE;	
	hadc.Init.NbrOfDiscConversion = 0;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.ExternalTrigConvEdge = 0;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;

	rc = HAL_ADC_Init(&hadc);
	if(rc != HAL_OK) 
	{
		printf("ADC1 initialization failed with rc=%u\n",rc);
	}
	printf("ADC has been initialized successfully\n");
}

void ADCChannel(void)
{
/* Configure the selected channel */
	config.Channel = channel;
	config.Rank = 1;  
/* Rank needs to be 1, otherwise no conversion is done */
	config.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	config.SingleDiff = ADC_SINGLE_ENDED;
	config.OffsetNumber = ADC_OFFSET_NONE;
	config.Offset = 0;

	rc = HAL_ADC_ConfigChannel(&hadc,&config);
	if(rc != HAL_OK) 
	{
		printf("ADC channel failed with rc=%u\n",(unsigned)rc);
	}

/* Start the ADC peripheral */
	rc = HAL_ADC_Start(&hadc);
	if(rc != HAL_OK) 
	{
		printf("ADC start failed with rc=%u\n",(unsigned)rc);
	}

/* Wait for end of conversion */
	rc = HAL_ADC_PollForConversion(&hadc, 100);
	if(rc != HAL_OK) 
	{
		printf("ADC poll failed with ""rc=%u\n",(unsigned)rc);
	}

/* Read the ADC converted values */
	val = HAL_ADC_GetValue(&hadc);
	val1 = HAL_ADC_GetValue(&hadc);
	a = val1*3.3;
	a = a/4095;	
	printf("The ADC converted raw Value is %ld\n",val);
	printf("The voltage is %.2f V\n",a);
/* Stop the ADC peripheral */
	rc = HAL_ADC_Stop(&hadc);
	if(rc != HAL_OK) 
	{
		printf("ADC stop failed with ""rc=%u\n",(unsigned)rc);
	}

	if(flag==0)
	{
		flag=1;
		channel=channel1;
		ADCChannel();
	}
}
void ADCChannel1(void)
{
/* Configure the selected channel */
	config.Channel = channel;
	config.Rank = 1;  
/* Rank needs to be 1, otherwise no conversion is done */
	config.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	config.SingleDiff = ADC_SINGLE_ENDED;
	config.OffsetNumber = ADC_OFFSET_NONE;
	config.Offset = 0;

	rc = HAL_ADC_ConfigChannel(&hadc,&config);
	if(rc != HAL_OK) 
	{
		printf("ADC channel failed with rc=%u\n",(unsigned)rc);
	}

/* Start the ADC peripheral */
	rc = HAL_ADC_Start(&hadc);
	if(rc != HAL_OK) 
	{
		printf("ADC start failed with rc=%u\n",(unsigned)rc);
	}

/* Wait for end of conversion */
	rc = HAL_ADC_PollForConversion(&hadc, 100);
	if(rc != HAL_OK) 
	{
		printf("ADC poll failed with ""rc=%u\n",(unsigned)rc);
	}

/* Read the ADC converted values */
	val = HAL_ADC_GetValue(&hadc);
	val1 = HAL_ADC_GetValue(&hadc);
	printf("The ADC converted raw Value is %ld\n",val);
	a = val1*0.0289;
	printf("The temperature is %.2f°C\n",a);
/* Stop the ADC peripheral */
	rc = HAL_ADC_Stop(&hadc);
	if(rc != HAL_OK) 
	{
		printf("ADC stop failed with ""rc=%u\n",(unsigned)rc);
	}

	if(flag==0)
	{
		flag=1;
		channel=channel1;
		ADCChannel();
	}
}

ParserReturnVal_t CmdADCinit(int mode)
	{
  
  	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	ADCinitialization();
  	return CmdReturnOk;
	}

	ADD_CMD("adcinit",CmdADCinit,"            ADC Status")

ParserReturnVal_t CmdADC(int mode)
	{
  
  	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&channel);
	fetch_int32_arg(&channel1);
	ADCChannel();
  	return CmdReturnOk;
	}

	ADD_CMD("adc",CmdADC,"            ADC Status")

ParserReturnVal_t CmdADC1(int mode)
	{
  
  	if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&channel);
	fetch_int32_arg(&channel1);
	ADCChannel1();
  	return CmdReturnOk;
	}

	ADD_CMD("adctemp",CmdADC1,"            ADC Status")	
