/*
 * ds1307.h
 *
 *  Created on: May 27, 2018
 *      Author: phamv_000
 */

#ifndef LIBS_DS1307_H_
#define LIBS_DS1307_H_

typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t date;
	uint8_t month;
	uint16_t year;
} DateTime_t;

void DS1307_GetDateTime(const uint8_t *pRegVals, DateTime_t *pDateTime);

#endif /* LIBS_DS1307_H_ */
