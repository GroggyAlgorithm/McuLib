/**
 * \file OLED_SSD1306.h
 * \author Tim Robbins
 * \brief Source file for SSD1306 OLED display
 * \todo - Fix display buffer sizing
 */ 
#include "OLED_SSD1306.h"

#if defined(__cplusplus) && defined(__AVR) && !defined(__NO_INCLUDE_OLED_CPP)



#ifndef OLED_SSD1306_CPP_
#define OLED_SSD1306_CPP_





//#pragma region CONSTRUCTOR_DESTRUCTOR_INIT

/**
 * \brief Class constructor
 * \param newControlPinPort -The port for the control pins(ex. &PORTD)
 * \param csPinPosition		-The position of the CS pin
 * \param dcPinPosition		-The position of the DC pin
 * \param resetPinPosition	-The position of the RESET pin
 * \param useI2CTransmit #ifdef TWCR, give option for selecting TWI for communication
 * 
 */
OLED_SSD1306::OLED_SSD1306(volatile uint8_t* newControlPinPort, uint8_t csPinPosition, uint8_t dcPinPosition, uint8_t resetPinPosition
#ifdef TWCR
,bool useI2CTransmit
#endif
) 
{
	controlPinPort = newControlPinPort;
	chipSelectPin = csPinPosition;
	dcPin = dcPinPosition;
	resetPin = resetPinPosition;
	useGraphicMode = false;
	
	cursorPosX = 0;
	cursorPosY = 0;
	charMode = 1;
	
	#ifdef TWCR
	
	useI2C = useI2CTransmit;
	
	#endif
	screenWidth = 0;
	screenHeight = 0;
}



/**
 * \brief Class constructor
 * \param newControlPinPort -The port for the control pins(ex. &PORTD)
 * \param csPinPosition		-The position of the CS pin
 * \param dcPinPosition		-The position of the DC pin
 * \param resetPinPosition	-The position of the RESET pin
 * \param useI2CTransmit 	-#ifdef TWCR, give option for selecting TWI for communication
 * \param newUseGraphicMode -If using GRAPIC MODE on our OLED or if it is just text based
 */
OLED_SSD1306::OLED_SSD1306(volatile uint8_t* newControlPinPort, uint8_t csPinPosition, uint8_t dcPinPosition, uint8_t resetPinPosition,
#ifdef TWCR
bool useI2CTransmit,
#endif

bool newUseGraphicMode

) 
{
	controlPinPort = newControlPinPort;
	chipSelectPin = csPinPosition;
	dcPin = dcPinPosition;
	resetPin = resetPinPosition;
	useGraphicMode = newUseGraphicMode;
	
	cursorPosX = 0;
	cursorPosY = 0;
	charMode = 1;
	
	#ifdef TWCR
	
	useI2C = useI2CTransmit;
	
	#endif
	screenWidth = 0;
	screenHeight = 0;
}



/**
 * \brief Class constructor
 * \param newControlPinPort -The port for the control pins(ex. &PORTD)
 * \param csPinPosition		-The position of the CS pin
 * \param dcPinPosition		-The position of the DC pin
 * \param resetPinPosition	-The position of the RESET pin
 * \param useI2CTransmit 	-#ifdef TWCR, give option for selecting TWI for communication
 * \param newUseGraphicMode -If using GRAPIC MODE on our OLED or if it is just text based
 * \param newCharMode		-The char mode for our OLED to base character size on
 */
OLED_SSD1306::OLED_SSD1306(volatile uint8_t* newControlPinPort, uint8_t csPinPosition, uint8_t dcPinPosition, uint8_t resetPinPosition,
#ifdef TWCR
bool useI2CTransmit,
#endif
bool newUseGraphicMode,
uint8_t newCharMode
) 
{
	controlPinPort = newControlPinPort;
	chipSelectPin = csPinPosition;
	dcPin = dcPinPosition;
	resetPin = resetPinPosition;
	useGraphicMode = newUseGraphicMode;
	
	cursorPosX = 0;
	cursorPosY = 0;
	charMode = newCharMode;
	
	#ifdef TWCR
	
	useI2C = useI2CTransmit;
	
	#endif
	screenWidth = 0;
	screenHeight = 0;
}



/**
 * \brief Destroy the oled ssd1306::oled ssd1306 object
 * 
 * 
 */
OLED_SSD1306::~OLED_SSD1306() {
	controlPinPort = 0;
	chipSelectPin = 0;
	dcPin = 0;
	resetPin = 0;
	#ifdef TWCR
		useI2C = 0;
	#endif
	useGraphicMode = 0;
	ClearBuffer();
	
	charMode = 0;
	cursorPosX = 0;
	cursorPosY = 0;
	screenWidth = 0;
	screenHeight = 0;
}



/**
 * \brief Initializes the OLED
 * \param displayOn   		-If our OLED starts off as ON
 * \param newMainFont 		-The main font to be using for the OLED
 * \param newScreenWidth 	-The width of our OLED screen
 * \param newScreenHeight 	-The height of our OLED screen
 */
void OLED_SSD1306::Initialize(bool displayOn,
	uint8_t newScreenWidth, uint8_t newScreenHeight
)
{
	//Set the objects width and height to the values passed
	screenWidth = newScreenWidth;
	screenHeight = newScreenHeight;
	
	//Initialize new configuration variable based on the height passed
	uint8_t comConfig = (newScreenHeight == 64) ? 0x12 : (newScreenHeight == 32) ? 0x02 : 0x12;

	//Initialize the objects address variable based on the height passed
	address = (newScreenHeight == 32) ? 0x3C : 0x3D;

	//Initialize the objects init sequence
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
		0xA8, (uint8_t)(newScreenHeight-1), // Set multiplex ratio(1 to 64)
		0xA4,            // Output RAM to Display
		// 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
		0xD3, 0x00,      // Set display offset. 00 = no offset
		0xD5,            // --set display clock divide ratio/oscillator frequency
		0xF0,            // --set divide ratio
		0xD9, 0x22,      // Set pre-charge period
		0xDA, comConfig,// Set com pins hardware configuration
		0xDB,            // --set vcomh
		0x20,            // 0x20,0.77xVcc
		0x8D, 0x14,      // Set DC-DC enable
		(displayOn ? (uint8_t)SSD1306_CMD_DISPLAY_ON : (uint8_t)SSD1306_CMD_DISPLAY_OFF) //If the boolean passed is true, turn on, else turn off
		// dispAttr
		
	};

	
	
	//If the two wire register exists, only set up our pins if SPI mode selected
	//Else, just set up the SPI pins
	#ifdef TWCR
	
	if(!useI2C) {
		*controlPinPort |= (1<<chipSelectPin);
		*controlPinPort |= (1<<dcPin);
		*controlPinPort |= (1<<resetPin);
		*controlPinPort &= ~(1<<resetPin);
		delayForMilliseconds(10);
		*controlPinPort |= (1<<resetPin);
	}
	

	
		
	
	#else
	
	//On the control port, run the reset on the oled
	*controlPinPort |= (1<<chipSelectPin);
	*controlPinPort |= (1<<dcPin);
	*controlPinPort |= (1<<resetPin);
	*controlPinPort &= ~(1<<resetPin);
	delayForMilliseconds(10);
	*controlPinPort |= (1<<resetPin);
	
	
	#endif
	
	//Send our initialization sequence
	SendCommand(init_sequence, sizeof(init_sequence));

	//Clear the OLED screen and deactivate any scrolling
	ClearScreen();
	SendCommand(SSD1306_CMD_DEACTIVATE_SCROLL);
	
}


//#pragma endregion



//#pragma region SENDING_CMDS_AND_DATA


/**
 * \brief Sends a single command to the OLED
 * 
 * 
 * \param cmd -The command to send
 */
void OLED_SSD1306::SendCommand(uint8_t cmd) {
	#if defined TWCR
	if(useI2C) {
		I2CStart((address << 1) | 0);
		I2CStart(SSD1306_CMD_SEND_CMD);    // 0x00 for command, 0x40 for data
		I2CStart(cmd);
		i2c_stop();
	}
	else {
		*controlPinPort &= ~(1 << chipSelectPin);
		*controlPinPort &= ~(1 << dcPin);
		SpiTransmit(cmd);
		*controlPinPort |= (1 << chipSelectPin);
	}
	#else
	*controlPinPort &= ~(1 << chipSelectPin);
	*controlPinPort &= ~(1 << dcPin);
	SpiTransmit(cmd);
	*controlPinPort |= (1 << chipSelectPin);
	#endif
}



/**
 * \brief Sends pointer commands to the OLED
 * 
 * 
 * \param cmd -The commands to send
 */
void OLED_SSD1306::SendCommand(uint8_t *cmd) {
	#if defined TWCR
	if(useI2C) {
		I2CStart((address << 1) | 0);
		I2CStart(SSD1306_CMD_SEND_CMD);    // 0x00 for command, 0x40 for data
		while(*cmd) I2CStart(*cmd++);
		i2c_stop();
	}
	else {
		*controlPinPort &= ~(1 << chipSelectPin);
		*controlPinPort &= ~(1 << dcPin);
		while(*cmd) SpiTransmit(*cmd++);
		*controlPinPort |= (1 << chipSelectPin);
	}
	#else
	*controlPinPort &= ~(1 << chipSelectPin);
	*controlPinPort &= ~(1 << dcPin);
	while(*cmd) SpiTransmit(*cmd++);
	*controlPinPort |= (1 << chipSelectPin);
	#endif
}



/**
 * \brief Sends an array of commands to the OLED
 * 
 * 
 * \param cmd  -The commands to send
 * \param size -The size of the array
 */
void OLED_SSD1306::SendCommand(uint8_t cmd[], uint16_t size) {
	#if defined TWCR
	if(useI2C) {
		I2CStart((address << 1) | 0);
		I2CStart(SSD1306_CMD_SEND_CMD);    // 0x00 for command, 0x40 for data
		for (uint16_t i = 0; i<size; i++) I2CStart(cmd[i]);
		i2c_stop();
	}
	else {
		*controlPinPort &= ~(1 << chipSelectPin);
		*controlPinPort &= ~(1 << dcPin);
		for (uint16_t i = 0; i<size; i++) SpiTransmit(cmd[i]);
		*controlPinPort |= (1 << chipSelectPin);
	}
	#else
	*controlPinPort &= ~(1 << chipSelectPin);
	*controlPinPort &= ~(1 << dcPin);
	for (uint16_t i = 0; i<size; i++) SpiTransmit(cmd[i]);
	*controlPinPort |= (1 << chipSelectPin);
	#endif
}



/**
 * \brief Sends a single byte of data to the OLED
 * 
 * 
 * \param data -The data to send
 */
void OLED_SSD1306::SendData(uint8_t data) {
	#if defined TWCR
	if(useI2C) {
		I2CStart((address << 1) | 0);
		I2CStart(SSD1306_CMD_SEND_DATA);    // 0x00 for command, 0x40 for data
		I2CStart(data);
		i2c_stop();
	}
	else {
		*controlPinPort &= ~(1 << chipSelectPin);
		*controlPinPort |= (1 << dcPin);
		SpiTransmit(data);
		*controlPinPort |= (1 << chipSelectPin);
	}
	#else
	*controlPinPort &= ~(1 << chipSelectPin);
	*controlPinPort |= (1 << dcPin);
	SpiTransmit(data);
	*controlPinPort |= (1 << chipSelectPin);
	#endif
}



/**
 * \brief Sends a pointer of byte/bytes of data to the OLED
 * 
 * 
 * \param data -The datas to send
 */
void OLED_SSD1306::SendData(uint8_t* data) {
	#if defined TWCR
	if(useI2C) {
		I2CStart((address << 1) | 0);
		I2CStart(SSD1306_CMD_SEND_DATA);    // 0x00 for command, 0x40 for data
		while(*data) I2CStart(*data++);
		i2c_stop();
	}
	else {
		*controlPinPort &= ~(1 << chipSelectPin);
		*controlPinPort |= (1 << dcPin);
		while(*data) SpiTransmit(*data++);
		*controlPinPort |= (1 << chipSelectPin);
	}
	#else
	*controlPinPort &= ~(1 << chipSelectPin);
	*controlPinPort |= (1 << dcPin);
	while(*data) SpiTransmit(*data++);
	*controlPinPort |= (1 << chipSelectPin);
	#endif
}



/**
 * \brief Sends an array of bytes of data to the OLED
 * 
 * 
 * \param data -The datas to send
 * \param size -The size of the array
 */
void OLED_SSD1306::SendData(uint8_t data[], uint16_t size) {
	#if defined TWCR
	if(useI2C) {
		I2CStart((address << 1) | 0);
		I2CStart(SSD1306_CMD_SEND_DATA);    // 0x00 for command, 0x40 for data
		for (uint16_t i = 0; i<size; i++) I2CStart(data[i]);
		i2c_stop();
	}
	else {
		*controlPinPort &= ~(1 << chipSelectPin);
		*controlPinPort |= (1 << dcPin);
		for (uint16_t i = 0; i<size; i++) SpiTransmit(data[i]);
		*controlPinPort |= (1 << chipSelectPin);
	}
	#else
	*controlPinPort &= ~(1 << chipSelectPin);
	*controlPinPort |= (1 << dcPin);
	for (uint16_t i = 0; i<size; i++) SpiTransmit(data[i]);
	*controlPinPort |= (1 << chipSelectPin);
	#endif
}

//#pragma endregion



//#pragma region HELPER_FUNCTIONS



/**
 * \brief Clears the OLED screen
 * 
 * 
 */
void OLED_SSD1306::ClearScreen() {
	for (uint8_t i = 0; i < screenHeight/8; i++){
		memset(displayBuffer[i], 0x00, sizeof(displayBuffer[i]));
		GoToPixelPosition(0,i);
		SendData(displayBuffer[i], sizeof(displayBuffer[i]));
	}
	
	GoToPixelPosition(0, 0);
}



/**
 * \brief Goes the the position passed based on font size
 * 
 * 
 * \param x -The x position on the OLED screen
 * \param y -The y position on the OLED screen
 */
void OLED_SSD1306::GoToPosition(uint8_t x, uint8_t y, uint8_t fontSize) {
	x = x * fontSize;
	GoToPixelPosition(x,y);
}



/**
 * \brief Goes to the exact pixel on the OLED screen
 * 
 * 
 * \param x -The x position on the OLED screen
 * \param y -The y position on the OLED screen 
 */
void OLED_SSD1306::GoToPixelPosition(uint8_t x, uint8_t y) {
	if( x > (screenWidth) || y > (screenHeight/8-1)) {
		return;// out of display
	}
	cursorPosX=x;
	cursorPosY=y;
	uint8_t commandSequence[4] = {(uint8_t)(0xb0+y), (uint8_t)0x21, x, (uint8_t)0x7f};
	SendCommand(commandSequence, sizeof(commandSequence));
}




/**
 * \brief Sets the OLEDS screen inversion
 * 
 * 
 * \param invert -Whether of not the OLED should be inverted colors
 */
void OLED_SSD1306::SetInvert(bool invert) {
	if (!invert) {
		SendCommand(0xA6);
	}
	else {
		SendCommand(0xA7);
	}
}



/**
 * \brief Puts the OLED to sleep or wakes up the OLED
 * 
 * 
 * \param gotoSleep -Whether of not the OLED should be tired enough to fall asleep or if it should wake up
 */
void OLED_SSD1306::SetSleep(bool gotoSleep) {
	if (!gotoSleep) {
		SendCommand(SSD1306_CMD_DISPLAY_ON);
	}
	else
	{
		SendCommand(SSD1306_CMD_DISPLAY_OFF);
	}
}



/**
 * \brief Sets the contrast for the OLED
 * 
 * 
 * \param contrast -The contrast value of the OLED
 */
void OLED_SSD1306::SetContrast(uint8_t contrast) {
	uint8_t commandSequence[2] = {SSD1306_CMD_SET_CONTRAST, contrast};
	SendCommand(commandSequence, 2);
}



/**
 * \brief Sets the CHAR MODE size for the OLED
 * 
 * 
 * \param mode -The size mode for the oled text
 */
void OLED_SSD1306::SetCharMode(uint8_t mode) {
	charMode = mode;
}



//#pragma endregion


//#pragma region PRINTING_PUTTING








/**
 * \brief Prints a text char onto the screen
 * 
 * 
 * \param c -The char
 */
void OLED_SSD1306::PrintText(char c, const char fontSheet[], uint8_t fontSheetCharacterLength) {
	
	switch (c)
	{
		//Backspace
		case '\b':
			GoToPosition(cursorPosX-charMode, cursorPosY,fontSheetCharacterLength);
			Put(' ', fontSheet, fontSheetCharacterLength);
			GoToPosition(cursorPosX-charMode, cursorPosY,fontSheetCharacterLength);
		break;
		
		//Tab
		case '\t':
			if( (cursorPosX+charMode*4) < (screenWidth / fontSheetCharacterLength-charMode*4) )
			{
				GoToPosition(cursorPosX+charMode*4, cursorPosY,fontSheetCharacterLength);
			}
			else
			{
				GoToPosition(screenWidth / fontSheetCharacterLength, cursorPosY,fontSheetCharacterLength);
			}

		break;
		
		//Next line
		case '\n':
			if(cursorPosY < (screenHeight/8-1)){
				GoToPosition(cursorPosX, cursorPosY+charMode,fontSheetCharacterLength);
			}

		break;
		
		//Carriage return
		case '\r':
			GoToPosition(0, cursorPosY,fontSheetCharacterLength);

		break;
		
		//Clear screen
		case '\f':
			ClearScreen();

		break;

		default:
			if(((cursorPosX-fontSheetCharacterLength)<=screenWidth))
			{
				for (uint8_t j = 0; j < fontSheetCharacterLength; j++)
				{
					if(useGraphicMode) 
					{
						displayBuffer[cursorPosY][cursorPosX+j] = fontSheet[((c - ' ') * fontSheetCharacterLength)+j];
					}
					else 
					{
						SendData(fontSheet[((c - ' ') * fontSheetCharacterLength)+j]);
					}
					
				}
				cursorPosX+=fontSheetCharacterLength;
			}
			
		break;
	}

	
}
	




/**
 * \brief Puts a byte into memory
 * 
 * 
 * \param c -The char
 */
void OLED_SSD1306::Put(char c) {

	if(useGraphicMode) 
	{
		displayBuffer[cursorPosY][cursorPosX] = c;
	}
	else 
	{
		SendData(c);
	}
}




/**
 * \brief puts a string into memory
 * 
 * 
 * \param s 
 */
void OLED_SSD1306::Put(char* s) {
	while (*s) {
		unsigned char c = (*s++);
		Put(c);
	}
}



/**
 * \brief Writes a string pointer to the screen using progmem
 * 
 * 
 * \param progmemS -The progmem string to write
 */
void OLED_SSD1306::Put(PGM_P progmemS) {
	register uint8_t c;
	while ((c = pgm_read_byte(progmemS++))) {
		Put(c);
	}
}



 /**
  * \brief Writes a char onto the screen
  * 
  * 
  * \param c -The char
  */
 void OLED_SSD1306::Put(char c, const char fontSheet[], uint8_t fontSheetCharacterLength) {

 	switch (c)
	{
		//Backspace
		case '\b':
			GoToPosition(cursorPosX-charMode, cursorPosY,fontSheetCharacterLength);
			Put(' ', fontSheet, fontSheetCharacterLength);
			GoToPosition(cursorPosX-charMode, cursorPosY,fontSheetCharacterLength);
		break;
		
		//Tab
		case '\t':
			if( (cursorPosX+charMode*4) < (screenWidth / fontSheetCharacterLength-charMode*4) )
			{
				GoToPosition(cursorPosX+charMode*4, cursorPosY,fontSheetCharacterLength);
			}
			else
			{
				GoToPosition(screenWidth / fontSheetCharacterLength, cursorPosY,fontSheetCharacterLength);
			}

		break;
		
		//Next line
		case '\n':
			if(cursorPosY < (screenHeight/8-1)){
				GoToPosition(cursorPosX, cursorPosY+charMode,fontSheetCharacterLength);
			}

		break;
		
		//Carriage return
		case '\r':
			GoToPosition(0, cursorPosY,fontSheetCharacterLength);

		break;
		
		//Clear screen
		case '\f':
			ClearScreen();

		break;

		default:
			if(((cursorPosX-fontSheetCharacterLength)<=screenWidth))
			{
				for (uint8_t j = 0; j < fontSheetCharacterLength; j++)
				{
					if(useGraphicMode) 
					{
						displayBuffer[cursorPosY][cursorPosX+j] = fontSheet[((c - ' ') * fontSheetCharacterLength)+j];
					}
					else 
					{
						SendData(fontSheet[((c - ' ') * fontSheetCharacterLength)+j]);
					}
					
				}
				cursorPosX+=fontSheetCharacterLength;
			}
			
		break;
	}
 }



 /**
  * \brief Writes a string onto the screen
  * 
  * 
  * \param s 
  */
 void OLED_SSD1306::Put(char* s, const char fontSheet[], uint8_t fontSheetCharacterLength) {
 	for(uint8_t i = 0; i < strlen(s); i++)
 	{
		char c = s[i];
		switch (c)
		{
		//Backspace
		case '\b':
			GoToPosition(cursorPosX-charMode, cursorPosY,fontSheetCharacterLength);
			Put(' ', fontSheet, fontSheetCharacterLength);
			GoToPosition(cursorPosX-charMode, cursorPosY,fontSheetCharacterLength);
		break;
		
		//Tab
		case '\t':
			if( (cursorPosX+charMode*4) < (screenWidth / fontSheetCharacterLength-charMode*4) )
			{
				GoToPosition(cursorPosX+charMode*4, cursorPosY,fontSheetCharacterLength);
			}
			else
			{
				GoToPosition(screenWidth / fontSheetCharacterLength, cursorPosY,fontSheetCharacterLength);
			}

		break;
		
		//Next line
		case '\n':
			if(cursorPosY < (screenHeight/8-1)){
				GoToPosition(cursorPosX, cursorPosY+charMode,fontSheetCharacterLength);
			}

		break;
		
		//Carriage return
		case '\r':
			GoToPosition(0, cursorPosY,fontSheetCharacterLength);

		break;
		
		//Clear screen
		case '\f':
			ClearScreen();

		break;

		default:
			if(cursorPosX+fontSheetCharacterLength > screenWidth) {
				break;
			}
			for (uint8_t j = 0; j < fontSheetCharacterLength; j++)
			{
				if(useGraphicMode) 
				{
					displayBuffer[cursorPosY][cursorPosX+j] = fontSheet[((c - ' ') * fontSheetCharacterLength)+j];
				}
				else 
				{
					SendData(fontSheet[((c - ' ') * fontSheetCharacterLength)+j]);
				}
					
			}
			cursorPosX+=fontSheetCharacterLength;
			
			
		break;
		}
	}
 }




 /**
  * \brief Writes a string onto the screen
  * 
  * 
  * \param s 
  */
 void OLED_SSD1306::Put(const char* s, const char fontSheet[], uint8_t fontSheetCharacterLength) {
 	
	 for(uint8_t i = 0; i < strlen(s); i++)
 	{
		char c = s[i];
		switch (c)
		{
		//Backspace
		case '\b':
			GoToPosition(cursorPosX-charMode, cursorPosY,fontSheetCharacterLength);
			Put(' ', fontSheet, fontSheetCharacterLength);
			GoToPosition(cursorPosX-charMode, cursorPosY,fontSheetCharacterLength);
		break;
		
		//Tab
		case '\t':
			if( (cursorPosX+charMode*4) < (screenWidth / fontSheetCharacterLength-charMode*4) )
			{
				GoToPosition(cursorPosX+charMode*4, cursorPosY,fontSheetCharacterLength);
			}
			else
			{
				GoToPosition(screenWidth / fontSheetCharacterLength, cursorPosY,fontSheetCharacterLength);
			}

		break;
		
		//Next line
		case '\n':
			if(cursorPosY < (screenHeight/8-1)){
				GoToPosition(cursorPosX, cursorPosY+charMode,fontSheetCharacterLength);
			}

		break;
		
		//Carriage return
		case '\r':
			GoToPosition(0, cursorPosY,fontSheetCharacterLength);

		break;
		
		//Clear screen
		case '\f':
			ClearScreen();

		break;

		default:
			if(cursorPosX+fontSheetCharacterLength > screenWidth) {
				break;
			}
			for (uint8_t j = 0; j < fontSheetCharacterLength; j++)
			{
				if(useGraphicMode) 
				{
					displayBuffer[cursorPosY][cursorPosX+j] = fontSheet[((c - ' ') * fontSheetCharacterLength)+j];
				}
				else 
				{
					SendData(fontSheet[((c - ' ') * fontSheetCharacterLength)+j]);
				}
					
			}
			cursorPosX+=fontSheetCharacterLength;
			
			
		break;
		}
	}
 }
 


 
//#pragma endregion




//#pragma region GRAPHIC_COMMANDS_AND_ANIMATIONS



/**
 * \brief Has the display start scrolling towards the right
 * 
 * 
 * \param start -The scroll start point
 * \param stop  -The scroll stop point
 */
void OLED_SSD1306::StartScrollRight(uint8_t start, uint8_t stop) {
	
	uint8_t cmds[] = {
		SSD1306_CMD_HORIZONTAL_SCROLL_RIGHT, 0x00,start,0x00,stop,0x00,0xff,SSD1306_CMD_ACTIVATE_SCROLL
	};
	SendCommand(cmds,sizeof(cmds));
	
	
}



/**
 * \brief Has the display start scrolling towards the left
 * 
 * 
 * \param start -The scroll start point
 * \param stop  -The scroll stop point
 */
void OLED_SSD1306::StartScrollLeft(uint8_t start, uint8_t stop) {
	
	uint8_t cmds[] = {
		SSD1306_CMD_HORIZONTAL_SCROLL_LEFT, 0x00,start,0x00,stop,0x00,0xff,SSD1306_CMD_ACTIVATE_SCROLL
	};
	SendCommand(cmds,sizeof(cmds));
	
}



/**
 * \brief Draws a single pixel on the screen
 * 
 * 
 * \param x 	-The x position
 * \param y 	-The y position
 * \param color -The color of pixel to draw
 * \return uint8_t 1 if out of display, 0 if worked
 */
uint8_t OLED_SSD1306::DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
	
	
	if( x > screenWidth-1 || y > (screenHeight-1)) return 1; // out of Display
	
	if( color == SSD1306_WHITE)
	{
		displayBuffer[(y / 8)][x] |= (1 << (y % 8));
	}
	else
	{
		displayBuffer[(y / 8)][x] &= ~(1 << (y % 8));
	}
	
	return 0;
	
}



/**
 * \brief Draws a line onto the screen
 * 
 * 
 * \param x1 	-The starting x position
 * \param y1 	-The starting y position
 * \param x2 	-The ending x position
 * \param y2 	-The ending y position
 * \param color -The color of line to draw
 * \return uint8_t The result of drawing onto the oled display
 */
uint8_t OLED_SSD1306::DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) {
	uint8_t result;
	
	int dx =  abs(x2-x1), sx = x1<x2 ? 1 : -1;
	int dy = -abs(y2-y1), sy = y1<y2 ? 1 : -1;
	int err = dx+dy, e2; /* error value e_xy */
	
	while(1) {
		result = DrawPixel(x1, y1, color);
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
 * \brief Draws a rectangle onto the screen
 * 
 * 
 * \param px1 	-The starting x pixel position
 * \param py1 	-The starting y pixel position
 * \param px2 	-The ending x pixel position
 * \param py2 	-The ending y pixel position
 * \param color -The color of rectangle outline to draw
 * \return uint8_t The result of drawing onto the oled display
 */
uint8_t OLED_SSD1306::DrawRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color) {
	uint8_t result;
			
	result = DrawLine(px1, py1, px2, py1, color);
	result = DrawLine(px2, py1, px2, py2, color);
	result = DrawLine(px2, py2, px1, py2, color);
	result = DrawLine(px1, py2, px1, py1, color);
			
	return result;
}



/**
 * \brief Draws a filled rectangle onto the screen
 * 
 * 
 * \param px1 	-The starting x pixel position
 * \param py1 	-The starting y pixel position
 * \param px2 	-The ending x pixel position
 * \param py2 	-The ending y pixel position
 * \param color -The fill color of rectangle to draw
 * \return uint8_t The result of drawing onto the oled display
 */
uint8_t OLED_SSD1306::FillRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color) {
	uint8_t result;
			
	if( px1 > px2) {
		uint8_t temp = px1;
		px1 = px2;
		px2 = temp;
		temp = py1;
		py1 = py2;
		py2 = temp;
	}
	for (uint8_t i=0; i<=(py2-py1); i++) {
		result = DrawLine(px1, py1+i, px2, py1+i, color);
	}
			
	return result;
}



/**
 * \brief Draws a circle onto the oled's screen
 * 
 * 
 * \param centerX -The center x position of the circle drawn on the screen
 * \param centerY -The center y position of the circle drawn on the screen
 * \param radius   -The radius of the circle
 * \param color    -The color of the circles outline
 * \return uint8_t -The result of drawing onto the screen
 */
uint8_t OLED_SSD1306::DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, uint8_t color) {
	uint8_t result;
			
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;
			
	result = DrawPixel(centerX  , centerY+radius, color);
	result = DrawPixel(centerX  , centerY-radius, color);
	result = DrawPixel(centerX+radius, centerY  , color);
	result = DrawPixel(centerX-radius, centerY  , color);
			
	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
				
		result = DrawPixel(centerX + x, centerY + y, color);
		result = DrawPixel(centerX - x, centerY + y, color);
		result = DrawPixel(centerX + x, centerY - y, color);
		result = DrawPixel(centerX - x, centerY - y, color);
		result = DrawPixel(centerX + y, centerY + x, color);
		result = DrawPixel(centerX - y, centerY + x, color);
		result = DrawPixel(centerX + y, centerY - x, color);
		result = DrawPixel(centerX - y, centerY - x, color);
	}
	return result;
}



/**
 * \brief Draws a filled circle onto the oled's screen
 * 
 * 
 * \param center_x -The center x position of the circle drawn on the screen
 * \param center_y -The center y position of the circle drawn on the screen
 * \param radius   -The radius of the circle
 * \param color    -The fill color of the circle
 * \return uint8_t -The result of drawing onto the screen
 */
uint8_t OLED_SSD1306::FillCircle(uint8_t center_x, uint8_t center_y, uint8_t radius, uint8_t color) {
	uint8_t result;
	for(uint8_t i=0; i<= radius;i++){
		result = DrawCircle(center_x, center_y, i, color);
	}
	return result;
}



/**
 * \brief Draws a bitmap onto the OLED screen
 * 
 * 
 * \param x -The x position to draw at
 * \param y -The y position to draw at
 * \param picture -const pointer for the picture to draw
 * \param width   -The width of the bitmap
 * \param height  -The height of the bitmap
 * \param color   -The color of the bitmap
 * \return uint8_t The result of drawing onto the screen
 */
uint8_t OLED_SSD1306::DrawBitmap(uint8_t x, uint8_t y, const uint8_t *picture, uint8_t width, uint8_t height, uint8_t color) {
	uint8_t result,i,j, byteWidth = (width+7)/8;
	for (j = 0; j < height; j++) {
		for(i=0; i < width;i++){
			if(pgm_read_byte(picture + j * byteWidth + i / 8) & (128 >> (i & 7)))
			{
				result = DrawPixel(x+i, y+j, color);
			} 
			else 
			{
				result = DrawPixel(x+i, y+j, !color);
			}
		}
	}
	return result;
}



/**
 * \brief Updates the OLED screen display
 * 
 * 
 */
void OLED_SSD1306::Update() {
	GoToPixelPosition(0,0);
	SendData(&displayBuffer[0][0], screenWidth*screenHeight/8);
}



/**
 * \brief Clears the OLED display buffer
 * 
 * 
 */
void OLED_SSD1306::ClearBuffer() {
	for (uint8_t i = 0; i < screenHeight/8; i++){
		memset(displayBuffer[i], 0x00, sizeof(displayBuffer[i]));
	}
}



/**
 * \brief Checks the status of the display buffer
 * 
 * 
 * \param x -The x position to check
 * \param y -The y position to check
 * \return uint8_t The status of the display buffer
 */
uint8_t OLED_SSD1306::CheckBuffer(uint8_t x, uint8_t y) {
	if( x > screenWidth-1 || y > (screenHeight-1)) return 0; // out of Display
	return displayBuffer[(y / (screenHeight/8))][x] & (1 << (y % (screenHeight/8)));
}



/**
 * \brief Displays a block onto the oled screen
 * 
 * 
 * \param x 	-The x position to Draw to
 * \param y 	-The y position to draw to
 * \param width -The width of the block
 */
void OLED_SSD1306::DisplayBlock(uint8_t x, uint8_t y, uint8_t width) {
	if (y > (screenHeight/8-1) || x > screenWidth - 1)
	{
		return;
	}

	if (x + width > screenWidth) { // no -1 here, x alone is width 1
		width = screenWidth - x;
	}
	GoToPixelPosition(x,y);
	SendData(&displayBuffer[y][x], width);
}


//#pragma endregion



#endif


#endif






