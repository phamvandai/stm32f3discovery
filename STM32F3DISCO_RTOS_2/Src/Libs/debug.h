/*
 * debug.h
 *
 *  Created on: Apr 23, 2018
 *      Author: phamv_000
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/* Debug configuration */
#define 	DEBUG		1

#if DEBUG
#include "stm32f3xx_hal.h"
extern 			UART_HandleTypeDef huart1;
#define 		DEBUG_UART_HANDLE	huart1

		void 		debug_outbyte(char c);
		void 		debug_print(const char *str);
		int 		debug_printf(const char *format, ...);
		int 		debug_sprintf(char *out, const char *format, ...);

#else
		#define debug_outbyte(...)
		#define debug_print(...)
		#define debug_printf(...)
		#define debug_sprintf(...)
#endif /* DEBUG */

#endif /* DEBUG_H_ */
