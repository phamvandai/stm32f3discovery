/**
 LCD 16x2 driver for STM32F30x using CubeMX firmwares
 To use this driver with FreeRTOS, place Init function in a task after starting 
 kernel, otherwise system will hang.
 @modified by joedalton
 @ver 0.02
 **/


#include "stm32f3xx_hal.h"
#include "cmsis_os.h"
/*----------USER CONFIGS HERE----------------*/


/** connect RW leg to GND, and power supply rails to +5V and gnd
 Define symbolic LCD - MCU pin mappings
 We've set DATA PIN TO 4,5,6,7 for easy translation **/
#define 		LCD_GPIO_PORT		  		GPIOB				//Define GPIO PORT for LCD
#define 		LCD_CLK_ENABLE()	  	__GPIOB_CLK_ENABLE()
#define     LCD_OUT               LCD_GPIO_PORT->ODR
#define 		LCD_PIN_RW			  		GPIO_PIN_2				//To read BF flag
#define     LCD_PIN_RS            GPIO_PIN_0         // RS=1 write DATA,   RS=0 write Instruction
#define     LCD_PIN_EN            GPIO_PIN_1         // EN=0 - Disable LCD, EN=1 - ENABLE, EN:1->0: Write to LCD 
#define     LCD_PIN_D7            GPIO_PIN_7          
#define     LCD_PIN_D6            GPIO_PIN_6          
#define     LCD_PIN_D5            GPIO_PIN_5          
#define     LCD_PIN_D4            GPIO_PIN_4          

#define     LCD_PIN_MASK  ((LCD_PIN_RS | LCD_PIN_EN | LCD_PIN_D7 | LCD_PIN_D6 | LCD_PIN_D5 | LCD_PIN_D4))
/*----------END USER CONFIGS----------------*/


#ifndef FALSE  
#define     FALSE               0 
#endif
#ifndef TRUE   
#define     TRUE                1 
#endif

/*---------Private Function Prototypes--------------*/
void __LCD_wait(void);
void __LCD_Pulse(void);
void __LCD_SendByte(uint8_t ByteToSend, int IsData);
/*---------LCD user 's function---------------------*/
void my_LCD_gotoXY(uint8_t Row, uint8_t Col);
void my_LCD_Clear(void);
void my_LCD_Init(void);
void my_LCD_PrintStr(uint8_t *Text);
void my_LCD_PutChar (uint8_t c);

