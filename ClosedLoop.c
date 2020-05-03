#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

static int32_t Previous_Reading,Reading,Sensor,Error,Control,Kp,Set_Point;

void PID_Init(void *data)
{
	PWM_Init();
	EncoderInit();
	DCMotorInit();
}

void PID_Task(void *data)
{
	Reading = encoders();
	Sensor = Reading - Previous_Reading;
	Previous_Reading = Reading;
	Error = Set_Point - Sensor;
	Control = Kp*Error;
	
	/*if(control<=0)//////////this is for speed control using P logic
	{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);
	PWM_Set4(1,control);
	}
	else
	{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
	PWM_Set4(1,-control);
	}*/
////////////this is for position control using P control logic.
	if(Control<0)
	{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
	PWM_Set3(1,100);
		if(Reading<Control)
		{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
		PWM_Set3(1,0);
		}
	}
	if(Reading<=Control)
	{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);
	PWM_Set3(1,100);
	}
	else if (Reading>Control)
	{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
	PWM_Set3(1,100);
	}
	else if (Reading==Control)
	{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
	PWM_Set3(1,0);
	}

	if(Control==0)
	{
	PWM_Set3(1,0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
	
	}
}
ADD_TASK (PID_Task,PID_Init,NULL,100,"this is going to start the motor to control the speed by itself")

ParserReturnVal_t CmdGetParameters(int mode)
{
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;
	fetch_int32_arg(&Set_Point);
	fetch_int32_arg(&Kp);
  return CmdReturnOk;
}

ADD_CMD("parameters",CmdGetParameters,"            this is to give the parameters")
