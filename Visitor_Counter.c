#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"


void HD44780_Init();
void HD44780_ClrScr();
void HD44780_GotoXY();
void HD44780_PutStr();

int Read_In_Count1,Read_In_Count2;
int flag1,flag2,flag11,flag22,Inside;
int Algorithm_In,Algorithm_Out;
static volatile int i;

static volatile uint32_t ticks,Mins;
static volatile int32_t In_Count1,In_Count2,Out_Count1,Out_Count2;

void Sensor_initialisation(void *data)
{
	ticks++;
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.Pin = (GPIO_PIN_1| GPIO_PIN_4);
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	//GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	GPIO_InitStruct.Pin = (GPIO_PIN_7| GPIO_PIN_6 |GPIO_PIN_5);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	Read_In_Count1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
	Read_In_Count2 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);

		if(Read_In_Count1 == 1)
		{
		flag1 = 1;

		}
		if(Read_In_Count1 == 0)
		{
		flag1 = flag1-1;
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0);
		}
		if(flag1 == 0)
		{
		In_Count1++;
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
		}

		if(Read_In_Count2 == 1)
		{
		flag2 = 1;
		}
		if(Read_In_Count2 == 0)
		{
		flag2 = flag2-1;
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
		}
		if(flag2==0)
		{
		if(In_Count1 != Out_Count1)
		{		
		//In_Count1--;
		Out_Count1++;
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
		}
		else if (In_Count1 == Out_Count1)
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,1);
		}

		
		if((ticks/10)==60*i)
			{
			Mins++;		
			i++;
			}

		char stringBuffer[16] = { 0 };
		HD44780_GotoXY(4, 0);
		snprintf(stringBuffer, 16, "%ld",ticks/10);
		HD44780_PutStr(stringBuffer);

		HD44780_GotoXY(14, 0);
		snprintf(stringBuffer, 16, "%ld",Mins-1);
		HD44780_PutStr(stringBuffer);

		HD44780_GotoXY(4, 1);
		snprintf(stringBuffer, 16, "%ld", In_Count1);
		HD44780_PutStr(stringBuffer);
		HD44780_GotoXY(13, 1);
		snprintf(stringBuffer, 16, "%ld", Out_Count1);
		HD44780_PutStr(stringBuffer);
		printf("the IN count is %ld\n",Out_Count2);


}

void Screen_initialise(void *data)
{
	HD44780_Init();//to initialise LCD
	PWM_Init();	     ///to initialize pwm
	PWM_Set3(3,100);//this is to enable the LCD backlight
	char stringBuffer[16] = { 0 };
	HD44780_ClrScr();
	HD44780_GotoXY(0, 0);
	snprintf(stringBuffer, 16, "Sec:");
	HD44780_PutStr(stringBuffer);
	HD44780_GotoXY(10, 0);
	HD44780_PutStr("Min:");
	HD44780_GotoXY(0, 1);
	HD44780_PutStr("In:");
	HD44780_GotoXY(8, 1);
	HD44780_PutStr("Out:");
	printf("Screen initialisation successful\n");
}


ADD_TASK(Sensor_initialisation,Screen_initialise,NULL,100,"this is goin to check for the key press continuously")
