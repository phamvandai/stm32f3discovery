# Example of working with STM32F3Discovery peripherals.

(c) 2018 - phamvandaibk@gmail.com

Setup environment:

-HW:

	+ STM32F3Discovery board
	+ LCD 16x2 connect with PORTB (See mylcd.h for configuration)
	+ ADC2 with channel 1 (PA4) & channel 6 (PC0)
	+ USART1 (PC4 & PC5) 
	
-SW:

	+ STM32CubeMX with STM32F3CubeMX firmware package
	+ Atollic TrueSTUDIO for STM32
	+ FreeRTOS 
	
Release note:

	May 20th, 2018:
	+ Test DMA, ADC1 with some option (Regular or injected, polling or DMA or interrupt)
	+ from terminal send 'a' for ADC test, 'd' for DMA test
	