#include "main.h"


/** System Clock Configuration
	* System Clock source: HSE 
	* HSE Frequency: 8MHz
	* PLLMUL: 9 (RCC_PLL_MUL_9)
	* SYSCLCK(Hz): 72 MHz
	* AHB Prescaler: 1
	* APB1 Prescaler: 2
	* APB2 Prescaler: 2
	* TIM2,3,4,6,7 CLCK: 72MHz
	* USART1 CLCK: 36 MHz
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

/** Configure GPIO
*/
void GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__GPIOA_CLK_ENABLE();
	
	HAL_NVIC_SetPriority(EXTI0_IRQn,6,0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	/*Configure GPIO pin : PA0 -User button*/
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*Configure PA2 as TIM2 PWM Chanel 3 - motor EN*/
	GPIO_InitStruct.Pin=GPIO_PIN_2;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate=GPIO_AF1_TIM2; //NOTE: See datasheet -alternative functions for PORTA
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	/*Configure PA3 as one motor IN 1, PA4 as motor IN2*/
	GPIO_InitStruct.Pin=GPIO_PIN_3|GPIO_PIN_4;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	/*Configure PD12 and PD13 as TIM4 channel 1 and 2
	* Encoder A <---> PD12
	* Encoder B <---> PD13
	*/
	__GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin=GPIO_PIN_12|GPIO_PIN_13;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
	//See STM32F303x Datasheet Pinout alternative function descrition
	GPIO_InitStruct.Alternate=GPIO_AF2_TIM4;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	
	/*UART1 PINS: PC4 <->Tx, PC5 <->Rx*/
	__GPIOC_CLK_ENABLE();
	GPIO_InitStruct.Pin=GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Alternate=GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
}


/**
	Init Timer 3 Time base
**/
void TIM3_BaseInit(void)
{	
	/*Enable clock*/
	__TIM3_CLK_ENABLE();

	/* Set Interrupt Group Priority */ 
	HAL_NVIC_SetPriority(TIM3_IRQn, 5, 0);

	/* Enable the TIM3 global Interrupt */
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
	/*Set timer frequency f=10KHz and update period=20mc*/
	/*SystemClock_Config() sets SystemCoreClock=72MHz*/
	TIM3_Handle.Instance=TIM3;
	TIM3_Handle.Init.Prescaler=(SystemCoreClock / 10000) - 1;
	TIM3_Handle.Init.ClockDivision=0;
	TIM3_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
	TIM3_Handle.Init.Period= 250-1; //0.025s

	HAL_TIM_Base_Init(&TIM3_Handle);
	
}

/*
	Init Timer 2 PWM Mode
*/

void TIM2_PWMInit(void)
{	
	__TIM2_CLK_ENABLE();
	TIM_MasterConfigTypeDef sMasterConfig;
	
	TIM2_Handle.Instance=TIM2;
	TIM2_Handle.Init.Prescaler=(SystemCoreClock/1000000)-1; //  (1MHz)
	TIM2_Handle.Init.Period=5000-1; //5ms PWM freq=200Hz
	TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
	HAL_TIM_PWM_Init(&TIM2_Handle);
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM2_Handle, &sMasterConfig);
	
	TIM2_PWM_InitStruct.OCMode=TIM_OCMODE_PWM1;
	TIM2_PWM_InitStruct.OCPolarity=TIM_OCPOLARITY_HIGH;
	TIM2_PWM_InitStruct.OCFastMode=TIM_OCFAST_DISABLE;
	TIM2_PWM_InitStruct.Pulse=PWM_Pulse;
	HAL_TIM_PWM_ConfigChannel(&TIM2_Handle,&TIM2_PWM_InitStruct,TIM_CHANNEL_3);
	
}

/*
* Init Timer 4 in Counter Mode
*/
void TIM4_EncInit(void)
{	
	TIM_Encoder_InitTypeDef TIM4_Enc_InitStruct;
	
	__TIM4_CLK_ENABLE();
	TIM4_Handle.Instance=TIM4;
	TIM4_Handle.Init.Prescaler=0;
	TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
	TIM4_Handle.Init.Period=0xFFFF; //Set max value of ARR register

	/*Encoder 's resolution 888 p/r;
	* TIM->CNT: 888*2 p/r;
	*/
	TIM4_Enc_InitStruct.EncoderMode=TIM_ENCODERMODE_TI1;
	TIM4_Enc_InitStruct.IC1Filter=0;
	TIM4_Enc_InitStruct.IC1Polarity=TIM_ICPOLARITY_RISING;
	TIM4_Enc_InitStruct.IC1Prescaler=TIM_ICPSC_DIV1;
	TIM4_Enc_InitStruct.IC1Selection=TIM_ICSELECTION_DIRECTTI;
	TIM4_Enc_InitStruct.IC2Filter=0;
	TIM4_Enc_InitStruct.IC2Polarity=TIM_ICPOLARITY_RISING;
	TIM4_Enc_InitStruct.IC2Prescaler=TIM_ICPSC_DIV1;
	TIM4_Enc_InitStruct.IC2Selection=TIM_ICSELECTION_DIRECTTI;
	
	HAL_TIM_Encoder_Init(&TIM4_Handle,&TIM4_Enc_InitStruct);
	
}

/*
* Init UART1 
*/
void UART1_Init(void)
{
	__USART1_CLK_ENABLE();
	/* Set Interrupt Group Priority */ 
	HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);

  /* Enable the TIM3 global Interrupt */
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	
	UART1_Handle.Instance=USART1;
	UART1_Handle.Init.BaudRate=38400;
	UART1_Handle.Init.WordLength=UART_WORDLENGTH_8B;
	UART1_Handle.Init.StopBits=UART_STOPBITS_1;
	UART1_Handle.Init.Parity=UART_PARITY_NONE;
	UART1_Handle.Init.Mode=UART_MODE_TX_RX;
	UART1_Handle.Init.HwFlowCtl= UART_HWCONTROL_NONE;
	UART1_Handle.Init.OverSampling= UART_OVERSAMPLING_16;
	UART1_Handle.Init.OneBitSampling=UART_ONEBIT_SAMPLING_DISABLED;
	UART1_Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&UART1_Handle);
	
}
