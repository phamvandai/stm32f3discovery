/**
 LCD 16x2 driver for STM32F30x using CubeMX firmwares -CMSIS-OS Comply
 @modified by joedalton
 @ver 0.01
 @ver 0.1	13th May, 2018
 **/

/* Include header file, modify it as MCU family (F3, F4...)*/
#include "stm32f3xx_hal.h"
#include "cmsis_os.h"

/**** Configure LCD 's Pins ********/
/**
 * Connect RW leg to GND, and power supply rails to +5V and gnd
 * Define symbolic LCD - MCU pin mappings
 * We've set DATA PIN TO 4,5,6,7 for easy translation
 * NOTE: All pins should be in one GPIO PORT
 */

#define 	LCD_GPIO_PORT		GPIOB				// Define GPIO PORT for LCD
#define 	LCD_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define     LCD_OUT             LCD_GPIO_PORT->ODR
#define 	LCD_PIN_RW			GPIO_PIN_2		   	// To read BF flag
#define     LCD_PIN_RS          GPIO_PIN_0         	// RS=1 write DATA,   RS=0 write Instruction
#define     LCD_PIN_EN          GPIO_PIN_1         	// EN=0 - Disable LCD, EN=1 - ENABLE, EN:1->0: Write to LCD
#define     LCD_PIN_D7          GPIO_PIN_7          
#define     LCD_PIN_D6          GPIO_PIN_6          
#define     LCD_PIN_D5          GPIO_PIN_5          
#define     LCD_PIN_D4          GPIO_PIN_4          

#define     LCD_PIN_MASK  ((LCD_PIN_RS | LCD_PIN_EN | LCD_PIN_D7 | LCD_PIN_D6 | LCD_PIN_D5 | LCD_PIN_D4))


#ifndef FALSE  
#define     FALSE               0 
#endif
#ifndef TRUE   
#define     TRUE                1 
#endif

/* Delay function definition */
#define 	LCD_Delay(x)		osDelay(x)

/*---------LCD user 's function---------------------*/
void myLCD_GotoXY(uint8_t Row, uint8_t Col);
void myLCD_Clear(void);
void myLCD_Init(void);
void myLCD_PrintStr(char *Text);
void myLCD_PutChar (char c);

