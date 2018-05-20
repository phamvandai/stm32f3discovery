/*
 * tests.h
 *
 *  Created on: May 15, 2018
 *      Author: phamv_000
 */

#ifndef TESTS_H
#define TESTS_H

#include "tasks.h"

#define DMA1_TEST_EN			1

#define ADC2_REGULAR_TEST		1
#define ADC2_INJ_TEST			0

#define	ADC2_POLL_MODE			0
#define ADC2_INTR_MODE			1

void DMA1_Test(void);
void ADC2_Test();

#endif
