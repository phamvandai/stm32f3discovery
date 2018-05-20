#ifndef TASKS_H
#define TASKS_H

#include "main.h"
#include "stm32f3xx.h"
#include "cmsis_os.h"
#include "Libs/mylcd.h"
#include "Libs/debug.h"

extern osThreadId 		DefaultTaskHandle;
extern osThreadId 		ProcessCmdTaskHandle;

extern osTimerId 		SwTmr01Handle;

extern osSemaphoreId 	myBinarySem01Handle;
extern osSemaphoreId 	lcdSemHandle;
extern osSemaphoreId 	dma1SemHandle;

extern IWDG_HandleTypeDef 	hiwdg;
extern DMA_HandleTypeDef 	hdma_adc2;
extern ADC_HandleTypeDef 	hadc2;
extern DMA_HandleTypeDef	hdma1;
extern char rxChar;

void DefaultTask(void const * argument);
void ProcessCmdTask(void const *argument);
void SwTmrCallback(void const * argument);

#endif
