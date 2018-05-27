#include "main.h"

/*variables*/
uint8_t newline='\n';
uint8_t str_newline[]="\n";
uint8_t *speed_str;
uint8_t rps[]="r/s";
uint8_t command[5];

/*Startup task*/
void StartUp(void const * argument)
{	
	for(;;) {
		my_LCD_Init();
		my_LCD_Clear();
		HAL_TIM_Base_Start_IT(&TIM3_Handle);
		/*Set direction*/
		Motor_Direction_Pos();
		count_old=count;
		//HAL_TIM_Encoder_Start(&TIM4_InitStruct,TIM_CHANNEL_ALL);
		vTaskDelete(NULL);
	}
}

/* LCD Display function */
 void LCD_Display(void const * argument)
{ 
	uint8_t Duty_str[]="PWM Duty:",SP_str[]="Setpoint:",KP_str[]="KP:",TI_str[]="TI:",TD_str[]="TD:";
	uint8_t disp_mode=0;
	uint8_t m_mode[]="Manual mode   ", a_mode[]="Automatic mode";
	
	for(;;){
		if(xSemaphoreTake(btn_semph,50)==pdPASS){
			disp_mode++; my_LCD_Clear();
		if (disp_mode>2)
			disp_mode=0;
		}
		switch(disp_mode) {
		case 0: 
			my_LCD_gotoXY(1,1);
			my_LCD_PrintStr(KP_str);
			my_LCD_gotoXY(1,4);
			my_LCD_PrintStr(FloatToStr(pid_param.KP));
			my_LCD_gotoXY(2,1);
			my_LCD_PrintStr(TI_str);
			my_LCD_gotoXY(2,4);
			my_LCD_PrintStr(FloatToStr(pid_param.TI));
			my_LCD_gotoXY(2,9);
			my_LCD_PrintStr(TD_str);
			my_LCD_gotoXY(2,12);
			my_LCD_PrintStr(FloatToStr(pid_param.TD));
			break;
		case 1: 
			my_LCD_gotoXY(1,1);
			my_LCD_PrintStr(SP_str);
			my_LCD_gotoXY(1,10);
			my_LCD_PrintStr(FloatToStr(pid_param.SP));
			my_LCD_gotoXY(2,1);
			my_LCD_PrintStr(FloatToStr(speed));
			my_LCD_gotoXY(2,6);
			my_LCD_PrintStr(rps);	
			break;
		case 2: 	
			my_LCD_gotoXY(1,1);			
			if(pid_param.MANUAL)
				my_LCD_PrintStr(m_mode);
			else 
				my_LCD_PrintStr(a_mode);
			my_LCD_gotoXY(2,1);
			my_LCD_PrintStr(Duty_str);
			my_LCD_gotoXY(2,10);
			my_LCD_PrintStr(FloatToStr(pid_param.OUT));
			my_LCD_gotoXY(2,16);
			my_LCD_PutChar('%');	
			break;
		default: break;
	}
	osDelay(400);
  }
}


/*UART Sending task function*/
void UART_Send(void const *argument)
{ 
	for (;;){
		if(xSemaphoreTake(pid_semph,portMAX_DELAY)==pdPASS){
			if(UART_SEND_EN){
				speed_str=FloatToStrNL(speed);
				HAL_UART_Transmit_IT(&UART1_Handle,speed_str,(2+sizeof(speed_str)/sizeof(*speed_str)));
			}
		}
	}
}
/*UART Receiving task function*/
void UART_Receive (void const *argument)
{	
	for (;;){
		if(HAL_UART_Receive_IT(&UART1_Handle,command,5*sizeof(uint8_t))==HAL_OK) 
			BSP_LED_Toggle(LED9);
		osDelay(50);
	}
}

/*
*UART Command processing
*PC command format:Cxxxx
* C=U; enable/stop data sending from mcu to pc
* C=S; Setpoint =xx.xx
* C=P; Kp=xx.xx
* C=I; Ti=xx.xx
* C=D; Td=xx.xx
* C=M; Manual mode with PWM duty=xx.xx%
* C=A; Automatic mode
*/
void CommandProcessing (void const *argument)
{	
	uint8_t answer[18];
	uint8_t * param;
	uint8_t instruction;
	uint8_t data[4];
	
	for (;;) {	
		if(xSemaphoreTake(uart_semph,10)==pdPASS) {
			instruction=command[0];
			data[0]=command[1];
			data[1]=command[2];
			data[2]=command[3];
			data[3]=command[4];
			
			switch(instruction) {
			//Stop/enable data sending to PC
			case 'u': 
				if (command[1]=='0')
					UART_SEND_EN=0; 
				else
					UART_SEND_EN=1; 
				break; 
			//Setpoint
			case 's':
				pid_param.SP=command_convert(data);
				break;
			//kp
			case 'p':	
				pid_param.KP=command_convert(data);
				break;
			//ti
			case 'i': 
				pid_param.TI=command_convert(data); 
				break;
			//td
			case 'd': 
				pid_param.TD=command_convert(data); 
				break;
			//manual mode
			case 'm': 
				pid_param.MANUAL=1;
				pid_param.MANUAL_OUT=command_to_duty(data);
				break;
			//automatic mode
			case 'a': 
				pid_param.MANUAL=0; 
				break;
			//pc requests mcu to send pid params
			case 'r':
				answer[0]='p';
				param=FloatToStr(pid_param.SP);
				answer[1]=param[0];
				answer[2]=param[1];
				answer[3]=param[3];
				answer[4]=param[4];
				param=FloatToStr(pid_param.KP);
				answer[5]=param[0];
				answer[6]=param[1];
				answer[7]=param[3];
				answer[8]=param[4];	
				param=FloatToStr(pid_param.TI);
				answer[9]=param[0];
				answer[10]=param[1];
				answer[11]=param[3];
				answer[12]=param[4];		
				param=FloatToStrNL(pid_param.TD);
				answer[13]=param[0];
				answer[14]=param[1];
				answer[15]=param[3];
				answer[16]=param[4];		
				answer[17]=param[5];// \n
				HAL_UART_Transmit_IT(&UART1_Handle,answer,18*sizeof(uint8_t));
				break;
			//change direction
			case 'n':
				if (command[1]=='0'){ 
					DIRECTION=0;
					Motor_Stop();
					vTaskDelay(500);
					Motor_Direction_Neg();
				}
				else 
					if(command[1]=='1'){
						DIRECTION=1;
						Motor_Stop();
						vTaskDelay(500);
						Motor_Direction_Pos();
					}
				break;
			default: break;
			}
		}
	}
}

/*PID Task takes semaphore from TIM3 ISR */
void PID_Task_Function(const void *argument)
{
	for(;;){
		if(xSemaphoreTake(tim3_semph,portMAX_DELAY)==pdPASS) {
			speed_raw=((float)delta_count)/((float)(DELTA_T*ENC_RES*2)); //
			speed=filter(speed_raw,Tfilter,DELTA_T);
		
			pid_param.PV=speed;
			pid(&pid_param);
			PWM_Pulse=(uint16_t) (pid_param.OUT*50-1);
	
			//Set new PWM Pulse
			//TIM2_PWM_InitStruct.Pulse=PWM_Pulse;
			//HAL_TIM_PWM_ConfigChannel(&TIM2_Handle,&TIM2_PWM_InitStruct,TIM_CHANNEL_3);
			TIM2->CCR3=PWM_Pulse;
			HAL_TIM_PWM_Start(&TIM2_Handle,TIM_CHANNEL_3);
			xSemaphoreGive(pid_semph);
		}
	}
}
