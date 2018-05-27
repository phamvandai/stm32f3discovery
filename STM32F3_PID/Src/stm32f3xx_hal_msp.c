/**

  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"


/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
	/* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}
