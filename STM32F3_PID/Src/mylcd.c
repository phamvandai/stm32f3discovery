/**
 LCD 16x2 driver for STM32F30x using CubeMX firmwares
 To use this driver with FreeRTOS, place Init function in a task after starting 
 kernel, otherwise system will hang.
 @modified by joedalton
 @ver 0.02
 **/
 /*----------INCLUDED FILES-------------*/
#include "mylcd.h"

/*Private variables*/
GPIO_InitTypeDef  GPIO_InitStructure;
uint8_t initialized=FALSE;
/*----------Low level functions------------*/

/**
*	Wait LCD; place before sending instruction to LCD
*/
void __LCD_wait(void)
{	
	if(initialized){
		/*Configure D4-D7 as input to read DF flag*/
		GPIO_InitStructure.Pin=LCD_PIN_D7;
		GPIO_InitStructure.Mode=GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(LCD_GPIO_PORT, &GPIO_InitStructure);
	
		LCD_OUT &= LCD_PIN_RW; // Set RW to Read from LCD
		while(1){
			//LCD_OUT &= LCD_PIN_EN;
			if(HAL_GPIO_ReadPin(LCD_GPIO_PORT,LCD_PIN_D7)==GPIO_PIN_RESET) break;
			//LCD_OUT &= ~LCD_PIN_EN;
		}
		LCD_OUT &= ~LCD_PIN_RW; // Unset to allow writing to LCD
		GPIO_InitStructure.Pin = LCD_PIN_D7;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(LCD_GPIO_PORT, &GPIO_InitStructure);
	}
}
/**
* Low level function
* To write data or instruction to LCD; Reset EN, then Set EN, and lastly Reset EN to make negative edge
*/
void __LCD_Pulse(void)
{		
    //LCD_OUT &= ~LCD_PIN_EN; //EN=0; Disable
    LCD_OUT |= LCD_PIN_EN; //EN=1; Enable
	osDelay(2);
    LCD_OUT &= (~LCD_PIN_EN); //EN: 1->0: write
}

/**
	Low level function
	@brief Send byte to LCD, this may be command or data
	@param ByteToSend as a command may be one of these
	0x80 		// Address Display Data RAM pointer
	0x40 		// Address Character Generator RAM pointer
	0x01 		// Clear entire display and set Display Data Address to 0
	0x02 		// sets DDRAM address 0 and returns display from being shifted to original position.
	0x28 		// function set is 4 bit data length and 2 lines
	0x06 		// Entry mode is display Data RAM pointer incremented after write
	0x0C		// Sets entire display on, cursor on and blinking of cursor position character
	0x08   		// Sets entire display off, cursor off
	0x04		// turn on cursor
	0x00    	// turn off cursor
	0x10		// Cursor move and shift to left
	0x14		// Cursor move and shift to right
	@param isData: TRUE or FALSE; if isData TRUE RS=0 else RS=1
**/
void __LCD_SendByte(uint8_t ByteToSend, int IsData)
{		
	//Write first 4 major bits
	//__LCD_wait();
	osDelay(2);
    LCD_OUT &= (~LCD_PIN_MASK); // Reset LCD OUT 
    LCD_OUT |= (ByteToSend & 0xF0); 
		
    if (IsData == TRUE) {
        LCD_OUT |= LCD_PIN_RS;  // Set RS=1 to write data
    }
    else {
        LCD_OUT &= ~LCD_PIN_RS; //Inverse RS from 1 to 0 to write instruction
    }
		
    __LCD_Pulse();
	//write other 4 bits
	//__LCD_wait();
	osDelay(2);
	LCD_OUT &= (~LCD_PIN_MASK);
    LCD_OUT |= ((ByteToSend & 0x0F) << 4);

    if (IsData == TRUE){
        LCD_OUT |= LCD_PIN_RS;
    }
    else {
        LCD_OUT &= ~LCD_PIN_RS;
    }
	__LCD_Pulse();
}

/************************** User 's functions *********************/
/**
	@brief LCD initialization
	@param none
	@retVal none
**/
void my_LCD_Init(void)
{			
	LCD_CLK_ENABLE();
    GPIO_InitStructure.Pin =LCD_PIN_RS | LCD_PIN_EN| LCD_PIN_D4 | LCD_PIN_D5| LCD_PIN_D6| LCD_PIN_D7 | LCD_PIN_RW;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LCD_GPIO_PORT, &GPIO_InitStructure);

	//__LCD_wait();
		
    LCD_OUT &= ~(LCD_PIN_MASK);
    LCD_OUT &= ~LCD_PIN_RW; //Reset RW to write
    LCD_OUT = 0x20;
	osDelay(2);
    __LCD_Pulse();
	__LCD_SendByte(0x28, FALSE);	//Function set: Set 4bit mode 5x10dot
    __LCD_SendByte(0x0E, FALSE);  // Display ON/OFF:Display On, cursor is displayed and does not blinks
    __LCD_SendByte(0x06, FALSE);  //Entry mode set: increment address counter; display will be shifted to the right
	initialized=TRUE;
	my_LCD_Clear();
}


/**
	@brief move the cussor to the required position
	@param Row [1-2]
	@param Col [1-16]
	@retVal None
**/
void my_LCD_gotoXY(uint8_t Row, uint8_t Col)
{	
	uint8_t _row,_col;
    uint8_t address;
	
	_row=Row-1;_col=Col-1;
    if (_row == 0)
        address = 0;
    else
        address = 0x40;
	
    address |= _col;
    __LCD_SendByte(0x80 | address, FALSE);
}

/**
	@brief Clear entire display
	@param none
	@retVal none
**/
void my_LCD_Clear(void)
{
	__LCD_SendByte(0x01,FALSE);
}


/**
* Print String 
*
*/
void my_LCD_PrintStr(uint8_t *Text)
{
    uint8_t *c;

    c = Text;
    while ((c != 0) && (*c != 0)){
        __LCD_SendByte(*c, TRUE);
        c++;
    }
}

/**
* Put a char
*/
void my_LCD_PutChar (uint8_t c)
{
	__LCD_SendByte(c,TRUE);
}
