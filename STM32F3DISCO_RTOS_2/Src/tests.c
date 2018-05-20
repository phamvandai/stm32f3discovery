/*
 * tests.c
 *
 *  Created on: May 15, 2018
 *      Author: phamv_000
 */

#include "tests.h"
#include <string.h>
#include "Libs/cycle_counter.h"
#include "Libs/mylcd.h"

#define BUF_MAX_SIZE 128

static int src_buf[BUF_MAX_SIZE];
static int dst_buf[BUF_MAX_SIZE];
static int startVal = 1;
static int  length = 16;

void DMA1_Test(void)
{
	debug_print("DMA1 M2M test\r\n");
	int i;

	memset(src_buf, 0, sizeof(src_buf));
	memset(dst_buf, 0, sizeof(dst_buf));

	reset_timer();
	start_timer();
	for(i = 0; i < length; i++) {
		src_buf[i] = startVal + i;
	}
	stop_timer();
	uint32_t count = getCycles();
	debug_printf("Fill %d bytes, %d cycles\r\n", length * 4, count);

	reset_timer();
	start_timer();
	HAL_DMA_Start_IT(&hdma1, (uint32_t) &src_buf[0], (uint32_t )&dst_buf[0], length * 4);
	osSemaphoreWait(dma1SemHandle, portMAX_DELAY);
	stop_timer();
	count = getCycles();

	debug_printf("DMA %d bytes, %d cycles : ", length * 4, count);

	if(length > BUF_MAX_SIZE) {
		length = 16;
	}

	int result = 0;
	for(i = 0; i < length; i++) {
		if(src_buf[i] != dst_buf[i]) {
			result++;
		}
	}

	if(result) {
		debug_print("FAIL\r\n");
	}
	else {
		debug_print("PASS\r\n");
	}

	startVal += 1;
	length += 16;
}


volatile int ADC2_Chan15_EOS = 0;
uint16_t val1, val2;
static uint16_t adc_val[2];
void ADC2_Test()
{
#if ADC2_REGULAR_TEST

#if ADC2_INTR_MODE
	debug_print("Testing ADC (DMA)...");
	ADC2_Chan15_EOS = 0;
	reset_timer();
	start_timer();
	HAL_ADC_Start_DMA(&hadc2, (uint32_t*) &adc_val[0], 2);
	while(ADC2_Chan15_EOS == 0) ;
	stop_timer();
	uint32_t cnt = getCycles();

	val1 = adc_val[0];
	val2 = adc_val[1];
	debug_printf("done\r\nVal1: %d, Val2: %d, (%d cycles)\r\n", val1, val2, cnt);

	myLCD_Clear();
	myLCD_GotoXY(1, 1);
	myLCD_PrintStr("ADC reg done");
#endif

#if ADC2_POLL_MODE
	debug_print("Testing ADC (poll)...");
	reset_timer();
	start_timer();
	HAL_ADC_Start(&hadc2);
	stop_timer();
	uint32_t cnt = getCycles();

	if(HAL_ADC_PollForConversion(&hadc2, 1000) == HAL_OK) {
		uint16_t val = HAL_ADC_GetValue(&hadc2);
		debug_printf(" %d (%d cycles)\r\n", val, cnt);

		myLCD_Clear();
		myLCD_GotoXY(1, 1);
		myLCD_PrintStr("ADC reg done");
	}
	else {
		debug_print("Timeout\r\n");
		myLCD_Clear();
		myLCD_GotoXY(1, 1);
		myLCD_PrintStr("ADC timeout");
	}
#endif
#endif

#if ADC2_INJ_TEST
#if ADC2_POLL_MODE
	debug_print("Testing ADC injected (poll)...");
	HAL_ADCEx_InjectedStart(&hadc2);

	if(HAL_ADCEx_InjectedPollForConversion(&hadc2, 1000) == HAL_OK) {
		debug_print("done\r\n");
		val1 = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_1);
		val2 = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_2);

		debug_printf("Val1: %d, Val2: %d", val1, val2);
		myLCD_Clear();
		myLCD_GotoXY(1, 1);
		myLCD_PrintStr("ADC inj done");
	}
	else {
		debug_print("Timeout\r\n");
		myLCD_Clear();
		myLCD_GotoXY(1, 1);
		myLCD_PrintStr("ADC timeout");
	}
#endif

#if ADC2_INTR_MODE
	ADC2_Chan15_EOS = 0;
	debug_print("Testing ADC injected (intr)...");
	HAL_ADCEx_InjectedStart_IT(&hadc2);
	while(ADC2_Chan15_EOS == 0);
	debug_print("done\r\n");
	val1 = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_1);
	val2 = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_2);

	debug_printf("Val1: %d, Val2: %d\r\n", val1, val2);
	myLCD_Clear();
	myLCD_GotoXY(1, 1);
	myLCD_PrintStr("ADC inj done");
#endif

#endif
}

