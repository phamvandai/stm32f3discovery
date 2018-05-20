/**
 LCD 16x2 driver for STM32F30x using CubeMX firmware;
 not comply with FreeRTOS
 @modified by joedalton
 @ver 0.01
 @ver 0.1	13th May, 2018 - Fix errors & speed optimization
 **/

#include "mylcd.h"

/*---------Private Function Prototypes--------------*/
static void __LCD_Wait(void);
static void __LCD_Pulse();
static void __LCD_SendByte(uint8_t ByteToSend, int IsData);

/**
*	Wait LCD; place before sending instruction to LCD
*/
static void __LCD_Wait(void)
{	
	volatile int i = 0;
	for(i = 0; i < 200; i++) {};
}

/**
* Low level function
* To write data or instruction to LCD; Set EN, delay sometime and Reset EN to make negative edge
*/
static void __LCD_Pulse(void)
{		
    LCD_OUT |= LCD_PIN_EN; //EN=1; Enable
    int i = 0;
    for(i = 0; i < 10; i++) {};
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
static void __LCD_SendByte(uint8_t ByteToSend, int IsData)
{		
	//Write first nibble
	__LCD_Wait();
	uint32_t currState = LCD_OUT;

	currState &= (~LCD_PIN_MASK); // Reset LCD OUT
	currState |= (ByteToSend & 0xF0);
		
    if (IsData == TRUE) {
    	currState |= LCD_PIN_RS;  // Set RS=1 to write data
    }
    else {
    	currState &= ~LCD_PIN_RS; //Inverse RS from 1 to 0 to write instruction
    }

    LCD_OUT = currState;
    __LCD_Pulse();
	
	//write second nibble
	__LCD_Wait();
	currState = LCD_OUT;
	currState &= (~LCD_PIN_MASK);
	currState |= ((ByteToSend & 0x0F) << 4);

    if (IsData == TRUE) {
    	currState |= LCD_PIN_RS;
    }
    else {
    	currState &= ~LCD_PIN_RS;
    }

    LCD_OUT = currState;

	__LCD_Pulse();
}

/************************** User 's functions *********************/
/**
	@brief LCD initialization
	@param none
	@retVal none
**/
void myLCD_Init(void)
{			
	GPIO_InitTypeDef  	LCD_IO_InitStruct;

	LCD_CLK_ENABLE();
    LCD_IO_InitStruct.Pin 	= LCD_PIN_RS | LCD_PIN_EN| LCD_PIN_RW |
    						  LCD_PIN_D4 | LCD_PIN_D5| LCD_PIN_D6| LCD_PIN_D7;
    LCD_IO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
    LCD_IO_InitStruct.Pull 	= GPIO_PULLUP;
    LCD_IO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_GPIO_PORT, &LCD_IO_InitStruct);

    LCD_Delay(20);					// Wait for power on stabilization

    LCD_OUT &= ~(LCD_PIN_RS);		// Will send instruction
    LCD_OUT &= ~(LCD_PIN_RW);		// Data direction MCU -> LCD
    LCD_OUT &= ~(LCD_PIN_EN);		// Reset EN

    LCD_OUT |= (LCD_PIN_EN);		// Enable EN
    LCD_OUT |= (LCD_PIN_D5);		// Set pin D5 before enter to 4bit mode (avoid of error on reset)
    LCD_OUT &= ~(LCD_PIN_EN);		// Write to LCD
	
    __LCD_SendByte(0x28, FALSE);  	// Function set: Set 4bit mode 5x10 dot
    __LCD_SendByte(0x0E, FALSE);  	// Display ON/OFF:Display On, cursor is displayed and does not blinks
    __LCD_SendByte(0x06, FALSE);  	// Entry mode set: increment address counter; display will be shifted to the right

	myLCD_Clear();
}


/**
	@brief move the cussor to the required position
	@param Row [1-2]
	@param Col [1-16]
	@retVal None
**/
void myLCD_GotoXY(uint8_t Row, uint8_t Col)
{	
	if(Row < 1 || Row > 2) {
		return;
	}

	if(Col < 1 ||Col > 16) {
		return;
	}
	
    uint8_t address;
	
	if (Row == 1) {
        address = 0;
    }
    else {
        address = 0x40;
    }
	
    address |= Col - 1;
    __LCD_SendByte(0x80 | address, FALSE);
}

/**
	@brief Clear entire display
	@param none
	@retVal none
**/
void myLCD_Clear(void)
{		
	__LCD_SendByte(0x01, FALSE);
	LCD_Delay(3);
}


/**
* Print String 
*
*/
void myLCD_PrintStr(char *Text)
{
    char *c = Text;

    while ((c != 0) && (*c != 0)) {
        __LCD_SendByte(*c, TRUE);
        c++;
    }
}

/**
* Put a char
*/
void myLCD_PutChar (char c)
{
	__LCD_SendByte(c,TRUE);
}
