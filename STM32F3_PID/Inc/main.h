/*Main include file*/

#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "mylcd.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
/*------------------PIN OUT----------------------------
* PA0: User button in EXTIO mode
* PA2: TIM2 PWM Chanel 3 - Motor ENABLE
* PA3: GPIO-OUT for Motor IN1
* PA4: GPIO-OUT for Motor IN2
* GPIOB: see mylcd.h
* TIMER3: TimeBase
* TIMER4: Encoder mode PD12(A) - PD13(B)
* UART1: PC4<->Tx, PC5<->Rx
*/

#define Motor_Direction_Neg() {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET); \
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);}
#define Motor_Direction_Pos() {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET); \
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);}
#define Motor_Stop() {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET); \
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);}
#define ENC_RES 888 //Encoder 's resolution
#define DELTA_T 0.025 //Time interval of TIM3
#define ABS(_A_) (_A_>0 ? _A_: -_A_)
#define Tfilter 0.2

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
typedef struct {
	float SP;		// Setpoint
	float PV;		// Feedback signal
	uint8_t I_EN; //Enable integral
	uint8_t D_EN; //Enable derivate
	float KP;		// Proportional coefficient
	float TI;		// Integral constant
	float TD;		// Derivate constant
	float Tf; 	// Filter coefficient for derivate 
	float DEADZONE; //error dead zone
	float CYCLE; 	// PID Discrete interval 
  uint8_t MANUAL;	// Manual mode
	float MANUAL_OUT; // Manual output
	float OUT; 	//Output value
	uint8_t OVFL; //Output overflow flag
} PID_DB;
extern PID_DB pid_param;

/*----------------- Init structures--------------------*/
extern TIM_HandleTypeDef TIM3_Handle;
extern TIM_HandleTypeDef TIM2_Handle;
extern TIM_OC_InitTypeDef TIM2_PWM_InitStruct;
extern TIM_HandleTypeDef TIM4_Handle;
extern UART_HandleTypeDef UART1_Handle;
/*--------------------- Task handles--------------------*/
extern xTaskHandle LCD_DisplayTask_Handle;
extern xTaskHandle StartUpTask_Handle;
extern xTaskHandle UART_SendTask_Handle;
extern xTaskHandle ButtonTask_Handle;
extern xTaskHandle UART_ReceiveTask_Handle;
extern xTaskHandle CommandProcessingTask_Handle;
extern xTaskHandle PID_Task_Handle;
/*---------------------Semaphore handles----------------*/
extern xSemaphoreHandle tim3_semph;
extern xSemaphoreHandle btn_semph;
extern xSemaphoreHandle uart_semph;
extern xSemaphoreHandle pid_semph;
/*----------------------Variables-----------------------*/
extern __IO uint16_t PWM_Pulse;
extern __IO uint16_t count;	
extern __IO float speed,speed_raw;
extern __IO int32_t delta_count,delta_count_old;
extern __IO uint16_t count_old;
extern __IO float KP,TI,TD,SP;
extern  uint8_t command[5];
extern __IO uint8_t UART_SEND_EN,DIRECTION; //Enable UART Sending
/*------------------Init functions------------------*/
void SystemClock_Config(void);
void GPIO_Init(void);
void TIM3_BaseInit(void);
void TIM2_PWMInit(void);
void TIM4_EncInit(void);
void UART1_Init(void);
/*-------------FreeRTOS tasks functions--------------*/
void LCD_Display(void const * argument);
void StartUp(void const * argument);
void UART_Send(void const *argument);
void UART_Receive(void const *argument);
void CommandProcessing (void const *argument);
void PID_Task_Function(const void *argument);
/*--------------User functions-----------------------*/
uint8_t* UI16ToStr(uint16_t num);
uint8_t* FloatToStr(float num);
uint8_t * FloatToStrNL(float num);
uint8_t* StrWithNewLine(uint8_t * first_str);
float filter(float in,float T, float DT);
void pid(PID_DB *param);
float abs(float in);
int8_t sign(float in);
//static float PKU_FO(float IN, float T, float T0, float CYCLE);
float command_convert(uint8_t * str);
float command_to_duty(uint8_t *str);
#endif
