#include "tasks.h"
#include "usb_device.h"
#include "tests.h"

osThreadId DefaultTaskHandle;
osThreadId ProcessCmdTaskHandle;

osTimerId SwTmr01Handle;

osSemaphoreId myBinarySem01Handle;

osSemaphoreId lcdSemHandle;

osSemaphoreId dma1SemHandle;

char rxChar;
extern DMA_HandleTypeDef	hdma1;


/* StartDefaultTask function */
void DefaultTask(void const * argument)
{
	/* init code for USB_DEVICE */
	MX_USB_DEVICE_Init();

	osTimerStart(SwTmr01Handle, 2000);
	char buildVersion[32];
	myLCD_Init();
	myLCD_GotoXY(1, 1);
	osDelay(100);

	sprintf(buildVersion, "STM32 %s", __TIME__);
	myLCD_PrintStr(buildVersion);

	uint8_t count = 0;
	for(;;)
	{
		osDelay(1000);
		HAL_GPIO_TogglePin(GPIOE, LD3_Pin);
		HAL_IWDG_Refresh(&hiwdg);

		sprintf(buildVersion, "%03d", count);
		osSemaphoreWait(lcdSemHandle, portMAX_DELAY);
		myLCD_GotoXY(2, 14);
		myLCD_PrintStr(buildVersion);
		osSemaphoreRelease(lcdSemHandle);
		count++;
	}
}

void ProcessCmdTask(void const * argument)
{
	while(1)
	{
		osSemaphoreWait(myBinarySem01Handle, portMAX_DELAY);
		HAL_GPIO_TogglePin(GPIOE, LD6_Pin);
		osSemaphoreWait(lcdSemHandle, portMAX_DELAY);
		myLCD_GotoXY(2,1);
		myLCD_PutChar(rxChar);
		osSemaphoreRelease(lcdSemHandle);

		switch(rxChar) {
		case 'd':
			DMA1_Test();
			break;

		case 'a':
			ADC2_Test();
			break;

		case 'c':
			myLCD_Clear();
			break;

		default:
			debug_outbyte(rxChar);
			break;
		}
	}
}

/* SwTmr01Callback function */
void SwTmrCallback(void const * argument)
{
	HAL_GPIO_TogglePin(GPIOE, LD4_Pin);
}
