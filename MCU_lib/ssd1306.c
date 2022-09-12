/**
 * \file ssd1306.c
 * \author Tim Robbins - R&D Engineer, Atech Training
 * \brief Source file for ssd1306 oled functions
 * \version v2.0
 */ 
#include "ssd1306.h"

#if !defined(SSD1306_C) && defined(__INCLUDED_SSD1306__)
#define SSD1306_C


#include <string.h>

#if !defined(SSD1306_DRAW_IMMEDIATE) || SSD1306_DRAW_IMMEDIATE < 1
static uint8_t SSD1306DisplayBuffer[8][128]; //Current max possible size, maybe need to fix this?
#endif
static uint8_t cursorPosX;
static uint8_t cursorPosY;
static uint8_t ssd1306csPinPositions[] =
{
  SSD1306_CS_PIN_POSITIONS  
};

static uint8_t currentDisplay;

/**
 * Selects the currently active display from the display array
 * @param display The index of the display in the SSD1306_CS_PIN_POSITIONS macro
 */
void SSD1306SelectDisplay(uint8_t display)
{
    if(display < sizeof(ssd1306csPinPositions))
    {
        currentDisplay = display;
    }
    else
    {
        currentDisplay = 0;
    }
}



/**
 * Initializes the OLED display. Chip select must be set before running this
 * @param displayOn if the display should start as on
 * @param newScreenWidth The width of the screen
 * @param newScreenHeight The height of the screen
 */
void SSD1306Initialize(bool displayOn, uint8_t currentDisplaySelection)
{
    
    
    cursorPosX = 0;
    cursorPosY = 0;
    currentDisplay = currentDisplaySelection;
    
    //Initialize the displays init sequences
	uint8_t init_sequence [28] = {    // Initialization Sequence
		SSD1306_CMD_DISPLAY_OFF,    // Display OFF (sleep mode)
		
		0x20, 0b00,      // Set Memory Addressing Mode
						 // 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
						 // 10=Page Addressing Mode (RESET); 11=Invalid

		0xB0,            // Set Page Start Address for Page Addressing Mode, 0-7
		0xC8,            // Set COM Output Scan Direction
		0x00,            // --set low column address
		0x10,            // --set high column address
		0x40,            // --set start line address
		0x81, 0x3F,      // Set contrast control register
		0xA1,            // Set Segment Re-map. A0=address mapped; A1=address 127 mapped.
		0xA6,            // Set display mode. A6=Normal; A7=Inverse
		0xA8, SSD1306_HEIGHT-1, // Set multiplex ratio(1 to 64)
		0xA4,            // Output RAM to Display
		// 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
		0xD3, 0x00,      // Set display offset. 00 = no offset
		0xD5,            // --set display clock divide ratio/oscillator frequency
		0xF0,            // --set divide ratio
		0xD9, 0x22,      // Set pre-charge period
		0xDA, 
        #if SSD1306_HEIGHT == 32
            0x02
        #else
            0x12
        #endif
        ,// Set com pins hardware configuration
		0xDB,            // --set vcomh
		0x20,            // 0x20,0.77xVcc
		0x8D, 0x14,      // Set DC-DC enable
		(displayOn ? SSD1306_CMD_DISPLAY_ON : SSD1306_CMD_DISPLAY_OFF) //If the boolean passed is true, turn on, else turn off
		// dispAttr
		
	};
    
    //If the two wire register exists, only set up our pins if SPI mode selected
	//Else, just set up the SPI pins
	#if SSD1306_SPI == 1
	
	//On the control port, run the reset on the oled
    SSD1306_CS_PORT |= (1 << ssd1306csPinPositions[currentDisplay]);
    SSD1306_SET_DC();
    SSD1306_SET_RES();
	SSD1306_CLEAR_RES();
	delayForMilliseconds(10);
	SSD1306_SET_RES();
    
    
	#endif
	
	//Send our initialization sequence
	SSD1306SendCommandArray(init_sequence, sizeof(init_sequence));

	//Clear the OLED screen and deactivate any scrolling
	SSD1306ClearScreen();
	SSD1306SendCommand(SSD1306_CMD_DEACTIVATE_SCROLL);
}

  


/**
 * Sends a single command to the display. Chip select must be set before running this
 * @param cmd
 */
void SSD1306SendCommand(uint8_t cmd)
{
#if SSD1306_SPI == 1
    SSD1306_CS_PORT &= ~(1 << ssd1306csPinPositions[currentDisplay]);
    SSD1306_CLEAR_DC();
    Spi1Transmit(cmd);
    SSD1306_SET_DC();
    SSD1306_CS_PORT |= (1 << ssd1306csPinPositions[currentDisplay]);
#elif SSD1306_I2C == 1
    I2CStart((SSD1306_ADDRESS << 1) | 0);
    I2CByte(SSD1306_CMD_SEND_CMD);    // 0x00 for command, 0x40 for data
    I2CByte(cmd);
    i2c_stop();
#endif
}



/**
 * Sends commands to the display. Chip select must be set before running this
 * @param cmd
 */
void SSD1306SendMoreCommands(uint8_t* cmd)
{
#if SSD1306_SPI == 1
    SSD1306_CS_PORT &= ~(1 << ssd1306csPinPositions[currentDisplay]);
    SSD1306_CLEAR_DC();
    while(*cmd) Spi1Transmit(*cmd++);
    SSD1306_SET_DC();
    SSD1306_CS_PORT |= (1 << ssd1306csPinPositions[currentDisplay]);
#elif SSD1306_I2C == 1
    I2CStart((SSD1306_ADDRESS << 1) | 0);
    I2CByte(SSD1306_CMD_SEND_CMD);    // 0x00 for command, 0x40 for data
    while(*cmd) I2CByte(*cmd++);
    i2c_stop();
#endif
}



/**
 * Sends commands to the display. Chip select must be set before running this
 * @param cmd
 */
void SSD1306SendCommandArray(uint8_t cmds[], uint16_t cmdlen)
{
#if SSD1306_SPI == 1
    SSD1306_CS_PORT &= ~(1 << ssd1306csPinPositions[currentDisplay]);
    SSD1306_CLEAR_DC();
    for(uint8_t i = 0; i < cmdlen; i++) {
        Spi1Transmit(cmds[i]);
    }
    
    SSD1306_SET_DC();
    SSD1306_CS_PORT |= (1 << ssd1306csPinPositions[currentDisplay]);
    
#elif SSD1306_I2C == 1
    I2CStart((SSD1306_ADDRESS << 1) | 0);
    I2CByte(SSD1306_CMD_SEND_CMD);    // 0x00 for command, 0x40 for data
    
    for(uint8_t i = 0; i < cmdlen; i++) {
        I2CByte(cmds[i]);
    }
    
    i2c_stop();
#endif
}



/**
 * Sends a single data byte to the display. Chip select must be set before running this
 * @param data
 */
void SSD1306SendData(uint8_t data)
{
#if SSD1306_SPI == 1
    SSD1306_CS_PORT &= ~(1 << ssd1306csPinPositions[currentDisplay]);
    SSD1306_SET_DC();
    Spi1Transmit(data);
    SSD1306_CS_PORT |= (1 << ssd1306csPinPositions[currentDisplay]);
#elif SSD1306_I2C == 1
    I2CStart((SSD1306_ADDRESS << 1) | 0);
    I2CByte(SSD1306_CMD_SEND_DATA);    // 0x00 for command, 0x40 for data
    I2CByte(data);
    i2c_stop();
#endif
}



/**
 * Sends data bytes to the display. Chip select must be set before running this
 * @param data
 */
void SSD1306SendMoreData(uint8_t* data)
{
#if SSD1306_SPI == 1
    SSD1306_CS_PORT &= ~(1 << ssd1306csPinPositions[currentDisplay]);
    SSD1306_SET_DC();
    while(*data) Spi1Transmit(*data++);
    SSD1306_CS_PORT |= (1 << ssd1306csPinPositions[currentDisplay]);
#elif SSD1306_I2C == 1
    I2CStart((SSD1306_ADDRESS << 1) | 0);
    I2CByte(SSD1306_CMD_SEND_DATA);    // 0x00 for command, 0x40 for data
    while(*data) I2CByte(*data++);
    i2c_stop();
#endif
}



/**
 * Sends data to the display. Chip select must be set before running this
 * @param cmd
 */
void SSD1306SendDataArray(uint8_t data[], uint16_t datalen)
{
    #if SSD1306_SPI == 1
    SSD1306_CS_PORT &= ~(1 << ssd1306csPinPositions[currentDisplay]);
    SSD1306_SET_DC();
    
    for(uint8_t i = 0; i < datalen; i++) {
        Spi1Transmit(data[i]);
    }
    
    SSD1306_CS_PORT |= (1 << ssd1306csPinPositions[currentDisplay]);
    
#elif SSD1306_I2C == 1
    I2CStart((SSD1306_ADDRESS << 1) | 0);
    I2CByte(SSD1306_CMD_SEND_DATA);    // 0x00 for command, 0x40 for data
    
    for(uint8_t i = 0; i < datalen; i++) {
        I2CByte(data[i]);
    }
    
    i2c_stop();
#endif
}



/**
 * Clears the ssd1306 display
 */
void SSD1306ClearScreen()
{
#if !defined(SSD1306_DRAW_IMMEDIATE) || SSD1306_DRAW_IMMEDIATE < 1
    for (uint8_t i = 0; i < SSD1306_HEIGHT/8; i++){
		memset(SSD1306DisplayBuffer[i], 0x00, sizeof(SSD1306DisplayBuffer[i]));
		SSD1306GoToPixelPosition(0,i);
		SSD1306SendDataArray(SSD1306DisplayBuffer[i], sizeof(SSD1306DisplayBuffer[i]));
	}
#else
    
    unsigned char clearScreenBuffer[64] = {0};
    
    for (uint8_t i = 0; i < SSD1306_HEIGHT/8; i++){
		SSD1306GoToPixelPosition(0,i);
		SSD1306SendDataArray(clearScreenBuffer, sizeof(clearScreenBuffer));
	}
#endif
	SSD1306GoToPixelPosition(0, 0);
}



/**
 * Sends the stop scrolling command
 */
void SSD1306StopScroll() {
    SSD1306SendCommand(SSD1306_CMD_DEACTIVATE_SCROLL);
}



/**
 * Inverts the ssd1306 display
 * @param invert whether or not to invert
 */
void SSD1306SetInvert(bool invert)
{
    if (!invert) {
		SSD1306SendCommand(SSD1306_CMD_NORMAL_DISPLAY);
	}
	else {
		SSD1306SendCommand(SSD1306_CMD_INVERT_DISPLAY);
	}
}



/**
 * 
 * @param gotoSleep Whether or not to set to sleep
 */
extern void SSD1306SetSleep(bool gotoSleep)
{
    if (!gotoSleep) {
		SSD1306SendCommand(SSD1306_CMD_DISPLAY_ON);
	}
	else
	{
		SSD1306SendCommand(SSD1306_CMD_DISPLAY_OFF);
	}
}



/**
 * Sets the contrast of the display
 * @param contrast The contrast value
 */
void SSD1306SetContrast(uint8_t contrast) {
	uint8_t commandSequence[2] = {SSD1306_CMD_SET_CONTRAST, contrast};
	SSD1306SendCommandArray(commandSequence, 2);
}



/**
 * @brief Has the display start scrolling towards the right
 * 
 * 
 * @param start -The scroll start point
 * @param stop  -The scroll stop point
 */
void SSD1306StartScrollRight(uint8_t start, uint8_t stop)
{
    uint8_t cmds[] = {
		SSD1306_CMD_HORIZONTAL_SCROLL_RIGHT, 0x00,start,0x00,stop,0x00,0xff,SSD1306_CMD_ACTIVATE_SCROLL
	};
	SSD1306SendCommandArray(cmds,sizeof(cmds));
}



/**
 * @brief Has the display start scrolling towards the left
 * 
 * 
 * @param start -The scroll start point
 * @param stop  -The scroll stop point
 */
void SSD1306StartScrollLeft(uint8_t start, uint8_t stop)
{
    uint8_t cmds[] = {
		SSD1306_CMD_HORIZONTAL_SCROLL_LEFT, 0x00,start,0x00,stop,0x00,0xff,SSD1306_CMD_ACTIVATE_SCROLL
	};
	SSD1306SendCommandArray(cmds,sizeof(cmds));
}



/**
 * Goes the the position on the display
 * @param x The x position on the OLED screen
 * @param y The y position on the OLED screen 
 * @param fontSize The size of the font width
 */
void SSD1306GoToPosition(uint8_t x, uint8_t y, uint8_t fontSize) {
	x = x * fontSize;
	SSD1306GoToPixelPosition(x,y);
}



/**
 * @brief Goes to the exact pixel on the OLED screen
 * 
 * 
 * @param x The x position on the OLED screen
 * @param y The y position on the OLED screen 
 */
void SSD1306GoToPixelPosition(uint8_t x, uint8_t y) {
	if( x > (SSD1306_WIDTH) || y > (SSD1306_HEIGHT/8-1)) {
		return;// out of display
	}
	cursorPosX=x;
	cursorPosY=y;
	uint8_t commandSequence[4] = {0xb0+y, 0x21, x, 0x7f};
	SSD1306SendCommandArray(commandSequence, sizeof(commandSequence));
}



/**
 * @brief Puts a byte into memory
 * 
 * 
 * @param c -The char
 */
void SSD1306PutChar(char c) 
{
//    SSD1306DisplayBuffer[cursorPosY][cursorPosX] = c;
    
#if defined(SSD1306_DRAW_IMMEDIATE) && SSD1306_DRAW_IMMEDIATE > 0
                
        SSD1306SendData(c);
#else
        SSD1306DisplayBuffer[cursorPosY][cursorPosX] = c;
                
#endif

}




/**
 * @brief puts a string into memory
 * 
 * 
 * @param s 
 */
void SSD1306PutString(char* s) {
	while (*s) {
		unsigned char c = (*s++);
		SSD1306PutChar(c);
	}
}


#if defined(__AVR)
/**
 * @brief Writes a string pointer to the screen using progmem
 * 
 * 
 * @param progmemS -The progmem string to write
 */
void SSD1306PutP(PGM_P progmemS) {
	register uint8_t c;
	while ((c = pgm_read_byte(progmemS++))) {
		Put(c);
	}
}
#endif


 /**
  * @brief Writes a char onto the screen
  * 
  * 
  * @param c -The char
  */
 void SSD1306PutFont(char c, const char fontSheet[], uint8_t fontSheetCharacterLength) {

 	switch (c)
	{
		//Backspace
		case '\b':
			SSD1306GoToPosition(cursorPosX, cursorPosY,fontSheetCharacterLength);
			SSD1306PutFont(' ', fontSheet, fontSheetCharacterLength);
			SSD1306GoToPosition(cursorPosX, cursorPosY,fontSheetCharacterLength);
		break;
		
		//Tab
		case '\t':
			if( (cursorPosX*4) < (SSD1306_WIDTH / fontSheetCharacterLength*4) )
			{
				SSD1306GoToPosition(cursorPosX*4, cursorPosY,fontSheetCharacterLength);
			}
			else
			{
				SSD1306GoToPosition(SSD1306_WIDTH / fontSheetCharacterLength, cursorPosY,fontSheetCharacterLength);
			}

		break;
		
		//Next line
		case '\n':
			if(cursorPosY < (SSD1306_HEIGHT/8-1)){
				SSD1306GoToPosition(cursorPosX, cursorPosY,fontSheetCharacterLength);
			}

		break;
		
		//Carriage return
		case '\r':
			SSD1306GoToPosition(0, cursorPosY,fontSheetCharacterLength);

		break;
		
		//Clear screen
		case '\f':
			SSD1306ClearScreen();

		break;

		default:
			if(((cursorPosX-fontSheetCharacterLength)<=SSD1306_WIDTH))
			{
				for (uint8_t j = 0; j < fontSheetCharacterLength; j++)
				{
#if defined(SSD1306_DRAW_IMMEDIATE) && SSD1306_DRAW_IMMEDIATE > 0
                
               SSD1306SendData(fontSheet[((c - ' ') * fontSheetCharacterLength)+j]);
#else
                SSD1306DisplayBuffer[cursorPosY][cursorPosX+j] = fontSheet[((c - ' ') * fontSheetCharacterLength)+j];
                
#endif		
				}
				cursorPosX+=fontSheetCharacterLength;
			}
			
		break;
	}
 }



 /**
  * @brief Writes a string onto the screen
  * 
  * 
  * @param s 
  */
 void SSD1306PutFontString(char* s, const char fontSheet[], uint8_t fontSheetCharacterLength) {
 	for(uint8_t i = 0; i < strlen(s); i++)
 	{
		char c = s[i];
		switch (c)
		{
		//Backspace
		case '\b':
			SSD1306GoToPosition(cursorPosX, cursorPosY,fontSheetCharacterLength);
			SSD1306PutFont(' ', fontSheet, fontSheetCharacterLength);
			SSD1306GoToPosition(cursorPosX, cursorPosY,fontSheetCharacterLength);
		break;
		
		//Tab
		case '\t':
			if( (cursorPosX*4) < (SSD1306_WIDTH / fontSheetCharacterLength*4) )
			{
				SSD1306GoToPosition(cursorPosX*4, cursorPosY,fontSheetCharacterLength);
			}
			else
			{
				SSD1306GoToPosition(SSD1306_WIDTH / fontSheetCharacterLength, cursorPosY,fontSheetCharacterLength);
			}

		break;
		
		//Next line
		case '\n':
			if(cursorPosY < (SSD1306_HEIGHT/8-1)){
				SSD1306GoToPosition(cursorPosX, cursorPosY,fontSheetCharacterLength);
			}

		break;
		
		//Carriage return
		case '\r':
			SSD1306GoToPosition(0, cursorPosY,fontSheetCharacterLength);

		break;
		
		//Clear screen
		case '\f':
			SSD1306ClearScreen();

		break;

		default:
			if(cursorPosX+fontSheetCharacterLength > SSD1306_WIDTH) {
				break;
			}
			for (uint8_t j = 0; j < fontSheetCharacterLength; j++)
			{
#if defined(SSD1306_DRAW_IMMEDIATE) && SSD1306_DRAW_IMMEDIATE > 0
                
               SSD1306SendData(fontSheet[((c - ' ') * fontSheetCharacterLength)+j]);
#else
                SSD1306DisplayBuffer[cursorPosY][cursorPosX+j] = fontSheet[((c - ' ') * fontSheetCharacterLength)+j];
                
#endif
			}
			cursorPosX+=fontSheetCharacterLength;
			
			
		break;
		}
	}
 }




 /**
  * @brief Writes a string onto the screen
  * 
  * 
  * @param s 
  */
 void SSD1306PutConstString(const char* s, const char fontSheet[], uint8_t fontSheetCharacterLength) {
 	
	 for(uint8_t i = 0; i < strlen(s); i++)
 	{
		char c = s[i];
		switch (c)
		{
		//Backspace
		case '\b':
			SSD1306GoToPosition(cursorPosX, cursorPosY,fontSheetCharacterLength);
			SSD1306PutFont(' ', fontSheet, fontSheetCharacterLength);
			SSD1306GoToPosition(cursorPosX, cursorPosY,fontSheetCharacterLength);
		break;
		
		//Tab
		case '\t':
			if( (cursorPosX*4) < (SSD1306_WIDTH / fontSheetCharacterLength*4) )
			{
				SSD1306GoToPosition(cursorPosX*4, cursorPosY,fontSheetCharacterLength);
			}
			else
			{
				SSD1306GoToPosition(SSD1306_WIDTH / fontSheetCharacterLength, cursorPosY,fontSheetCharacterLength);
			}

		break;
		
		//Next line
		case '\n':
			if(cursorPosY < (SSD1306_HEIGHT/8-1)){
				SSD1306GoToPosition(cursorPosX, cursorPosY,fontSheetCharacterLength);
			}

		break;
		
		//Carriage return
		case '\r':
			SSD1306GoToPosition(0, cursorPosY,fontSheetCharacterLength);

		break;
		
		//Clear screen
		case '\f':
			SSD1306ClearScreen();

		break;

		default:
			if(cursorPosX+fontSheetCharacterLength > SSD1306_WIDTH) {
				break;
			}
			for (uint8_t j = 0; j < fontSheetCharacterLength; j++)
			{
                
#if defined(SSD1306_DRAW_IMMEDIATE) && SSD1306_DRAW_IMMEDIATE > 0
                
               SSD1306SendData(fontSheet[((c - ' ') * fontSheetCharacterLength)+j]);
#else
                SSD1306DisplayBuffer[cursorPosY][cursorPosX+j] = fontSheet[((c - ' ') * fontSheetCharacterLength)+j];
                
#endif
                //SSD1306DisplayBuffer[cursorPosY][cursorPosX+j] = fontSheet[((c - ' ') * fontSheetCharacterLength)+j];
                
                /*
				if(useGraphicMode) 
				{
                    displayBuffer[cursorPosY][cursorPosX+j] = fontSheet[((c - ' ') * fontSheetCharacterLength)+j];
				}
				else 
				{
					SendData(fontSheet[((c - ' ') * fontSheetCharacterLength)+j]);
				}
				*/	
			}
			cursorPosX+=fontSheetCharacterLength;
			
			
		break;
		}
	}
 }



/**
 * @brief Draws a single pixel on the screen
 * 
 * 
 * @param x 	-The x position
 * @param y 	-The y position
 * @param color -The color of pixel to draw
 * @return uint8_t 1 if out of display, 0 if worked
 */
uint8_t SSD1306DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
	
	uint16_t index = 0;
    index = (y / 8);
    
	if( x > SSD1306_WIDTH-1 || y > (SSD1306_HEIGHT-1)) return 1; // out of Display
	
    
#if defined(SSD1306_DRAW_IMMEDIATE) && SSD1306_DRAW_IMMEDIATE > 0
    SSD1306GoToPixelPosition(x,index);
    
    if( color == SSD1306_WHITE)
	{
        SSD1306SendData((1 << (y % 8)));
	}
	else
	{
        SSD1306SendData(~(1 << (y % 8)));
	}
    
#else
    if( color == SSD1306_WHITE)
	{
		SSD1306DisplayBuffer[index][x] |= (1 << (y % 8));
	}
	else
	{
		SSD1306DisplayBuffer[index][x] &= ~(1 << (y % 8));
	}
    
#endif
    
   
	return 0;
	
}



/**
 * @brief Draws a line onto the screen
 * 
 * 
 * @param x1 	-The starting x position
 * @param y1 	-The starting y position
 * @param x2 	-The ending x position
 * @param y2 	-The ending y position
 * @param color -The color of line to draw
 * @return uint8_t The result of drawing onto the oled display
 */
uint8_t SSD1306DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) {
	uint8_t result;
	
	int dx =  abs(x2-x1), sx = x1<x2 ? 1 : -1;
	int dy = -abs(y2-y1), sy = y1<y2 ? 1 : -1;
	int err = dx+dy, e2; /* error value e_xy */
	
	while(1) {
		result = SSD1306DrawPixel(x1, y1, color);
		if (x1==x2 && y1==y2) break;
		e2 = 2*err;
		if (e2 > dy) {
			err += dy; x1 += sx;
		}
		
		if (e2 < dx) {
			err += dx; y1 += sy;
		}
	}
			
	return result;
	
}



/**
 * @brief Draws a rectangle onto the screen
 * 
 * 
 * @param px1 	-The starting x pixel position
 * @param py1 	-The starting y pixel position
 * @param px2 	-The ending x pixel position
 * @param py2 	-The ending y pixel position
 * @param color -The color of rectangle outline to draw
 * @return uint8_t The result of drawing onto the oled display
 */
uint8_t SSD1306DrawRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color) {
	uint8_t result;
			
	result = SSD1306DrawLine(px1, py1, px2, py1, color);
	result = SSD1306DrawLine(px2, py1, px2, py2, color);
	result = SSD1306DrawLine(px2, py2, px1, py2, color);
	result = SSD1306DrawLine(px1, py2, px1, py1, color);
			
	return result;
}



/**
 * @brief Draws a filled rectangle onto the screen
 * 
 * 
 * @param px1 	-The starting x pixel position
 * @param py1 	-The starting y pixel position
 * @param px2 	-The ending x pixel position
 * @param py2 	-The ending y pixel position
 * @param color -The fill color of rectangle to draw
 * @return uint8_t The result of drawing onto the oled display
 */
uint8_t SSD1306FillRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color) {
	uint8_t result;
			
	if( px1 > px2) {
        
        px1 ^= px2;
        px2 ^= px1;
        px1 ^= px2;
		
        py1 ^= py2;
        py2 ^= py1;
        py1 ^= py2;
        
        //Removed because not good. No need for temp variable. Left incase things go horribly wrong.
//        
//        uint8_t temp = px1;
//		px1 = px2;
//		px2 = temp;
//        
//		temp = py1;
//		py1 = py2;
//		py2 = temp;
	}
	for (uint8_t i=0; i<=(py2-py1); i++) {
		result = SSD1306DrawLine(px1, py1+i, px2, py1+i, color);
	}
			
	return result;
}



/**
 * @brief Draws a circle onto the oled's screen
 * 
 * 
 * @param centerX -The center x position of the circle drawn on the screen
 * @param centerY -The center y position of the circle drawn on the screen
 * @param radius   -The radius of the circle
 * @param color    -The color of the circles outline
 * @return uint8_t -The result of drawing onto the screen
 */
uint8_t SSD1306DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, uint8_t color) {
	uint8_t result;
			
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;
			
	result = SSD1306DrawPixel(centerX  , centerY+radius, color);
	result = SSD1306DrawPixel(centerX  , centerY-radius, color);
	result = SSD1306DrawPixel(centerX+radius, centerY  , color);
	result = SSD1306DrawPixel(centerX-radius, centerY  , color);
			
	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
				
		result = SSD1306DrawPixel(centerX + x, centerY + y, color);
		result = SSD1306DrawPixel(centerX - x, centerY + y, color);
		result = SSD1306DrawPixel(centerX + x, centerY - y, color);
		result = SSD1306DrawPixel(centerX - x, centerY - y, color);
		result = SSD1306DrawPixel(centerX + y, centerY + x, color);
		result = SSD1306DrawPixel(centerX - y, centerY + x, color);
		result = SSD1306DrawPixel(centerX + y, centerY - x, color);
		result = SSD1306DrawPixel(centerX - y, centerY - x, color);
	}
	return result;
}



/**
 * @brief Draws a filled circle onto the oled's screen
 * 
 * 
 * @param center_x -The center x position of the circle drawn on the screen
 * @param center_y -The center y position of the circle drawn on the screen
 * @param radius   -The radius of the circle
 * @param color    -The fill color of the circle
 * @return uint8_t -The result of drawing onto the screen
 */
uint8_t SSD1306FillCircle(uint8_t center_x, uint8_t center_y, uint8_t radius, uint8_t color) {
	uint8_t result;
	for(uint8_t i=0; i<= radius;i++){
		result = SSD1306DrawCircle(center_x, center_y, i, color);
	}
	return result;
}



/**
 * @brief Draws a bitmap onto the OLED screen
 * 
 * 
 * @param x -The x position to draw at
 * @param y -The y position to draw at
 * @param picture -const pointer for the picture to draw
 * @param width   -The width of the bitmap
 * @param height  -The height of the bitmap
 * @param color   -The color of the bitmap
 * @return uint8_t The result of drawing onto the screen
 */
uint8_t SSD1306DrawBitmap(uint8_t x, uint8_t y, const uint8_t *picture, uint8_t width, uint8_t height, uint8_t color) {
	uint8_t result,i,j, byteWidth = (width+7)/8;
	for (j = 0; j < height; j++) {
		for(i=0; i < width;i++){
#if defined(__AVR)
			if(pgm_read_byte(picture + j * byteWidth + i / 8) & (128 >> (i & 7)))
#else
            if(*(picture + j * byteWidth + i / 8) & (128 >> (i & 7)))
#endif
			{
				result = SSD1306DrawPixel(x+i, y+j, color);
			} 
			else 
			{
				result = SSD1306DrawPixel(x+i, y+j, !color);
			}
		}
	}
	return result;
}



#if !defined(SSD1306_DRAW_IMMEDIATE) || SSD1306_DRAW_IMMEDIATE < 1
/**
 * Clears the display buffer
 */
void SSD1306ClearBuffer()
{
    for (uint8_t i = 0; i < SSD1306_HEIGHT/8; i++){
		memset(SSD1306DisplayBuffer[i], 0x00, sizeof(SSD1306DisplayBuffer[i]));
	}
}



/**
 * Updates the ssd1306 display
 */
void SSD1306Update() {
    
    //volatile uint16_t size = SSD1306_WIDTH*(SSD1306_HEIGHT/8);
    
	SSD1306GoToPixelPosition(0,0);
#if defined(__AVR)
    SSD1306SendDataArray(&SSD1306DisplayBuffer[0][0], SSD1306_WIDTH*SSD1306_HEIGHT/8);
#else
    for(uint8_t i = 0; i < 8; i++)
    {
        SSD1306SendDataArray(SSD1306DisplayBuffer[i], sizeof(SSD1306DisplayBuffer[i]));
    }
    
#endif
    
#if defined(SSD1306_AUTO_CLEAR_BUFF_ON_UPDATE)
    SSD1306ClearBuffer();
#endif
}


/**
 * @brief Checks the status of the display buffer at position
 * @param x -The x position to check
 * @param y -The y position to check
 * @return uint8_t The status of the display buffer
 */
uint8_t SSD1306CheckBuffer(uint8_t x, uint8_t y)
{
    if( x > SSD1306_WIDTH-1 || y > (SSD1306_HEIGHT-1)) return 0; // out of Display
	return SSD1306DisplayBuffer[(y / (SSD1306_HEIGHT/8))][x] & (1 << (y % (SSD1306_HEIGHT/8)));
}
#endif


#endif