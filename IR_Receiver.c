#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "common.h"
I2C_HandleTypeDef hi2c1;

uint32_t Data;
uint8_t count;
static void MX_I2C1_Init(void);

static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
 // hi2c1.Init.ClockSpeed = 100000;
  //hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

uint32_t DWT_Delay_Init(void) {
  /* Disable TRC */
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
  /* Enable TRC */
  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

  /* Disable clock cycle counter */
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
  /* Enable  clock cycle counter */
  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;

     /* 3 NO OPERATION instructions */
     __ASM volatile ("NOP");
     __ASM volatile ("NOP");
  __ASM volatile ("NOP");

  /* Check if clock cycle counter has started */
     if(DWT->CYCCNT)
     {
       return 0; /*clock cycle counter started*/
     }
     else
  {
    return 1; /*clock cycle counter not started*/
  }
}


uint32_t receive_data (void)
{
	uint32_t code=0;
	
		  /* The START Sequence begin here
	   * there will be a pulse of 9ms LOW and
	   * than 4.5 ms space (HIGH)
	   */
	  while (!(HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)))
{;}  // wait for the pin to go high.. 9ms LOW

	  while ((HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)))
{;}  // wait for the pin to go low.. 4.5ms HIGH

	  /* START of FRAME ends here*/

	  /* DATA Reception
	   * We are only going to check the SPACE after 562.5us pulse
	   * if the space is 562.5us, the bit indicates '0'
	   * if the space is around 1.6ms, the bit is '1'
	   */

	  for (int i=0; i<32; i++)
	  {
		  count=0;

		  while (!(HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)))
{;} // wait for pin to go high.. this is 562.5us LOW

		  while ((HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1)))  // count the space length while the pin is high
		  {
			  count++;
			  HAL_Delay(100);
		  }

		  if (count > 12) // if the space is more than 1.2 ms
		  {
			  code |= (1UL << (31-i));   // write 1
		  }

		  else code &= ~(1UL << (31-i));  // write 0
	  }
		
		return code;
}

void convert_code (uint32_t code)
{
		switch (code)
		{
			case (0xFFA25D):
				printf("1\n");
				//lcd_send_cmd (0x86);
				//lcd_send_data ('1');
				break;
			
			case (0xFF629D):
				printf("2\n");
				//lcd_send_cmd (0x86);
				//lcd_send_data ('2');
				break;			

			case (0xFFE21D):
				printf("3\n");			
				//lcd_send_cmd (0x86);
				//lcd_send_data ('3');
				break;

			case (0xFF22DD):
				//lcd_send_cmd (0x86);
				printf("4\n");
				break;

			case (0xFF02FD):
				//lcd_send_cmd (0x86);
				printf("5\n");
				break;

			case (0xFFC23D):
				//lcd_send_cmd (0x86);
				printf("6\n");
				break;

			case (0xFFE01F):
				//lcd_send_cmd (0x86);
				printf("7\n");
				break;

			case (0xFFA857):
				//lcd_send_cmd (0x86);
				printf("8\n");
				break;

			case (0xFF906F):
				//lcd_send_cmd (0x86);
				printf("9\n");
				break;

			case (0xFFB04F):
				//lcd_send_cmd (0x86);
				printf("#\n");
				break;

			case (0XFF6897):
				//lcd_send_cmd (0x86);
				printf("*\n");
				break;			
						
			case (0xFF9867):
				//lcd_send_cmd (0x86);
				printf("0\n");
				break;

			case (0xFF38C7):
				//lcd_send_cmd (0x86);
				printf("OK\n");
				break;

			case (0xFF18E7):
				//lcd_send_cmd (0x86);
				printf("UP\n");
				break;
			
			case (0xFF10EF):
				//lcd_send_cmd (0x86);
				printf("LEFT\n");
				break;			

			case (0xFF5AA5):
				//lcd_send_cmd (0x86);
				printf("RIGHT\n");
				break;
			
			case (0xFF4AB5):
				//lcd_send_cmd (0x86);
				//lcd_send_data ('u');
				break;
			
			default :
				break;
		}
}


void PinConfig(void *data)
{

	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  //RCC_OscInitStruct.HSEPredivValue = RCC_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }	

	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
 	MX_I2C1_Init();
	DWT_Delay_Init ();
}
void ProcessData (void *data)
{
	  while (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1))
		{
		;
		}   // wait for the pin to go low
		
		Data = receive_data ();
		
		convert_code (Data);
		HAL_Delay (200);
}
ADD_TASK(ProcessData,PinConfig,NULL,100,"this is goin to check for the key press continuously")

