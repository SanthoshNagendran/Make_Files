#include <stdio.h>
#include <stdint.h>
#include <ctype.h>


#ifdef HAL_I2C_MODULE_ENABLED
 #include "stm32f3xx_hal_i2c.h"
#endif /* HAL_I2C_MODULE_ENABLED */

#include "common.h"

#define adxl_address 0x53<<1

I2C_HandleTypeDef hi2c1;
 
uint8_t data_rec[6];
uint8_t chipid=0;
uint8_t x,y,z;
float xg, yg, zg;
char x_char[3], y_char[3], z_char[3];

static void MX_I2C1_Init(void);


static void MX_I2C1_Init()

{ /////////this is for system configuration
  
	  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
 __HAL_RCC_I2C1_CLK_ENABLE();	
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);
    
   hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x0000020B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }


}


void adxl_write (uint8_t reg, uint8_t value)
{
	uint8_t data[2];
	data[0] = reg;
	data[1] = value;
	HAL_I2C_Master_Transmit (&hi2c1, adxl_address, data, 2, 100);
}
 
void adxl_read_values (uint8_t reg)
{
	HAL_I2C_Mem_Read (&hi2c1, adxl_address, reg, 1, (uint8_t *)data_rec, 6, 100);
}
 
void adxl_read_address (uint8_t reg)
{
	HAL_I2C_Mem_Read (&hi2c1, adxl_address, reg, 1, &chipid, 1, 100);
}
 
void adxl_init (void)
{
	adxl_read_address (0x00); // read the DEVID
 
	adxl_write (0x31, 0x01);  // data_format range= +- 4g
	adxl_write (0x2d, 0x00);  // reset all bits
	adxl_write (0x2d, 0x08);  // power_cntl measure and wake up 8hz
 
}
 
void accelerometer(void)
{
 	MX_I2C1_Init(); 
  	adxl_init();  // initialize adxl
  /* USER CODE END WHILE */
 
  /* USER CODE BEGIN 3 */
		adxl_read_values (0x32);
	  x = ((data_rec[1]<<8)|data_rec[0]);
	  y = ((data_rec[3]<<8)|data_rec[2]);
	  z = ((data_rec[5]<<8)|data_rec[4]);
 	printf("X is %d\n",x);
	printf("Y is %d\n",y);
	printf("Z is %d\n",z);
	  xg = x * .0078;
	  yg = y * .0078;
	  zg = z * .0078;
	printf("XG is %f\n",xg);
	printf("YG is %f\n",yg);
	printf("ZG is %f\n",zg);
 
	  HAL_Delay (10);
 
  /* USER CODE END 3 */
 
}


ParserReturnVal_t Cmdaccelerometer(int mode)
	{
  	
		if(mode != CMD_INTERACTIVE) return CmdReturnOk;
		accelerometer();
		return CmdReturnOk;
	}
	
	ADD_CMD("xyz",Cmdaccelerometer,"            shows XYZ axis")
 
