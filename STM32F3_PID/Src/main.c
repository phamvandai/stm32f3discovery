/**
  STM32F3 Discovery board + FreeRTOS + PID Motor control
	pham van dai
 */
#include "main.h"

/*----------------- Init structures--------------------*/
TIM_HandleTypeDef TIM3_Handle;
TIM_HandleTypeDef TIM2_Handle;
TIM_OC_InitTypeDef TIM2_PWM_InitStruct;
TIM_HandleTypeDef TIM4_Handle;
UART_HandleTypeDef UART1_Handle;
/*--------------------- Task handles--------------------*/
xTaskHandle LCD_DisplayTask_Handle;
xTaskHandle StartUpTask_Handle;
xTaskHandle UART_SendTask_Handle;
xTaskHandle UART_ReceiveTask_Handle;
xTaskHandle ButtonTask_Handle;
xTaskHandle CommandProcessingTask_Handle;
xTaskHandle PID_Task_Handle;
/*---------------------Semaphore handles----------------*/
xSemaphoreHandle tim3_semph;
xSemaphoreHandle btn_semph;
xSemaphoreHandle uart_semph;
xSemaphoreHandle pid_semph;
/*----------------------Variables-----------------------*/
__IO uint16_t PWM_Pulse=0;
__IO uint16_t count,count_old;								
__IO float speed,speed_raw;
__IO int32_t delta_count,delta_count_old;
__IO float KP,TI,TD,SP;
__IO uint8_t UART_SEND_EN=0, DIRECTION=1;
PID_DB pid_param;
/* -----------------Main function------------------------*/
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

  /* Configure the system clock */
	SystemClock_Config();

  /* Initialize all configured peripherals */
	GPIO_Init();
	BSP_LED_Init(LED_GREEN);
	BSP_LED_Init(LED_RED);
	BSP_LED_Init(LED_BLUE);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED7);
	BSP_LED_Init(LED9);
	BSP_LED_Init(LED10);
	BSP_LED_Init(LED8);
	
	/* Initialize TIMER 3 count up in interrupt mode*/
	TIM3_BaseInit();
	/*Initialize TIMER 2 in PWM Mode*/
	TIM2_PWMInit();
	/*Initialize TIMER 4 in Encoder mode*/
	TIM4_EncInit();
	HAL_TIM_Encoder_Start(&TIM4_Handle,TIM_CHANNEL_ALL);
	/*Initialize UART1*/
	UART1_Init();
	
	/*##################NOTE######################################
	Every Hardware initialization must not call FreeRTOS API before kernel started
	*/
	pid_param.CYCLE=DELTA_T;
	pid_param.KP=20;
	pid_param.TI=2;
	pid_param.TD=0;
	pid_param.Tf=0.2;
	pid_param.DEADZONE=0.05;
	pid_param.I_EN=1;
	pid_param.D_EN=1;
	
	/*Create semaphore and mutex*/
	vSemaphoreCreateBinary(tim3_semph);
	vSemaphoreCreateBinary(btn_semph);
	vSemaphoreCreateBinary(uart_semph);
	vSemaphoreCreateBinary(pid_semph);
  
	/* Create the thread(s)
	definition and creation of LCD Display task */
	osThreadDef(LCD_Display_Task, LCD_Display, osPriorityNormal, 0, 128);
	LCD_DisplayTask_Handle = osThreadCreate(osThread(LCD_Display_Task), NULL);
	
	/*StartUp task has higher priority and will runs first, but it runs only once
	because LCD drivers can only use osDelay API after starting kernel*/
	osThreadDef(StartUpTask,StartUp,osPriorityHigh,0,128);
	StartUpTask_Handle=osThreadCreate(osThread(StartUpTask),NULL);
	
	/*UART Sending Task*/
	osThreadDef(UART_SendTask,UART_Send,osPriorityNormal,0,128);
	UART_SendTask_Handle=osThreadCreate(osThread(UART_SendTask),NULL);
	
	/*UART Receive Task*/
	osThreadDef(UART_ReceiveTask,UART_Receive,osPriorityNormal,0,128);
	UART_ReceiveTask_Handle=osThreadCreate(osThread(UART_ReceiveTask),NULL);
	
	/*UART Command processing task*/
	osThreadDef(CommandProccessingTask,CommandProcessing,osPriorityNormal,0,128);
	CommandProcessingTask_Handle=osThreadCreate(osThread(CommandProccessingTask),NULL);
	
	/*PID Task process has highest(realtime) priority*/
	osThreadDef(PID_Task,PID_Task_Function,osPriorityRealtime,0,128);
	PID_Task_Handle=osThreadCreate(osThread(PID_Task),NULL);
	
	/* Start scheduler */
	osKernelStart(NULL,NULL);
	while (1)
	{
	}
}

/*---------------------------------Interrupt handlers------------------------*/
/*
	Timer 2 period update processing
	PID function will be placed here
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{	
	count=TIM4->CNT;
	if(DIRECTION) 
		delta_count=count-count_old;
	else 
		delta_count=count_old-count;
	
	if((delta_count)<0) 
		delta_count=delta_count_old; // TIM4 CNT overflowed
	delta_count_old=delta_count;
	count_old=count;

	//Release semaphore from ISR
	portBASE_TYPE Woken=pdFALSE; 
	BSP_LED_Toggle(LED_RED);
	xSemaphoreGiveFromISR(tim3_semph,&Woken); 
	portEND_SWITCHING_ISR(Woken);
}
/*EXTI0 IRQ Handle*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_0){
		portBASE_TYPE Woken=pdFALSE; 
		xSemaphoreGiveFromISR(btn_semph,&Woken);
		portEND_SWITCHING_ISR(Woken);
	}
}
/*UART1 Receiver IRQ Handler*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	BSP_LED_Toggle(LED8);
	portBASE_TYPE Woken=pdFALSE; 
	xSemaphoreGiveFromISR(uart_semph,&Woken);
	portEND_SWITCHING_ISR(Woken);
}
/*UART1 Transmit Complete IRQ Handler*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	BSP_LED_Toggle(LED6);
}
/*-----------------------measurement and control functions--------------------*/
/*Convert integer number with format abc to string*/
uint8_t* UI16ToStr(uint16_t num)
{
	static uint8_t temp[4];
	uint8_t a,b,c,ch_a,ch_b,ch_c;
	
	a=num/100;
	b=(num-100*a)/10;
	c=(num-100*a-10*b);
	ch_a=a+48; ch_b=b+48; ch_c=c+48;
	temp[0]=ch_a; 
	temp[1]=ch_b;
	temp[2]=ch_c;
	temp[3]='\0';
	return (uint8_t *) temp;
}
/*Convert floating point number with format ab.de to string*/
uint8_t * FloatToStr(float num)
{
	static uint8_t temp[6];
	uint8_t a,b,d,e,a_ch,b_ch,d_ch,e_ch,de;
	
	if(num<100) {
		a=(uint8_t)(num/10);
		b=(uint8_t)((num-10*a));
		de=(uint8_t) ((num-10*a-b)*100);
		d=de/10;
		e=(de-10*d);
		a_ch=a+48;
		b_ch=b+48;
		d_ch=d+48;
		e_ch=e+48;
		temp[0]=a_ch;
		temp[1]=b_ch;
		temp[2]='.';
		temp[3]=d_ch;
		temp[4]=e_ch;
		temp[5]='\0';
		return temp;
	}
	else {
		temp[0]='1';
		temp[1]='0';
		temp[2]='0';
		temp[3]='.';
		temp[4]='0';
		temp[5]='\0';
		return temp;
	}
}
/*Convert floating point number with format ab.de to string with \n at the end*/
uint8_t * FloatToStrNL(float num)
{
	static uint8_t temp[7];
	uint8_t a,b,d,e,a_ch,b_ch,d_ch,e_ch,de;

    a=(uint8_t)(num/10);
    b=(uint8_t)((num-10*a));
    de=(uint8_t) ((num-10*a-b)*100);
    d=de/10;
    e=(de-10*d);
    a_ch=a+48;
    b_ch=b+48;
    d_ch=d+48;
    e_ch=e+48;
    temp[0]=a_ch;
    temp[1]=b_ch;
    temp[2]='.';
    temp[3]=d_ch;
    temp[4]=e_ch;
	temp[5]='\n';
	temp[6]='\0';
	return temp;
}
/*Filter- should be placed in TIM3 Handler*/
float filter(float in,float T, float DT)
{
	static float out, out_old;
	out=DT*in/T+out_old*(1-DT/T);
	out_old=out;
	return out;
}
/*PID algorithm
	output: PWM duty percentage
	Y(s)/X(s)=KP(1+1/(TI*s)+TD*s/(Tf*s+1));
*/
void pid(PID_DB *param)
{
	static float err,err_old, P_part, I_part,I_part_old, D_part,D_part_old;
	
	if(!param->MANUAL) {
		if(param->SP==0) 
			param->OUT=0;
		else {
			err=param->SP-param->PV;	
			if(abs(err)<=param->DEADZONE) 
				err=0;
			P_part=param->KP*err;
			//If enable Integral
			if(param->I_EN) {
				if (param->TI <param->CYCLE) param->TI =param->CYCLE;
				I_part=0.5f*(param->KP/param->TI)*(err+err_old)*param->CYCLE+I_part_old;
				if(I_part>=100) I_part=100; //Anti windump
				I_part_old=I_part;
			}
			else 
				I_part=0;
			//If enable derivate
			if(param->D_EN){
				D_part=(param->TD*(err-err_old)+param->Tf*D_part_old)/(param->CYCLE+param->Tf);
				D_part_old=D_part;
			}
			else 
				D_part=0;
			param->OUT=P_part+I_part+D_part;
			err_old=err;
		}
	}
	else 
		param->OUT=param->MANUAL_OUT;
	if (param->OUT>=100) 
		param->OUT=100; 
	else 
	if(param->OUT<=0)
		param->OUT=0;

}

/*Pseudo correction device with early phase
static float PKU_FO(float IN, float T, float T0, float CYCLE)
{
		static float out_fil, out_fil_old, in_old, out_old, out;
		uint8_t out_sign;
		out_fil=(T0*out_fil_old+IN*(T+CYCLE)-in_old*T)/(T0+CYCLE);
		out_fil_old=out_fil;
		in_old=IN;
		out_sign=sign(out_fil);
		out=IN*out_sign;
		return out;
}*/

float abs(float in)
{
	if (in>=0) return in;
	return (-1*in);
}
int8_t sign(float in)
{
	if(in>=0) return 1;
	return -1;
}
/*Convert value from command to float; abcd-> ab,cd*/
float command_convert(uint8_t * str)
{
	float temp,a,b,c,d;
	
	a= (float) (str[0]-48);
	b= (float) (str[1]-48);
	c= (float) (str[2]-48);
	d= (float) (str[3]-48);
	temp= 10*a +b+0.1f*c+0.01f*d;
	return temp;
}
/*Convert string abcd to PWM duty abc.d*/
float command_to_duty(uint8_t *str)
{
	float temp,a,b,c,d;
	a= (float) (str[0]-48);
	b= (float) (str[1]-48);
	c= (float) (str[2]-48);
	d= (float) (str[3]-48);
	temp=a*100+b*10+c+0.1f*d;
	return temp;
}
#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif


