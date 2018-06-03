/*
 * stm32f3_disco_sdspi_io.h
 *
 *  Created on: Jun 3, 2018
 *      Author: phamv_000
 */

#ifndef FATFS_DRIVERS_STM32F3_DISCO_SDSPI_IO_H_
#define FATFS_DRIVERS_STM32F3_DISCO_SDSPI_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"


/*############################### SPI_2 #######################################*/
#define F3DISCO_SPIx                                SPI2
#define F3DISCO_SPIx_CLK_ENABLE()                   __HAL_RCC_SPI2_CLK_ENABLE()

#define F3DISCO_SPIx_SCK_AF                         GPIO_AF5_SPI2
#define F3DISCO_SPIx_SCK_GPIO_PORT                  GPIOF
#define F3DISCO_SPIx_SCK_PIN                        GPIO_PIN_9
#define F3DISCO_SPIx_SCK_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOF_CLK_ENABLE()
#define F3DISCO_SPIx_SCK_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOF_CLK_DISABLE()

#define F3DISCO_SPIx_MISO_MOSI_AF                   GPIO_AF5_SPI2
#define F3DISCO_SPIx_MISO_MOSI_GPIO_PORT            GPIOB
#define F3DISCO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define F3DISCO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#define F3DISCO_SPIx_MISO_PIN                       GPIO_PIN_14
#define F3DISCO_SPIx_MOSI_PIN                       GPIO_PIN_15

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define F3DISCO_SPIx_TIMEOUT_MAX                   	1000

/**
 * @brief	Define DMA or Polling ReadWrite
 *
 */
#define SD_READWRITE_POLLING						0

/**
  * @brief  SD Control Interface pins
  */
#define SD_CS_PIN                                 	GPIO_PIN_13
#define SD_CS_GPIO_PORT                           	GPIOB
#define SD_CS_GPIO_CLK_ENABLE()                   	__HAL_RCC_GPIOB_CLK_ENABLE()
#define SD_CS_GPIO_CLK_DISABLE()                  	__HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @brief  SD Control Lines management
  */
#define SD_CS_LOW()       							HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()      							HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)

/* Configure Polling or interrupt/DMA transfer */
#if !SD_READWRITE_POLLING

#define SD_READWRITE_DMA							1	/* If use DMA transfer */

/* Configure interrupts & DMA here*/
#if SD_READWRITE_DMA
#define SDSPI_TxDMA_Handle							DMA1_Channel5
#define SDSPI_RxDMA_Handle							DMA1_Channel4
#define SDSPI_TxDMA_IRQ								DMA1_Channel5_IRQHandler
#define SDSPI_RxDMA_IRQ								DMA1_Channel4_IRQHandler
#define SDSPI_TxDMA_IRQn							DMA1_Channel5_IRQn
#define SDSPI_RxDMA_IRQn							DMA1_Channel4_IRQn
#endif
#define SDSPI_IRQ									SPI2_IRQHandler
#define SDSPI_IRQn									SPI2_IRQn
#endif

#ifdef __cplusplus
}
#endif

#endif /* FATFS_DRIVERS_STM32F3_DISCO_SDSPI_IO_H_ */
