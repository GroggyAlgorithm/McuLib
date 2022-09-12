/**
 * \file ckeypadMatrix.c
 * \author Tim Robbins
 * \brief source file for keypad interfacing
 */
#include "ckeypadMatrix.h"

#if !defined(__CKEYPADMATRIX_C__) && defined(__INCLUDED_CKEYPADMATRIX__)
#define __CKEYPADMATRIX_C__	1


/**
* \brief Gets a keypad value from the 2D array passed
* \return The key pressed, if any from the keypad array
*/
uint8_t kp_Scan(unsigned char uchrColumnPinPositions[KP_COLUMNS],
unsigned char uchrRowPinPositions[KP_ROWS], unsigned char uchrKeypadValues[KP_ROWS][KP_COLUMNS]) {
	
	//Variables
	uint8_t i = 0; //column index
	uint8_t j = 0; //Subloop row index
	uint8_t currentColumnPress = 0; //The currently pressed column value
	uint8_t currentRowPress = 0; //The currently pressed row value
	uint8_t pressedValue = '\0'; //The return value for the entered value

	//Set ports initial values
	writeMaskInput(KP_COLUMN_DIR, KP_COLUMN_PIN_MSK); //Column pins as input
	writeMaskOutput(KP_ROW_DIR, KP_ROW_PIN_MSK); //Row pins as output

	//Setup the values for the row and column pins
	KP_COLUMN_PORT |= (KP_COLUMN_PIN_MSK);
	KP_ROW_PORT &= ~(KP_ROW_PIN_MSK);
	
	
	//Delay very slightly to allow changes to take. not using nop,
	//It has been causing some strange things to happen at random times? Why?
	delayForMicroseconds(1);
	
	//Loop through and check for columns
	for(i = 0; i < 3; i++)
	{
		currentColumnPress = readBit(KP_COLUMN_READ, uchrColumnPinPositions[i]);
		
		//If there is a logic low level at the current column...
		if(!currentColumnPress) {
			
			//Invert the pins and check the rows
			writeMaskOutput(KP_COLUMN_DIR, KP_COLUMN_PIN_MSK); //Column pins as input
			writeMaskInput(KP_ROW_DIR, KP_ROW_PIN_MSK); //Row pins as output
			KP_COLUMN_PORT &= ~(KP_COLUMN_PIN_MSK);
			KP_ROW_PORT |= (KP_ROW_PIN_MSK);
			
			//Delay very slightly to allow changes to take. not using nop,
			//It has been causing some strange things to happen at random times? Why?
			delayForMicroseconds(1);
			
			
			for(j = 0; j < 4; j++) {
				currentRowPress = readBit(KP_ROW_READ, uchrRowPinPositions[j]);
				
				if(!currentRowPress) {
					pressedValue = uchrKeypadValues[j][i];
					i=3;
					break;
				}
				
			}
			
			if(i < 3) {
				
				//Set ports initial values
				writeMaskInput(KP_COLUMN_DIR, KP_COLUMN_PIN_MSK); //Column pins as input
				writeMaskOutput(KP_ROW_DIR, KP_ROW_PIN_MSK); //Row pins as output

				//Setup the values for the row and column pins
				KP_COLUMN_PORT |= (KP_COLUMN_PIN_MSK);
				KP_ROW_PORT &= ~(KP_ROW_PIN_MSK);
			}
		}
		
		
	}
	
	
	return pressedValue;
}








#endif