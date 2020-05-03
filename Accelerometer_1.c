#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"

SPI_HandleTypeDef hspi1;

uint8_t data_rec[6];
int16_t x,y,z;
float xg,yg,zg;

static void MX_SPI1_Init(void);
//void SystemConfig(void);
/*
void SystemConfig(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);


  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	printf("System Configuration is initialised successfully\n");
}
*/
static void MX_SPI1_Init(void)
{
 
   hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	printf("SPI is initialised successfully\n"); 
}

void adxl_write (uint8_t address, uint8_t value)
{
	uint8_t data[2];
	data[0] = address|0x40;  // multibyte write
	data[1] = value;
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_6, 0);
	HAL_SPI_Transmit (&hspi1, data, 2, 100);
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_6, 1);
}
 
 
uint8_t adxl_read_chipid (void)
{
	uint8_t address = 0x00;
	uint8_t value;
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_6, 0);
	HAL_SPI_Transmit (&hspi1, &address, 1, 100);
	HAL_SPI_Receive (&hspi1, &value, 1, 100);
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_6, 1);
	return value;
}
 
void adxl_init (void)
{
	adxl_write (0x31, 0x01);  // data_format range= +- 4g
	adxl_write (0x2d, 0x00);  // reset all bits
	adxl_write (0x2d, 0x08);  // power_cntl measure and wake up 8hz
}
 
void adxl_read (uint8_t address)
{
	address |= 0x80;  // read operation
	address |= 0x40;  // multibyte read
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_6, 0);
	HAL_SPI_Transmit (&hspi1, &address, 1, 100);
	HAL_SPI_Receive (&hspi1, data_rec, 6, 100);
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_6, 1);
}
 
void Axis_Init(void)
{



 GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	MX_SPI1_Init();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
	// = adxl_read_chipid ();
	adxl_init ();
	printf("Accelerometer is initialised successfully\n");
}

void Axis_Read(void)
{	
	  adxl_read (0x32);
	  x = ((data_rec[1]<<8)|data_rec[0]);
	  y = ((data_rec[3]<<8)|data_rec[2]);
	  z = ((data_rec[5]<<8)|data_rec[4]);
	printf("X is %d\n",x);
	printf("Y is %d\n",y);
	printf("Z is %d\n",z);
	  xg = x*.0078;
          yg = y*.0078;
          zg = z*.0078;
	printf("XG is %f\n",xg);
	printf("YG is %f\n",yg);
	printf("ZG is %f\n",zg);
}

ParserReturnVal_t Cmdaccelerometerinit(int mode)
	{
  	
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		Axis_Init();
		return CmdReturnOk;
	}
	
	ADD_CMD("xyzinit",Cmdaccelerometerinit,"            initialises XYZ axis")

ParserReturnVal_t Cmdaccelerometer(int mode)
	{
  	
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		Axis_Read();
		return CmdReturnOk;
	}
	
	ADD_CMD("xyz",Cmdaccelerometer,"            shows XYZ axis")
 

