
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include "Libs/debug.h"
#include "BSP/stm32f3_discovery.h"
#include "Libs/ds1307.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef 	hi2c1;
DMA_HandleTypeDef 	hdma_i2c1_rx;
DMA_HandleTypeDef 	hdma_i2c1_tx;
SPI_HandleTypeDef 	hspi1;
UART_HandleTypeDef 	huart1;
PCD_HandleTypeDef 	hpcd_USB_FS;

#define ACCLELERO_I2C_ADDR		(0x19 << 1)
#define COMPASS_I2C_ADDR		(0x1E << 1)
#define DS1307_I2C_ADDR			(0x68 << 1)

/* AT24C32 EEPROM */
#define EEPROM_I2C_ADDR			(0x50 << 1)
#define EEPROM_PAGE_COUNT		128
#define EEPROM_PAGE_SIZE		32

extern uint32_t 	I2cxTimeout;
static uint8_t		buffer[32];
static uint16_t 	page_idx = 0;
static DateTime_t 	dt;

extern void      	COMPASSACCELERO_IO_Init(void);
extern void      	COMPASSACCELERO_IO_ITConfig(void);
extern void      	COMPASSACCELERO_IO_Write(uint16_t DeviceAddr, uint8_t RegisterAddr, uint8_t Value);
extern uint8_t   	COMPASSACCELERO_IO_Read(uint16_t DeviceAddr, uint8_t RegisterAddr);

extern void      	GYRO_IO_Init(void);
extern void      	GYRO_IO_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
extern void      	GYRO_IO_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_USART1_UART_Init(void);
static void Test_I2C1(void);
static void Test_SPI1_Config(void);
static void Test_SPI1_Read(void);

/* Test config */
#define I2C_BUILTIN_LIB		0
#define EEPROM_PREFILL		0
#define GYRO_TEST			1

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_DMA_Init();

#if !I2C_BUILTIN_LIB
	MX_I2C1_Init();
#endif

#if GYRO_TEST
	GYRO_IO_Init();
#else
	MX_SPI1_Init();
#endif
	MX_USB_PCD_Init();

#if I2C_BUILTIN_LIB
	COMPASSACCELERO_IO_Init();
	debug_print("COMPASSACCELERO\r\n");
	uint8_t val = 0xFF;
	uint8_t reg_addr = 0;
#endif

#if EEPROM_PREFILL
	int i = 0, j;
	HAL_StatusTypeDef status;

	for (i = 0; i < EEPROM_PAGE_COUNT; i++) {
		for(j = 0; j < 32; j++) {
			buffer[j] = i + j;
		}

		status = HAL_I2C_Mem_Write(&hi2c1, EEPROM_I2C_ADDR, i * 32,
								   I2C_MEMADD_SIZE_16BIT, buffer, 32, I2cxTimeout);
		if(status == HAL_OK) {
			debug_printf("fill page %d\r\n", i);
		}

		HAL_Delay(100);
	}
#endif

	/* Test GYROSCOPE on SPI1*/
#if GYRO_TEST
	  Test_SPI1_Config();
#endif

	while (1)
	{
#if GYRO_TEST
	  Test_SPI1_Read();
#endif

#if I2C_BUILTIN_LIB
	  for(reg_addr = 0x20; reg_addr < 0x3D; reg_addr++) {
		  val = COMPASSACCELERO_IO_Read(ACCELERO_I2C_ADDRESS, reg_addr);
		  if(val > 0) {
			  debug_printf("ACCELERO 0x%02x: 0x%02x\r\n", reg_addr, val);
		  }
		  HAL_Delay(100);
	  }

	  for(reg_addr = 0x00; reg_addr < 0x0C; reg_addr++) {
		  val = COMPASSACCELERO_IO_Read(COMPASS_I2C_ADDR, reg_addr);
		  if(val > 0) {
			  debug_printf("COMPASS 0x%02x: 0x%02x\r\n", reg_addr, val);
		  }
		  HAL_Delay(100);
	  }

	  HAL_Delay(5000);
#else
	 Test_I2C1();
#endif
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  	  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
							  |RCC_PERIPHCLK_I2C1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure the Systick interrupt time
	*/
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* NOTE: VOLATILE!!! */
static volatile uint8_t I2C1_RxDone = 0;
static volatile uint8_t I2C1_TxDone = 0;

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == &hi2c1) {
		I2C1_RxDone = 1;
	}
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == &hi2c1) {
		I2C1_TxDone = 1;
	}
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{
	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_RESET) {
		hi2c1.Instance = I2C1;
		hi2c1.Init.OwnAddress1 		= ACCELERO_I2C_ADDRESS;
		hi2c1.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
		hi2c1.Init.DualAddressMode 	= I2C_DUALADDRESS_DISABLE;
		hi2c1.Init.OwnAddress2		= 0;
		hi2c1.Init.GeneralCallMode 	= I2C_GENERALCALL_DISABLE;
		hi2c1.Init.NoStretchMode 	= I2C_NOSTRETCH_DISABLE;
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

		HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

		HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
	}
}

/**
 * @brief Test on-board Accelerator & compass; Test DS1307 & AT24C32 on I2C1
 */
static void Test_I2C1(void)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t val = 0;
	uint8_t reg_addr;
	int i = 0;

	/* Read accelerometer registers */
	for(reg_addr = 0x20; reg_addr < 0x3D; reg_addr++) {
	  status = HAL_I2C_Mem_Read(&hi2c1, ACCELERO_I2C_ADDRESS, reg_addr, I2C_MEMADD_SIZE_8BIT, &val, 1, I2cxTimeout);
	  if(val > 0) {
		  debug_printf("ACCELERO 0x%02x: 0x%02x\r\n", reg_addr, val);
	  }
	  HAL_Delay(50);
	}

	/* Read compass registers */
	for(reg_addr = 0x00; reg_addr < 0x0C; reg_addr++) {
	  status = HAL_I2C_Mem_Read(&hi2c1, COMPASS_I2C_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &val, 1, I2cxTimeout);
	  if(status == HAL_OK && val > 0) {
		  debug_printf("COMPASS 0x%02x: 0x%02x\r\n", reg_addr, val);
	  }
	  HAL_Delay(50);
	}

	/* Read RTC Time */
	status = HAL_I2C_Mem_Read(&hi2c1, DS1307_I2C_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, buffer, 32, I2cxTimeout);
	if(status == HAL_OK && val > 0) {
	  debug_printf("\r\nRTC: ");
	  DS1307_GetDateTime(buffer, &dt);
	  debug_printf("%d:%d:%d - %d/%d/%d\r\n", dt.hour, dt.min, dt.sec, dt.date, dt.month, dt.year);
	}

	/* Read EEPROM all pages */
	I2C1_RxDone = 0;
	status = HAL_I2C_Mem_Read_DMA(&hi2c1, EEPROM_I2C_ADDR, page_idx * 32, I2C_MEMADD_SIZE_16BIT, buffer, 32);
	if(status == HAL_OK) {
		while(I2C1_RxDone == 0);
		debug_printf("\r\nEEPROM page %d: ", page_idx);
		page_idx++;
		if(page_idx >= EEPROM_PAGE_COUNT) {
		  page_idx = 0;
		}

		for(i = 0; i < 32; i++) {
		  debug_printf("0x%02x ", buffer[i]);
		}
	}

	debug_print("\r\n");
	HAL_Delay(1000);
}

static void Test_SPI1_Config(void)
{
	uint8_t 	val = 0, addr = 0;

	GYRO_IO_Read(&val, 0x0F, 1);

	if(val == 0xD4) {
		debug_print("GYRO L3GD20\r\n");
	}
	else {
		debug_print("Other GYRO\r\n");
	}

	for(addr = 0x20; addr <= 0x38; addr++) {
		GYRO_IO_Read(&val, addr, 1);
		debug_printf("GYRO 0x%02x : 0x%02x\r\n", addr, val);
	}

	val = 0x0F;
	GYRO_IO_Write(&val, 0x20, 1);
	debug_print("Enabled GYRO\r\n=====================\r\n");
}

static void Test_SPI1_Read(void)
{
	uint8_t 	regs[2];
	uint16_t 	data;

	debug_print("\r\n--------------------\r\n");
	GYRO_IO_Read(regs, 0x28, 2);
	data = regs[0] | regs[1] << 8;
	debug_printf("X: %d, ", data);

	GYRO_IO_Read(regs, 0x2A, 2);
	data = regs[0] | regs[1] << 8;
	debug_printf("Y: %d, ", data);

	GYRO_IO_Read(regs, 0x2C, 2);
	data = regs[0] | regs[1] << 8;
	debug_printf("Z: %d\r\n", data);
	debug_print("--------------------\r\n");
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

	/* SPI1 parameter configuration*/
	hspi1.Instance 			= SPI1;
	hspi1.Init.Mode 		= SPI_MODE_MASTER;
	hspi1.Init.Direction 	= SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize 	= SPI_DATASIZE_4BIT;
	hspi1.Init.CLKPolarity 	= SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase 	= SPI_PHASE_1EDGE;
	hspi1.Init.NSS 			= SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi1.Init.FirstBit 	= SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode 		= SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength 	= SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode 	= SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USB init function */
static void MX_USB_PCD_Init(void)
{

  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.ep0_mps = DEP0CTL_MPS_64;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin 
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin 
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT1_Pin 
                           MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT1_Pin 
                          |MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin 
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin 
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin 
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin 
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
