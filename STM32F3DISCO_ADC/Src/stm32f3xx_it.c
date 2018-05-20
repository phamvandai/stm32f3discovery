/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
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
#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"
#include "stm32f3xx_it.h"
#include "cmsis_os.h"
#include "Libs/debug.h"
#include "tests.h"

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;
extern DMA_HandleTypeDef hdma_adc2;
extern ADC_HandleTypeDef hadc2;
extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  HAL_IncTick();
  osSystickHandler();
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles ADC1 and ADC2 interrupts.
*/
extern volatile int ADC2_Chan15_EOS;
void ADC1_2_IRQHandler(void)
{
#if ADC2_INTR_MODE
	HAL_ADC_IRQHandler(&hadc2);
#endif
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc == &hadc2) {
		ADC2_Chan15_EOS = 1;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc == &hadc2) {
		ADC2_Chan15_EOS = 1;
	}
}
/**
* @brief This function handles USB low priority or CAN_RX0 interrupts.
*/
void USB_LP_CAN_RX0_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

/**
* @brief This function handles I2C1 event global interrupt / I2C1 wake-up interrupt through EXTI line 23.
*/
void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c1);
}

/**
* @brief This function handles I2C1 error interrupt.
*/
void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c1);
}

/**
* @brief This function handles SPI1 global interrupt.
*/
void SPI1_IRQHandler(void)
{
  /* USER CODE BEGIN SPI1_IRQn 0 */

  /* USER CODE END SPI1_IRQn 0 */
  HAL_SPI_IRQHandler(&hspi1);
  /* USER CODE BEGIN SPI1_IRQn 1 */

  /* USER CODE END SPI1_IRQn 1 */
}

/**
* @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
*/
extern osSemaphoreId myBinarySem01Handle;
extern char rxChar;
void USART1_IRQHandler(void)
{
	uint32_t isrflags   = READ_REG(huart1.Instance->ISR);
	if(isrflags & USART_ISR_RXNE) {
		rxChar = (char) READ_REG(huart1.Instance->RDR);
		HAL_GPIO_TogglePin(GPIOE, LD7_Pin);
		osSemaphoreRelease(myBinarySem01Handle);
	}
}

/**
* @brief This function handles DMA2 channel1 global interrupt.
*/
void DMA2_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_adc2);
}


extern DMA_HandleTypeDef	hdma1;
void DMA1_Channel1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma1);
}

extern osSemaphoreId dma1SemHandle;
void DMA1Chan1_Complete(struct __DMA_HandleTypeDef * hdma)
{
	osSemaphoreRelease(dma1SemHandle);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
