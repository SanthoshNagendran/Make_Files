#include <stdio.h>
#include <stdint.h>
#include <ctype.h>


#include "common.h"
#define HAL_ADC_MODULE_ENABLED

ADC_HandleTypeDef hadc;
ADC_ChannelConfTypeDef config;



uint32_t val;
HAL_StatusTypeDef rc;

TIM_HandleTypeDef htim1;


void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);


void setPWMandTurnON(TIM_HandleTypeDef* htim1, int32_t pwmPeriod) {
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
	htim1->Instance = TIM1;
	htim1->Init.Prescaler = 0;
	htim1->Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1->Init.Period = pwmPeriod;
	htim1->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1->Init.RepetitionCounter = 0;
	htim1->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&*htim1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&*htim1, &sClockSourceConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	if (HAL_TIM_PWM_Init(&*htim1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&*htim1, &sMasterConfig)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_TIM_PWM_Stop(&*htim1, TIM_CHANNEL_1);
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pwmPeriod / 2;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&*htim1, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&*htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	/* USER CODE BEGIN 1 */
	HAL_TIMEx_PWMN_Start(&*htim1, TIM_CHANNEL_1);
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM1_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspPostInit 0 */

  /* USER CODE END TIM1_MspPostInit 0 */
  
    /**TIM1 GPIO Configuration    
    PA7     ------> TIM1_CH1N 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate =  6/*GPIO_AF6_TIM1*/;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	printf("GPIO is Done\n");
  /* USER CODE BEGIN TIM1_MspPostInit 1 */

  /* USER CODE END TIM1_MspPostInit 1 */
  }

}
void ComposeTone(int x, int t, int t1) //this is to compose tone and it has an alphabet,on time and off time
{
	if (x == 51) {
		int32_t freq = 523;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 52)

	{
		int32_t freq = 587;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 53)

	{
		int32_t freq = 659;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 54) {
		int32_t freq = 699;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 55) {
		int32_t freq = 784;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 56) {
		int32_t freq = 880;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 57) {
		int32_t freq = 988;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 61) {
		int32_t freq = 1047;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 62) {
		int32_t freq = 1175;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 63) {
		int32_t freq = 1319;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 64) {
		int32_t freq = 1397;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 65) {
		int32_t freq = 1568;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 66) {
		int32_t freq = 1760;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 67) {
		int32_t freq = 1976;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	}
}
void AirtelMusic() // this when called plays Airtel Music
{
	printf("playing airtel music\n");
	ComposeTone(56, 300, 100);
	ComposeTone(56, 150, 100);
	ComposeTone(56, 150, 100);
	ComposeTone(56, 300, 100);
	ComposeTone(63, 400, 100);
	ComposeTone(57, 100, 100);
	ComposeTone(61, 100, 100);
	ComposeTone(57, 100, 100);
	ComposeTone(56, 100, 100);
	ComposeTone(55, 400, 100);
	ComposeTone(57, 400, 100);
	ComposeTone(56, 100, 100);
	ComposeTone(57, 100, 100);
	ComposeTone(56, 100, 100);
	ComposeTone(55, 100, 100);
	ComposeTone(56, 200, 100);
	ComposeTone(57, 100, 100);
	ComposeTone(61, 300, 100);
	ComposeTone(57, 300, 100);
	ComposeTone(56, 100, 100);
	ComposeTone(55, 100, 100);
	ComposeTone(53, 300, 100);
	
	printf("end of music\n");
}

void speakerSetup(void *data)
{
	
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = (GPIO_PIN_0);
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
	
	int32_t freq = 440;
	int32_t pwmPeriod = 10000000 / (freq * 250);
	__HAL_RCC_TIM1_CLK_ENABLE();
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 0;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pwmPeriod / 2;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	HAL_TIM_MspPostInit(&htim1);
	printf("speaker setup is done\n");
	printf("keyboard setup is done\n");
	AirtelMusic();
	
}

void deBounceReadPin(void *data)
{
	
	/*config.Channel = 6;
	config.Rank = 1;  

	config.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
	config.SingleDiff = ADC_SINGLE_ENDED;
	config.OffsetNumber = ADC_OFFSET_NONE;
	config.Offset = 0;

	rc = HAL_ADC_ConfigChannel(&hadc,&config);
	if(rc != HAL_OK) 
	{
		printf("ADC channel failed with rc=%u\n",(unsigned)rc);
	}

//Start the ADC peripheral 
	rc = HAL_ADC_Start(&hadc);
	if(rc != HAL_OK) 
	{
		printf("ADC start failed with rc=%u\n",(unsigned)rc);
	}

// Wait for end of conversion 
	rc = HAL_ADC_PollForConversion(&hadc, 100);
	if(rc != HAL_OK) 
	{
		printf("ADC poll failed with ""rc=%u\n",(unsigned)rc);
	}

// Read the ADC converted values 
	val = HAL_ADC_GetValue(&hadc);
	//printf("The ADC converted raw Value is %ld\n",val);
// Stop the ADC peripheral 
	rc = HAL_ADC_Stop(&hadc);
	if(rc != HAL_OK) 
	{
		printf("ADC stop failed with ""rc=%u\n",(unsigned)rc);
	}

	if(val==180)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 523;
	int32_t pwmPeriod = 1000000000 / (freq*2 );
	setPWMandTurnON(&htim1, pwmPeriod);
	}
	else if(val==181)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 587;
	int32_t pwmPeriod = 1000000000 / (freq*2 );
	setPWMandTurnON(&htim1, pwmPeriod);
	}
	else if(val==182)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 659;
	int32_t pwmPeriod = 1000000000 / (freq*2 );
	setPWMandTurnON(&htim1, pwmPeriod);
	}
	else if(val==183)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 699;
	int32_t pwmPeriod = 1000000000 / (freq*2 );
	setPWMandTurnON(&htim1, pwmPeriod);
	}
	else if(val==184)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 784;
		int32_t pwmPeriod = 1000000000 / (freq * 2);
		setPWMandTurnON(&htim1, pwmPeriod);
	}
	else if(val==185)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 880;
	int32_t pwmPeriod = 1000000000 / (freq*2 );
	setPWMandTurnON(&htim1, pwmPeriod);
	}
	else if(val==186)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 988;
	int32_t pwmPeriod = 1000000000 / (freq*2 );
	setPWMandTurnON(&htim1, pwmPeriod);
	}
	else
	{
	setPWMandTurnON(&htim1, 0);
	} */
	
	int pinState1=1,pinState2=1,pinState3=1,pinState4=1;
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = (GPIO_PIN_0| GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_4);
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	//GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//pinState1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
	pinState2 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
	//pinState3 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
	pinState4 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
	pinState1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
	pinState3 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
	if(pinState1==0)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 523;
	int32_t pwmPeriod = 1000000000 / (freq*2 );
	setPWMandTurnON(&htim1, pwmPeriod);
	}
	else if(pinState2==0)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 587;
	int32_t pwmPeriod = 1000000000 / (freq*2 );
	setPWMandTurnON(&htim1, pwmPeriod);
	}
	else if(pinState3==0)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 659;
	int32_t pwmPeriod = 1000000000 / (freq*2 );
	setPWMandTurnON(&htim1, pwmPeriod);
	}
	else if(pinState4==0)
	{
	setPWMandTurnON(&htim1, 0);
	int32_t freq = 699;
	int32_t pwmPeriod = 1000000000 / (freq*2 );
	setPWMandTurnON(&htim1, pwmPeriod);
	}
	else
	{
	setPWMandTurnON(&htim1, 0);
	}
}


ADD_TASK(deBounceReadPin,speakerSetup,NULL,100,"this is goin to check for the key press continuously")

/*
void ComposeTone(int x, int t, int t1) //this is to compose tone and it has an alphabet,on time and off time
{
	if (x == 51) {
		int32_t freq = 523;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 52)

	{
		int32_t freq = 587;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 53)

	{
		int32_t freq = 659;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 54) {
		int32_t freq = 699;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 55) {
		int32_t freq = 784;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 56) {
		int32_t freq = 880;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 57) {
		int32_t freq = 988;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 61) {
		int32_t freq = 1047;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 62) {
		int32_t freq = 1175;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 63) {
		int32_t freq = 1319;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 64) {
		int32_t freq = 1397;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 65) {
		int32_t freq = 1568;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 66) {
		int32_t freq = 1760;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	} else if (x == 67) {
		int32_t freq = 1976;
		int32_t pwmPeriod = 1000000000 / (freq * 25);
		setPWMandTurnON(&htim1, pwmPeriod);
		HAL_Delay(t);
		setPWMandTurnON(&htim1, 0);
		HAL_Delay(t1);
	}
}
void AirtelMusic() // this when called plays Airtel Music
{
	printf("playing airtel music\n");
	ComposeTone(56, 300, 100);
	ComposeTone(56, 150, 100);
	ComposeTone(56, 150, 100);
	ComposeTone(56, 300, 100);
	ComposeTone(63, 400, 100);
	ComposeTone(57, 100, 100);
	ComposeTone(61, 100, 100);
	ComposeTone(57, 100, 100);
	ComposeTone(56, 100, 100);
	ComposeTone(55, 400, 100);
	ComposeTone(57, 400, 100);
	ComposeTone(56, 100, 100);
	ComposeTone(57, 100, 100);
	ComposeTone(56, 100, 100);
	ComposeTone(55, 100, 100);
	ComposeTone(56, 200, 100);
	ComposeTone(57, 100, 100);
	ComposeTone(61, 300, 100);
	ComposeTone(57, 300, 100);
	ComposeTone(56, 100, 100);
	ComposeTone(55, 100, 100);
	ComposeTone(53, 300, 100);
	
	printf("end of music\n");
}

ParserReturnVal_t CmdSpeakerSetup(int mode)
	{
  	
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		speakerSetup();
		return CmdReturnOk;
	}
	
	ADD_CMD("speakerinit",CmdSpeakerSetup,"            sets up the speaker")

ParserReturnVal_t Cmdbeepsound(int mode)
	{
  	
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		KeyPress();
		return CmdReturnOk;
	}
	
	ADD_CMD("beep",Cmdbeepsound,"            sets up the speaker")

*/
