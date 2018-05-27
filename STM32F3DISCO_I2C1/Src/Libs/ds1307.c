/*
 * ds1307.c
 *
 *  Created on: May 27, 2018
 *      Author: phamv_000
 */

#include <stdint.h>
#include "ds1307.h"


static uint8_t bcd2dec(uint8_t bcd)
{
	return (bcd & 0x0F) + (10 * ((bcd >> 4) & 0x0F));
}

/**
 * @brief	Get date time from array of DS1307 registers values from 0x00 to 0x06
 */
void DS1307_GetDateTime(const uint8_t *pRegVals, DateTime_t *pDateTime)
{
	if(pRegVals == 0 || pDateTime == 0) {
		return;
	}

	pDateTime->sec = bcd2dec(pRegVals[0]);
	pDateTime->min = bcd2dec(pRegVals[1]);
	pDateTime->hour= bcd2dec(pRegVals[2]);

	pDateTime->date= bcd2dec(pRegVals[4]);
	pDateTime->month=bcd2dec(pRegVals[5]);
	pDateTime->year= bcd2dec(pRegVals[6]);
}
