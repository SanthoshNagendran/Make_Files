/*******************************************************************************************
* File Name          : Valve_Positioner.c
* Description        : Final Project: Hardware Software Interfacing
*					   This program uses the concept State Machine to execute one by one process.
*					   The user have to adjust the valve position and when enters the command, the operation starts based on the prefixed and scaled values.
*					   
*
* Author:              Santhosh Nagendran
* Date:                22-03-2019
********************************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"


#define HAL_ADC_MODULE_ENABLED

ADC_HandleTypeDef hadc;
ADC_ChannelConfTypeDef config;
DAC_HandleTypeDef hdac;
DAC_ChannelConfTypeDef DacConfig;
TIM_HandleTypeDef tim17;
TIM_HandleTypeDef tim1;
HAL_StatusTypeDef rc;

uint32_t val;
int32_t a,b,c,temp,Numarator,ScaledValue,SpinBack;
int flag0 = 0;
volatile uint32_t myTicks2 = 0;
volatile uint32_t val1 = 0;
volatile float money1 = 0;

void HD44780_Init();
void HD44780_ClrScr();
void HD44780_GotoXY();
void HD44780_PutStr();
void stopMsg();
void PrintScreen();
void TIM4_IRQHandler(void);

// FUNCTION      : Valve_initialise()
// DESCRIPTION   : This function used to initialise the pins to be used 
// PARAMETERS    : nothing 
// RETURNS       : nothing
void Valve_initialise(void)
{
	ADCinitialization(); ///to initialise the ADC
	steppermotorinit (); ///to initialize the steppermotor
	PWM_Init();	     ///to initialize pwm
//////////this is for the dc motor initialization with the default direction "Front"
	HD44780_Init();//to initialise LCD
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = (GPIO_PIN_0| GPIO_PIN_1);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	///////////DC motor "STOP"
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);

	printf("DC motor initialisation successfull\n");

	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	TIM4->PSC = 72;
	TIM4->ARR = 1000;
	TIM4->CR1 |= TIM_CR1_URS;
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->EGR |= TIM_EGR_UG;

	NVIC_EnableIRQ(TIM4_IRQn);

	printf("Interrupt initialisation successful\n");

	char stringBuffer[16] = { 0 };
	HD44780_ClrScr();
	HD44780_GotoXY(4, 0);
	snprintf(stringBuffer, 16, "Welcome ");
	HD44780_PutStr(stringBuffer);
	HD44780_GotoXY(1, 1);
	HD44780_PutStr("How R U Today?");
	PWM_Set3(3,100);//this is to enable the LCD backlight
}
// FUNCTION      : Wait()
// DESCRIPTION   : This function is used to make the running process wait for some seconds 
// PARAMETERS    : delay - value 
// RETURNS       : nothing

void Wait(int32_t delay)
{
	for(int j=0,count1=0;j<delay;j++)
	{
		TIM17->CNT = 0;       
		while(TIM17->CNT < 1000) 
		{
		asm volatile ("nop\n");
		}
		count1++;
		if( count1==delay-1)
		{
		flag0 = 3;
		}
  	}
return;
}
// FUNCTION      : TIM17_IRQHandler()
// DESCRIPTION   : This function is used interrupts to make the delay more than 7 seconds
// PARAMETERS    : nothing 
// RETURNS       : nothing
void TIM4_IRQHandler(void)
{

	if(myTicks2>0)
	{
	//PWM_Set3(1,ScaledValue);
	myTicks2--;
	val1++;
	stepperenable(0); 
	for(int i = 14000;i>=1000;i-=200)
	{
		if(myTicks2==i)
		{
		PrintScreen();
		}
	}
	if(myTicks2==1000)
	{
	stepperenable(1);
	Angle(SpinBack,1000);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
	PWM_Set3(1,0);
	PWM_Set3(2,0);
	//PrintScreen();
	money1 = val1+1000;	
	money1 = money1*.125;
char stringBuffer[16] = { 0 };
	HD44780_ClrScr();
	HD44780_GotoXY(0, 0);
	HD44780_PutStr("Fuel Dispencing!");
	HD44780_GotoXY(0, 1);
	val1 = val1+1000;
	snprintf(stringBuffer, 16, "L:%ld", val1/10);
	HD44780_PutStr(stringBuffer);
	HD44780_GotoXY(7, 1);
	snprintf(stringBuffer, 16, "$:%.2f", money1);
	HD44780_PutStr(stringBuffer);
	}
		if(myTicks2==0)
		{
		//Angle(SpinBack,1000);
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
		//PWM_Set3(1,0);
		//PWM_Set3(2,0);
		PWM_Set3(3,0);
		stopMsg();
		TIM4->CR1 &= ~TIM_CR1_CEN;
		}
	}
	
	TIM4->SR &= ~TIM_SR_UIF;
}
// FUNCTION      : PrintScreen(void)
// DESCRIPTION   : This function is used to print the volume and the price of the fuel being consumed
// PARAMETERS    : nothing 
// RETURNS       : nothing
void PrintScreen(void)
{
money1 = val1*.125;
char stringBuffer[16] = { 0 };
	//HD44780_ClrScr();
	HD44780_GotoXY(0, 0);
	HD44780_PutStr("Fuel Dispencing!");
	HD44780_GotoXY(0, 1);
	snprintf(stringBuffer, 16, "L:%ld", val1/10);
	HD44780_PutStr(stringBuffer);
	HD44780_GotoXY(7, 1);
	snprintf(stringBuffer, 16, "$:%.2f", money1);
	HD44780_PutStr(stringBuffer);
return;
}
// FUNCTION      : Valve_Operation()
// DESCRIPTION   : This function is used to execute the actual process 
// PARAMETERS    : nothing 
// RETURNS       : nothing
void Valve_Operation(void)
{	
	stepperenable(1);
	PWM_Set3(3,100);
	config.Channel = 6;//channel 6 refres to PC0
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
	val = HAL_ADC_GetValue(&hadc);///this is the sensor output value all we need
	//val=1000;
//////////here im scaling the value 0 - 4095 to 3000 - 15000 and this is for price and volume of the fuel;
	a = val-0;
	b = 15000-3000;
	c = 4095-0;
	Numarator = a * b;
	temp = Numarator/c;
	ScaledValue = temp + 3000;
	float money = ScaledValue*.125;
/* Stop the ADC peripheral */
	char stringBuffer[16] = { 0 };
	HD44780_ClrScr();
	HD44780_GotoXY(0, 0);
	HD44780_PutStr("Fuel Dispencing!");
	HD44780_GotoXY(0, 1);
	snprintf(stringBuffer, 16, "L:%ld", ScaledValue/10);
	HD44780_PutStr(stringBuffer);
	HD44780_GotoXY(7, 1);
	snprintf(stringBuffer, 16, "$:%.2f", money);
	HD44780_PutStr(stringBuffer);
	
	rc = HAL_ADC_Stop(&hadc);
	if(rc != HAL_OK) 
	{
		printf("ADC stop failed with ""rc=%u\n",(unsigned)rc);
	}
	///////////DC motor "front direction"
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);

	if(val<=1365)//////////here im scaling the value 0 - 1365 to 0 - 100 and this is for glowing the Red LED;
	{
	a = val-0;
	b = 100-0;
	c = 1365-0;
	Numarator = a * b;
	temp = Numarator/c;
	ScaledValue = temp + 0;
	//PWM_Set3(1,ScaledValue);
	flag0 = 1;
	}
	else if(val>=2730)//////////here im scaling the value 2730 - 4095 to 0 - 100 and this is for glowing the Blue LED;
	{
	a = val-2730;
	b = 100-0;
	c = 4095-2730;
	Numarator = a * b;
	temp = Numarator/c;
	ScaledValue = temp + 0;
	//PWM_Set3(1,ScaledValue);
	PWM_Set3(3,ScaledValue);
	flag0 = 1;
	}
	else//////////here im scaling the value 1365 - 2730 to 0 - 100 and this is for glowing the Green LED;
	{
	a = val-1365;
	b = 100-0;
	c = 2730-1365;
	Numarator = a * b;
	temp = Numarator/c;
	ScaledValue = temp + 0;
	//PWM_Set3(1,ScaledValue);
	PWM_Set3(2,ScaledValue);
	flag0 = 1;
	}

	if(flag0==1)//////////here im scaling the value 0 - 4095 to -360 - +360 and this is for moving the stepper motor;
	{
	a = val-0;
	b = 360+360;
	c = 4095-0;
	Numarator = a * b;
	temp = Numarator/c;
	ScaledValue = temp + 360;
	SpinBack = -ScaledValue;
	Angle(ScaledValue,1000);
	flag0 = 2;
	}
	
	if(flag0==2)
	{
		if(val>2047)//////////here im scaling the value 2047 - 4095 to 3000 - 15000 and this is for Wait time "delay";
		{
		a = val-2047;
		b = 15000-3000;
		c = 4095-2047;
		Numarator = a * b;
		temp = Numarator/c;
		ScaledValue = temp + 3000;
		//Wait(ScaledValue);
		TIM4->CR1 |= TIM_CR1_CEN;
		HD44780_ClrScr();
		PWM_Set3(1,ScaledValue);
		myTicks2 = ScaledValue;
		}
		else//////////here im scaling the value 0 - 2097 to 3000 - 15000 and this is for Wait time "delay";
		{
		a = val-0;
		b = 15000-3000;
		c = 2047-0;
		Numarator = a * b;
		temp = Numarator/c;
		ScaledValue = temp + 3000;
		//Wait(ScaledValue);
		TIM4->CR1 |= TIM_CR1_CEN;
		HD44780_ClrScr();
		PWM_Set3(1,ScaledValue);
		myTicks2 = ScaledValue;
		}
	
	}

}
// FUNCTION      : stopMsg()
// DESCRIPTION   : This function is used to Clear the LCD when it is done 
// PARAMETERS    : nothing 
// RETURNS       : nothing
void stopMsg(void)
{ HD44780_ClrScr();
return;
}
////////////these are the two parser commands to activate the above functions,,,,,
ParserReturnVal_t CmdValveinit(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	Valve_initialise();
  return CmdReturnOk;
}

ADD_CMD("valveinit",CmdValveinit,"            Valve_Positioner initialisation")

ParserReturnVal_t CmdValve_Operation(int mode)
{
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	Valve_Operation();
  return CmdReturnOk;
}

ADD_CMD("valvestart",CmdValve_Operation,"            Starts the valve operation")
