/**
 * \file clcd.c
 * \author Tim Robbins
 * \brief C Source file for LCD
 */
#include "clcd.h"

#if !defined(CLCD_C) && defined(__INCLUDED_CLCD__)
#define CLCD_C 1



///The first positions in each row, in order from top to bottom
unsigned char m_auchrLineStartValues[LCD_ROW_COUNT];

///The line we're currently at on the LCD
unsigned char m_uchrCurrentLine = 0;

///The column we're currently at on the LCD
unsigned char m_uchrCurrentColumn = 0;


/**
* \brief Lcd initializer function. Runs through the startup sequence passed and copies the line start positions passed.
* \param startupSequence The sequence to run through the startup. Should be the initialization sequence in the datasheets followed by any additional commands.
* \param lineStartPositions The position at the beginning or each line on the display. Must be ordered from top to bottom
*/
void LcdInit(unsigned char startupSequence[], unsigned char lineStartPositions[])
{
	
	//Variables
	unsigned char index = 0; //Index for looping through the passed arrays.
    unsigned char currentByte = 0; //The current byte we're on in the parameters.
	
	LCD_CONTROL_PORT &= ~(1 << LCD_E_PIN | 1 << LCD_RS_PIN | 1 << LCD_RW_PIN);
	
	//Set the data pins to output
	writeMaskOutput(LCD_DATA_PORT_DIR, 0xFF);
	

	//Loop through the initialization values and send as commands
	for(index = 0; startupSequence[index] != '\0'; index++) {
        currentByte = startupSequence[index];
		LcdSendCommand(currentByte);
		delayForMilliseconds(10);
	}
	
	//Copy the line start positions
	for(index = 0; index < LCD_ROW_COUNT; index++) {
	 	currentByte = lineStartPositions[index];
	 	m_auchrLineStartValues[index] = currentByte;
	 }

	delayForMicroseconds(10);
}



/**
 * \brief Returns the current line the LCD is on
 * 
 * 
 * \return unsigned char The current line
 */
unsigned char LcdGetCurrentLine() {
	return m_uchrCurrentLine;
}



/**
 * \brief Returns the current column the LCD is on
 * 
 * 
 * \return unsigned char The current column
 */
unsigned char LcdGetCurrentColumn() {
	return m_uchrCurrentColumn;
}



/**
 * \brief Sends a command to the LCD
 * 
 * 
 * \param cmd The command to send
 */
void LcdSendCommand(unsigned char cmd) {
	LCD_CONTROL_PORT &= ~(1 << LCD_RS_PIN | 1 << LCD_RW_PIN);
	LCD_CONTROL_PORT |= (1 << LCD_E_PIN);
	LCD_DATA_PORT &= ~(0xFF);
	LCD_DATA_PORT |= cmd;
	LCD_CONTROL_PORT |= (1 << LCD_E_PIN);
	delayForMicroseconds(1);
	LCD_CONTROL_PORT &= ~(1 << LCD_E_PIN);
	delayForMilliseconds(1);
}



/**
* \brief Sends a data value to the LCD
* \param data The data to send
*/
void LcdSendData(unsigned char data) {
	LCD_CONTROL_PORT &= ~(1 << LCD_RS_PIN | 1 << LCD_RW_PIN);
	LCD_CONTROL_PORT |= (1 << LCD_E_PIN);
	LCD_DATA_PORT &= ~(0xFF);
	LCD_CONTROL_PORT |= (1 << LCD_RS_PIN);
	LCD_DATA_PORT |= data;
	delayForMicroseconds(1);
	LCD_CONTROL_PORT &= ~(1 << LCD_E_PIN);
	delayForMilliseconds(1);
}



/**
* \brief Waits for the LCD's busy flag to trigger
*
*/
void LcdBusyFlagWait() {



	//Toggle EPin to load any remaining data
	LCD_CONTROL_PORT |= (1 << LCD_E_PIN);
	delayForMicroseconds(1);
	LCD_CONTROL_PORT &= ~(1 << LCD_E_PIN);

	delayForMicroseconds(1);
	
	//Set RW pin for READ mode
	LCD_CONTROL_PORT &= ~(1 << LCD_RS_PIN);
	LCD_CONTROL_PORT |= ( 1 << LCD_RW_PIN);

	delayForMilliseconds(20);
	
	//Set the busy flag pin to input low
	LCD_DATA_PORT |= (1 << LCD_BUSY_FLAG_POSITION);
	
	setBitInput(LCD_DATA_PORT_DIR, LCD_BUSY_FLAG_POSITION);
	
	//Toggle the E pin while the busy flag is high
	do
	{
		LCD_CONTROL_PORT |= (1 << LCD_E_PIN);
		delayForMicroseconds(1);
		LCD_CONTROL_PORT &= ~(1 << LCD_E_PIN);
	} while (readBit(LCD_DATA_PORT_READ,LCD_BUSY_FLAG_POSITION));
	
	
	//Reset
	LCD_CONTROL_PORT &= ~(1 << LCD_RS_PIN | 1 << LCD_RW_PIN | 1 << LCD_E_PIN);
	setBitOutput(LCD_DATA_PORT_DIR, LCD_BUSY_FLAG_POSITION);
	LCD_DATA_PORT = 0x00;
}



/**
* \brief Turns the LCD on and optionally sets the cursor on and sets the cursors blink blinking
* \param cursorOn If the cursor is on or not
* \param cursorBlink If the cursor is on, sets whether or not the cursor is a blinking cursor
*/
void LcdDisplayOn(bool cursorOn, bool cursorBlink)
{
	if(cursorOn) {
		if(cursorBlink) LcdSendCommand(LCD_DISPLAY_ON_CURSOR_BLINK);
		else LcdSendCommand(LCD_DISPLAY_ON_NO_BLINK);
	}
	else {
		LcdSendCommand(LCD_DISPLAY_ON_NO_CURSOR);
	}
}



/**
* \brief Clears the line passed
* \param uchrLine The line to clear
*/
void LcdClearLine(unsigned char uchrLine) {
	if(uchrLine < LCD_ROW_COUNT) {
		unsigned char savedColumn = m_uchrCurrentColumn;
		unsigned char savedRow = m_uchrCurrentLine;
		LcdGoToPosition(uchrLine, 0);
		for(unsigned char i = 0; i < LCD_COLUMN_COUNT-1; i++) {
			LcdPrintChar(' ');
		}
		LcdGoToPosition(savedRow, savedColumn);
	}
}



/**
* \brief Clears the line passed starting at the column passed
* \param uchrLine The line to clear
* \param uchrColumn The column to begin clearing at
*/
void LcdClearLineFromColumn(unsigned char uchrLine, unsigned char uchrColumn) {
	if(uchrLine < LCD_ROW_COUNT && uchrColumn < LCD_COLUMN_COUNT - 1) {
		unsigned char savedColumn = m_uchrCurrentColumn;
		unsigned char savedRow = m_uchrCurrentLine;
		LcdGoToPosition(uchrLine, uchrColumn);
		for(unsigned char i = uchrColumn; i < LCD_COLUMN_COUNT-1; i++) {
			LcdPrintChar(' ');
		}
		LcdGoToPosition(savedRow, savedColumn);
	}
}



/**
* \brief Clears the line passed starting at the start column passed and ending at the ending column passed
* \param uchrLine The line to clear
* \param uchrStartColumn The column to begin clearing at
* \param uchrEndColumn The column to stop clearing at
*/
void LcdClearLineAtColumns(unsigned char uchrLine, unsigned char uchrStartColumn, unsigned char uchrEndColumn) 
{
    
    
	if(uchrLine < LCD_ROW_COUNT && (uchrStartColumn < LCD_COLUMN_COUNT - 1 || uchrEndColumn < LCD_COLUMN_COUNT - 1)) {
		unsigned char savedColumn = m_uchrCurrentColumn;
		unsigned char savedRow = m_uchrCurrentLine;
		LcdGoToPosition(uchrLine, uchrStartColumn);
		for(unsigned char i = uchrStartColumn; i < LCD_COLUMN_COUNT-1 && i < uchrEndColumn; i++) {
			LcdPrintChar(' ');
		}
		LcdGoToPosition(savedRow, savedColumn);
	}
}



/**
* \brief Clears the position passed
* \param uchrLine The line to clear
* \param uchrColumn The column position to clear
*/
void LcdClearPosition(unsigned char uchrLine, unsigned char uchrColumn) {
	if(uchrLine < LCD_ROW_COUNT && uchrColumn < LCD_COLUMN_COUNT - 1) {
		unsigned char savedColumn = m_uchrCurrentColumn;
		unsigned char savedRow = m_uchrCurrentLine;
		LcdGoToPosition(uchrLine, uchrColumn);
		LcdPrintChar(' ');
		LcdGoToPosition(savedRow, savedColumn);
	}
}



/**
* \brief Creates and stores the customer character dot line at the address passed
* \param uchrAddress The address to store the character at
* \param uchrNewDot The character "dots" to save
*/
void LcdStoreDotRow(unsigned char uchrAddress, unsigned char uchrNewDot) {
	LCD_set_cg_ram(uchrAddress);
	LcdSendData(uchrNewDot);
	LcdBusyFlagWait();
}



/**
* \brief Creates and stores the custom character at the address passed
* \param uchrAddress The address to store the character at
* \param auchrCharacter Array of the characters "dots" to save
*/
void LcdStoreCustomCharacter(unsigned char uchrAddress, unsigned char auchrCharacter[8]) {
	for(unsigned char i = 0; i < 8; i++) {
		LCD_set_cg_ram(uchrAddress + i);
		LcdSendData(auchrCharacter[i]);
		LcdBusyFlagWait();
	}
}



/**
* \brief Reads and returns the address counter from the LCD
*
*/
unsigned char LcdGetAddressCounter() {
	
	unsigned char address = 0;

	//Toggle EPin to load any remaining data
	LCD_CONTROL_PORT |= (1 << LCD_E_PIN);
	delayForMicroseconds(1);
	LCD_CONTROL_PORT &= ~(1 << LCD_E_PIN);

	delayForMicroseconds(1);
	
	//Set RW pin for READ mode
	LCD_CONTROL_PORT &= ~(1 << LCD_RS_PIN);
	LCD_CONTROL_PORT |= ( 1 << LCD_RW_PIN);

	delayForMilliseconds(20);
	
	//Set the busy flag pin to input low
	LCD_DATA_PORT |= (1 << LCD_BUSY_FLAG_POSITION);
	setBitInput(LCD_DATA_PORT_DIR, LCD_BUSY_FLAG_POSITION);
	
	//Toggle the E pin while the busy flag is high
	do
	{
		LCD_CONTROL_PORT |= (1 << LCD_E_PIN);
		delayForMicroseconds(1);
		LCD_CONTROL_PORT &= ~(1 << LCD_E_PIN);
	} while (readBit(LCD_DATA_PORT_READ,LCD_BUSY_FLAG_POSITION));
	
	address = readBit(LCD_DATA_PORT_READ,LCD_BUSY_FLAG_POSITION);
	address &= ~(1 << LCD_BUSY_FLAG_POSITION);

	//Reset
	LCD_CONTROL_PORT &= ~(1 << LCD_RS_PIN | 1 << LCD_RW_PIN | 1 << LCD_E_PIN);
	setBitOutput(LCD_DATA_PORT_DIR, LCD_BUSY_FLAG_POSITION);
	LCD_DATA_PORT = 0x00;

	return address;
}



/**
* \brief Goes to the position on th LCD passed
* \param uchrLine The line to go to
* \param uchrColumn The column to go to
*/
void LcdGoToPosition(unsigned char uchrLine, unsigned char uchrColumn) {
	if(uchrLine > LCD_ROW_COUNT -1) uchrLine = LCD_ROW_COUNT - 1;
	if(uchrColumn > LCD_COLUMN_COUNT-1) uchrColumn = LCD_COLUMN_COUNT - 1;
	m_uchrCurrentColumn = uchrColumn;
	m_uchrCurrentLine = uchrLine;
	unsigned char uchrNewAddress = ((m_auchrLineStartValues[m_uchrCurrentLine]) + m_uchrCurrentColumn);
	LCD_set_dd_ram(uchrNewAddress);
}



/**
* \brief Performs the action associated with the command passed. Example: LCD_RETURN_HOME will run the return home command.
* \param cmd The command to perform
*/
void LcdPerformCommand(unsigned char cmd) {
	 switch (cmd)
    {
    case LCD_RETURN_HOME://Return home
        m_uchrCurrentColumn = 0;
        m_uchrCurrentLine = 0;
        LcdSendCommand(LCD_RETURN_HOME);
        break;

    case LCD_CR://Carriage return
        m_uchrCurrentColumn = 0;
		LcdGoToPosition(m_uchrCurrentLine, m_uchrCurrentColumn);	
        break;

    
	case LCD_CLEAR_SCREEN:
        m_uchrCurrentColumn = 0;
        m_uchrCurrentLine = 0;
        LcdSendCommand(LCD_CLEAR_SCREEN);
        break;
		
	case LCD_FF:
        m_uchrCurrentColumn = 0;
        m_uchrCurrentLine = 0;
        LcdSendCommand(LCD_CLEAR_SCREEN);
        break;
		
    case LCD_LF: //Line feed \n
		m_uchrCurrentColumn = 0;
        if(m_uchrCurrentLine < LCD_ROW_COUNT-1) {
			m_uchrCurrentLine += 1;
		}
		else {
			m_uchrCurrentLine = 0;
			LcdSendCommand (LCD_CLEAR_SCREEN);
		}
		
		LcdGoToPosition(m_uchrCurrentLine, m_uchrCurrentColumn);
        break;
		
	case LCD_TAB:
		LcdPrintString("    ");
		break;
	
    case LCD_TOP: //Top
        m_uchrCurrentLine = 0;
        LcdGoToPosition(m_uchrCurrentLine, m_uchrCurrentColumn);
        break;

	case LCD_BOTTOM:
		m_uchrCurrentLine = LCD_ROW_COUNT - 1;
        LcdGoToPosition(m_uchrCurrentLine, m_uchrCurrentColumn);
		break;

    case LCD_UP: //Up
        if(m_uchrCurrentLine > 0) {
			m_uchrCurrentLine -= 1;
			LcdGoToPosition(m_uchrCurrentLine, m_uchrCurrentColumn);
		}
        break;

    case LCD_DN: //Down
	
        if(m_uchrCurrentLine < LCD_ROW_COUNT-1) {
	        m_uchrCurrentLine += 1;
	        LcdGoToPosition(m_uchrCurrentLine, m_uchrCurrentColumn);
        }
        break;

    case LCD_BS: //Backspace
		
		if(m_uchrCurrentColumn > 0) {
			m_uchrCurrentColumn -= 1;
			LcdSendCommand(LCD_CURSOR_SHIFT_L);
			LcdPrintChar(' ');
			m_uchrCurrentColumn -= 1;
			LcdSendCommand(LCD_CURSOR_SHIFT_L);
		}
		else if(m_uchrCurrentLine > 0) {
			m_uchrCurrentColumn = LCD_COLUMN_COUNT - 1;
			m_uchrCurrentLine -= 1;
			LcdGoToPosition(m_uchrCurrentLine, m_uchrCurrentColumn);
			LcdPrintChar(' ');
			m_uchrCurrentColumn = LCD_COLUMN_COUNT - 1;
			m_uchrCurrentLine -= 1;
			LcdSendCommand (LCD_CURSOR_SHIFT_L);
		}
		
        break;

    case LCD_RT: //Right
		if(m_uchrCurrentColumn < LCD_COLUMN_COUNT - 1) {
			m_uchrCurrentColumn++;
			LcdSendCommand (LCD_CURSOR_SHIFT_R);
		}
        break;

    case LCD_LFT: //Left
        if(m_uchrCurrentColumn > 0) {
	        m_uchrCurrentColumn--;
			LcdSendCommand (LCD_CURSOR_SHIFT_L);
        }
        break;
		
    default:
        break;
    }
}



/**
* \brief Sends a byte as a command if under 0x20(' ') or as a data if over
* \param uchrByte The data byte to send
*/
void LcdSendByte(unsigned char uchrByte) {
	if(uchrByte < 0x20) {
        LcdPerformCommand(uchrByte);
    }
    else {
		LcdPrintChar(uchrByte);
    }
}



/**
* \brief Sends a byte as a command if under 0x20(' ') or as a data if over and delays for the amount of microseconds passed afterwards
* \param uchrByte The data byte to send
* \param ushtDelayTime The amount of microseconds to delay for
*/
void LcdSendByteDelay(unsigned char uchrByte, unsigned short ushtDelayTime) {
	if(uchrByte < 0x20) {
        LcdPerformCommand(uchrByte);
    }
    else {
		LcdPrintChar(uchrByte);
    }
	
	delayForMicroseconds(ushtDelayTime);
}



/**
* \brief Sends a byte as a command if under 0x20(' ') or as a data if over at the position passed
* \param uchrByte The data byte to send
* \param uchrRow The row for the byte to display at
* \param uchrColumn The column for the byte to display at
*/
void LcdSendByteAtPosition(unsigned char uchrByte, unsigned char uchrRow, unsigned char uchrColumn) {
	
	LcdGoToPosition(uchrRow, uchrColumn);

	if(uchrByte < 0x20) {
        LcdPerformCommand(uchrByte);
    }
    else {
		LcdPrintChar(uchrByte);
    }
}



/**
* \brief Sends a byte as a command if under 0x20(' ') or as a data if over at the positions passed and delays for the amount of microseconds passed
* \param uchrByte The data byte to send
* \param uchrRow The row for the byte to display at
* \param uchrColumn The column for the byte to display at
* \param ushtDelayTime The amount of microseconds to delay for
*/
void LcdSendByteDelayAtPosition(unsigned char uchrByte, unsigned char uchrRow, unsigned char uchrColumn, unsigned short ushtDelayTime) {
	
	LcdGoToPosition(uchrRow, uchrColumn);
	
	if(uchrByte < 0x20) {
        LcdPerformCommand(uchrByte);
    }
    else {
		LcdPrintChar(uchrByte);
    }
	
	delayForMicroseconds(ushtDelayTime);
}



/**
* \brief Prints the passed character verbatim onto the LCD display
* \param uchrChar The character to print
*/
void LcdPrintChar(char uchrChar) {
	
	if(m_uchrCurrentColumn > LCD_COLUMN_COUNT-1) {
		if(m_uchrCurrentLine > LCD_ROW_COUNT-1) {
			LcdSendCommand(LCD_CLEAR_SCREEN);
			m_uchrCurrentColumn = 0;
			m_uchrCurrentLine = 0;
		}
		else {
			m_uchrCurrentColumn = 0;
			m_uchrCurrentLine += 1;
		}
	}
	
	LcdGoToPosition(m_uchrCurrentLine, m_uchrCurrentColumn);
	
	LcdSendData(uchrChar);
    
    m_uchrCurrentColumn+=1;
}



/**
* \brief Prints the passed character verbatim onto the LCD display and delays for the amount of microseconds passed
* \param uchrChar The character to print
* \param ushtDelayTime The amount of microseconds to delay for
*/
void LcdPrintCharDelay(char uchrChar, unsigned short ushtDelayTime) {
	
	if(m_uchrCurrentColumn > LCD_COLUMN_COUNT-1) {
		if(m_uchrCurrentLine > LCD_ROW_COUNT-1) {
			LcdSendCommand(LCD_CLEAR_SCREEN);
			m_uchrCurrentColumn = 0;
			m_uchrCurrentLine = 0;
		}
		else {
			m_uchrCurrentColumn = 0;
			m_uchrCurrentLine += 1;
		}
	}
	
	LcdGoToPosition(m_uchrCurrentLine, m_uchrCurrentColumn);
    
	
	
	LcdSendData(uchrChar);
	delayForMicroseconds(ushtDelayTime);
    
    m_uchrCurrentColumn+=1;
	
}



/**
* \brief Prints the passed character verbatim onto the LCD display at the position passed
* \param uchrChar The character to print
* \param uchrRow The row for the character to display at
* \param uchrColumn The column for the character to display at
*/
void LcdPrintCharAtPosition(char uchrChar, unsigned char uchrRow, unsigned char uchrColumn) {
	if(uchrColumn > LCD_COLUMN_COUNT-1) {
		if(uchrRow > LCD_ROW_COUNT-1) {
			LcdSendCommand(LCD_CLEAR_SCREEN);
			m_uchrCurrentColumn = 0;
			m_uchrCurrentLine = 0;
		}
		else {
			m_uchrCurrentColumn = 0;
			m_uchrCurrentLine += 1;
		}
	}

    
    LcdGoToPosition(uchrRow, uchrColumn);
	
	LcdSendData(uchrChar);
    
    m_uchrCurrentColumn+=1;
}



/**
* \brief Prints the passed character verbatim onto the LCD display at the position passed and delays for the amount of microseconds passed
* \param uchrChar The character to print
* \param uchrRow The row for the character to display at
* \param uchrColumn The column for the character to display at
* \param ushtDelayTime The amount of microseconds to delay for
*/
void LcdPrintCharDelayAtPosition(char uchrChar, unsigned char uchrRow, unsigned char uchrColumn, unsigned short ushtDelayTime) {
	if(uchrColumn > LCD_COLUMN_COUNT-1) {
		if(uchrRow > LCD_ROW_COUNT-1) {
			LcdSendCommand(LCD_CLEAR_SCREEN);
			m_uchrCurrentColumn = 0;
			m_uchrCurrentLine = 0;
		}
		else {
			m_uchrCurrentColumn = 0;
			m_uchrCurrentLine += 1;
		}
	}
	LcdGoToPosition(uchrRow, uchrColumn);
	
	LcdSendData(uchrChar);
    
    m_uchrCurrentColumn+=1;
    
	delayForMicroseconds(ushtDelayTime);
}



/**
* \brief Prints a string verbatim onto the display
* \param strToSend The string to send
*/
void LcdPrintString(char* strToSend) {
	unsigned char currentByte = 0;

	while(*strToSend)
	{	
		currentByte = (*strToSend++);
        
        
		LcdPrintChar(currentByte);
	}
}



/**
* \brief Prints a string verbatim onto the display and delays for the amount of microseconds passed
* \param strToSend The string to send
* \param ushtDelayTime The amount of microseconds to delay for in between each char
*/
void LcdPrintStringDelay(char* strToSend, unsigned short ushtDelayTime) {
	unsigned char currentByte = 0;

	while(*strToSend)
	{
		currentByte = (*strToSend++);
		LcdPrintCharDelay(currentByte, ushtDelayTime);
	}
}



/**
* \brief Prints a string verbatim onto the display at the position passed
* \param strToSend The string to send
* \param uchrRow The row to print at
* 
*/
void LcdPrintStringAtPosition(char* strToSend, unsigned char uchrRow, unsigned char uchrColumn) {
	unsigned char currentByte = 0;

	LcdGoToPosition(uchrRow, uchrColumn);

	while(*strToSend)
	{
		currentByte = (*strToSend++);
		LcdPrintChar(currentByte);
	}
}



/**
* \brief Prints a string verbatim onto the display at the position passed and delays for the amount of microseconds passed
* \param strToSend The string to send
* \param strToSend The string to send
* \param uchrRow The row to print at
* \param ushtDelayTime The amount of microseconds to delay for in between each char
*/
void LcdPrintStringDelayAtPosition(char* strToSend, unsigned char uchrRow, unsigned char uchrColumn, unsigned short ushtDelayTime) {
	unsigned char currentByte = 0;

	LcdGoToPosition(uchrRow, uchrColumn);

	while(*strToSend)
	{
		currentByte = (*strToSend++);
		LcdPrintCharDelay(currentByte, ushtDelayTime);
	}
}



/**
* \brief Sends a string to the LCD, performs any command characters passed and displays the printable characters passed
* \param strToSend The string to send
*/
void LcdPrint(char* strToSend) {
	unsigned char currentByte = 0;
	while(*strToSend)
	{
		currentByte = (*strToSend++);
		LcdSendByte(currentByte);
	}
}



/**
* \brief Sends a string to the LCD, performs any command characters passed and displays the printable characters passed and delays for the amount of microseconds passed
* \param strToSend The string to send
* \param ushtDelayTime The amount of microseconds to delay for in between each char
*/
void LcdPrintDelay(char* strToSend, unsigned short ushtDelayTime) {
	unsigned char currentByte = 0;

	while(*strToSend)
	{
		currentByte = (*strToSend++);
		LcdSendByteDelay(currentByte, ushtDelayTime);
	}
}



/**
* \brief Sends a string to the LCD, performs any command characters passed and displays the printable characters passed at the position passed
* \param strToSend The string to send
* \param uchrRow The row to print at
* \param uchrColumn The column to print at
*/
void LcdPrintAtPosition(char* strToSend, unsigned char uchrRow, unsigned char uchrColumn) {
	unsigned char currentByte = 0;
	
	LcdGoToPosition(uchrRow, uchrColumn);

	while(*strToSend)
	{
		currentByte = (*strToSend++);
		LcdSendByte(currentByte);
	}
}



/**
* \brief Sends a string to the LCD, performs any command characters passed and displays the printable characters passed at the position passed and delays by the amount of microseconds passed.
* \param strToSend The string to send
* \param uchrRow The row to print at
* \param uchrColumn The column to print at
* \param ushtDelayTime The amount of microseconds to delay for in between each char
*/
void LcdPrintDelayAtPosition(char* strToSend, unsigned char uchrRow, unsigned char uchrColumn, unsigned short ushtDelayTime) {
	unsigned char currentByte = 0;
	LcdGoToPosition(uchrRow, uchrColumn);

	
	while(*strToSend)
	{
		currentByte = (*strToSend++);
		LcdSendByteDelay(currentByte, ushtDelayTime);
	}
}





#endif