/*
 * stm32f3_disco_sdspi_io.c
 *
 *  Created on: Jun 3, 2018
 *      Author: phamv_000
 */

#ifndef FATFS_DRIVERS_STM32F3_DISCO_SDSPI_IO_C_
#define FATFS_DRIVERS_STM32F3_DISCO_SDSPI_IO_C_

#include "stm32f3_disco_sdspi_io.h"

/**
  * @brief LINK SD Card
  */
#define SD_DUMMY_BYTE            0xFF
#define SD_NO_RESPONSE_EXPECTED  0x80


/**
 * @brief BUS variables
 */

uint32_t SpixTimeout = F3DISCO_SPIx_TIMEOUT_MAX; /*<! Value of Timeout when SPI communication fails */
static SPI_HandleTypeDef hspi_sd;

/* Private Function Prototypes */
static void       SPIx_Init(void);
static void       SPIx_MspInit(SPI_HandleTypeDef *hspi);
static void       SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);
static void       SPIx_Error(void);

/* SD IO functions */
void              SD_IO_Init(void);
void              SD_IO_CSState(uint8_t state);
void              SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);
uint8_t           SD_IO_WriteByte(uint8_t Data);

#if SD_READWRITE_POLLING == 0
#if SD_READWRITE_DMA
static DMA_HandleTypeDef hdma_spisd_tx;
static DMA_HandleTypeDef hdma_spisd_rx;
#endif
static volatile int transfer_done = 0;

/* SD SPI With DMA & INTERRUPT*/
#if SD_READWRITE_DMA
/**
* @brief This function handles DMA1 channel4 global interrupt.
*/
void SDSPI_RxDMA_IRQ(void)
{
  HAL_DMA_IRQHandler(&hdma_spisd_rx);
}

/**
* @brief This function handles DMA1 channel5 global interrupt.
*/
void SDSPI_TxDMA_IRQ(void)
{
  HAL_DMA_IRQHandler(&hdma_spisd_tx);
}

#endif

/**
* @brief This function handles SPI2 global interrupt.
*/
void SDSPI_IRQ(void)
{
  HAL_SPI_IRQHandler(&hspi_sd);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi == &hspi_sd) {
		transfer_done = 1;
	}
}

#endif

/******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/

/******************************* SPI ********************************/

/**
  * @brief  Initialize SPI MSP.
  * @retval None
  */
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  F3DISCO_SPIx_SCK_GPIO_CLK_ENABLE();
  F3DISCO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();

  /* Configure SPI SCK */
  GPIO_InitStruct.Pin 			= F3DISCO_SPIx_SCK_PIN;
  GPIO_InitStruct.Mode 			= GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull  		= GPIO_PULLUP;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate 	= F3DISCO_SPIx_SCK_AF;
  HAL_GPIO_Init(F3DISCO_SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

  /* Configure SPI MISO and MOSI */
  GPIO_InitStruct.Pin 			= F3DISCO_SPIx_MOSI_PIN;
  GPIO_InitStruct.Alternate 	= F3DISCO_SPIx_MISO_MOSI_AF;
  GPIO_InitStruct.Pull  		= GPIO_PULLDOWN;
  HAL_GPIO_Init(F3DISCO_SPIx_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin 			= F3DISCO_SPIx_MISO_PIN;
  GPIO_InitStruct.Pull  		= GPIO_PULLDOWN;
  HAL_GPIO_Init(F3DISCO_SPIx_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);

  /*** Configure the SPI peripheral ***/
  /* Enable SPI clock */
  F3DISCO_SPIx_CLK_ENABLE();
}


/**
  * @brief  Initialize SPI HAL.
  * @retval None
  */
static void SPIx_Init(void)
{
  if (HAL_SPI_GetState(&hspi_sd) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    hspi_sd.Instance = F3DISCO_SPIx;

    hspi_sd.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_2;
    hspi_sd.Init.Direction 			= SPI_DIRECTION_2LINES;
    hspi_sd.Init.CLKPhase 			= SPI_PHASE_2EDGE;
    hspi_sd.Init.CLKPolarity 		= SPI_POLARITY_HIGH;
    hspi_sd.Init.CRCCalculation 	= SPI_CRCCALCULATION_DISABLE;
    hspi_sd.Init.CRCPolynomial 		= 7;
    hspi_sd.Init.DataSize 			= SPI_DATASIZE_8BIT;
    hspi_sd.Init.FirstBit 			= SPI_FIRSTBIT_MSB;
    hspi_sd.Init.NSS 				= SPI_NSS_SOFT;
    hspi_sd.Init.TIMode 			= SPI_TIMODE_DISABLE;
    hspi_sd.Init.Mode 				= SPI_MODE_MASTER;

#if (SD_READWRITE_POLLING == 0)
#if (SD_READWRITE_DMA)
    /* SPI2_TX Init */
    hdma_spisd_tx.Instance 			= SDSPI_TxDMA_Handle;
    hdma_spisd_tx.Init.Direction 	= DMA_MEMORY_TO_PERIPH;
    hdma_spisd_tx.Init.PeriphInc 	= DMA_PINC_DISABLE;
    hdma_spisd_tx.Init.MemInc 		= DMA_MINC_ENABLE;
    hdma_spisd_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spisd_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spisd_tx.Init.Mode 		= DMA_NORMAL;
    hdma_spisd_tx.Init.Priority 	= DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_spisd_tx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(&hspi_sd, hdmatx, hdma_spisd_tx);


    /* SPI2_RX Init */
    hdma_spisd_rx.Instance 			= SDSPI_RxDMA_Handle;
    hdma_spisd_rx.Init.Direction 	= DMA_PERIPH_TO_MEMORY;
    hdma_spisd_rx.Init.PeriphInc 	= DMA_PINC_DISABLE;
    hdma_spisd_rx.Init.MemInc 		= DMA_MINC_ENABLE;
    hdma_spisd_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spisd_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spisd_rx.Init.Mode 		= DMA_NORMAL;
    hdma_spisd_rx.Init.Priority 	= DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_spisd_rx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(&hspi_sd, hdmarx, hdma_spisd_rx);

    HAL_NVIC_SetPriority(SDSPI_TxDMA_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(SDSPI_TxDMA_IRQn);

    HAL_NVIC_SetPriority(SDSPI_RxDMA_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SDSPI_RxDMA_IRQn);
#endif /* SD_READWRITE_DMA */

    /* SPI2 interrupt Init */
    HAL_NVIC_SetPriority(SDSPI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SDSPI_IRQn);

#endif

    SPIx_MspInit(&hspi_sd);
    HAL_SPI_Init(&hspi_sd);
  }
}

/**
  * @brief  SPI Write byte(s) to device
  * @param  DataIn: Pointer to data buffer to write
  * @param  DataOut: Pointer to data buffer for read data
  * @param  DataLength: number of bytes to write
  * @retval None
  */
static void SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

#if SD_READWRITE_POLLING
  status = HAL_SPI_TransmitReceive(&hspi_sd, (uint8_t *) DataIn, DataOut, DataLength, SpixTimeout);

#else
  /* Polling read */
  if(DataLength < 256) {
	  status = HAL_SPI_TransmitReceive(&hspi_sd, (uint8_t *) DataIn, DataOut, DataLength, SpixTimeout);
  }
  /* DMA/IT Read*/
  else {
#if (SD_READWRITE_DMA == 0)
	  transfer_done = 0;
	  status = HAL_SPI_TransmitReceive_IT(&hspi_sd, (uint8_t *) DataIn, DataOut, DataLength);
	  while (transfer_done == 0);

#else

	  transfer_done = 0;
	  status = HAL_SPI_TransmitReceive_DMA(&hspi_sd, (uint8_t *) DataIn, DataOut, DataLength);
	  while(transfer_done == 0);
#endif /* SD_READWRITE_DMA */
  }
#endif	/* SD_READWRITE_POLLING */

  /* Check the communication status */
  if (status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}


/**
  * @brief  SPI error treatment function
  * @retval None
  */
static void SPIx_Error(void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&hspi_sd);

  /* Re-Initiaize the SPI communication BUS */
  SPIx_Init();
}

/******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/

/********************************* LINK SD ************************************/
/**
  * @brief  Initialize the SD Card and put it into StandBy State (Ready for
  *         data transfer).
  * @retval None
  */
void SD_IO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  uint8_t counter;

  /* SD_CS_GPIO Periph clock enable */
  SD_CS_GPIO_CLK_ENABLE();

  /* Configure SD_CS_PIN pin: SD Card CS pin */
  GPIO_InitStruct.Pin 			= SD_CS_PIN;
  GPIO_InitStruct.Mode 			= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 			= GPIO_PULLUP;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SD_CS_GPIO_PORT, &GPIO_InitStruct);

  /*------------Put SD in SPI mode--------------*/
  /* SD SPI Config */
  SPIx_Init();

  /* SD chip select high */
  SD_CS_HIGH();

  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (counter = 0; counter <= 9; counter++)
  {
    /* Send dummy byte 0xFF */
    SD_IO_WriteByte(SD_DUMMY_BYTE);
  }
}

/**
  * @brief  Set the SD_CS pin.
  * @param  val pin value.
  * @retval None
  */
void SD_IO_CSState(uint8_t val)
{
  if (val == 1)
  {
    SD_CS_HIGH();
  }
  else
  {
    SD_CS_LOW();
  }
}

/**
  * @brief  Write bytes on the SD.
  * @param  DataIn     Input Data buffer pointer.
  * @param  DataOut    Output Data buffer pointer.
  * @param  DataLength Number of byte to send.
  * @retval None
  */
void SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  /* Send the byte */
  SPIx_WriteReadData(DataIn, DataOut, DataLength);
}

/**
  * @brief  Write a byte on the SD.
  * @param  Data byte to send.
  * @retval None
  */
uint8_t SD_IO_WriteByte(uint8_t Data)
{
  uint8_t tmp;
  /* Send the byte */
  SPIx_WriteReadData(&Data, &tmp, 1);
  return tmp;
}


#endif /* FATFS_DRIVERS_STM32F3_DISCO_SDSPI_IO_C_ */
