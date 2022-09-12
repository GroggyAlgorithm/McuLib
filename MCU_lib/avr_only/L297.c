/**
 * \file L297.c
 * \author Tim Robbins
 * \brief Source file for the L297 stepper controller and L298 motor driver
 */


#include "L297.h"
#ifndef __L297_C__
#define __L297_C__


/**
* \brief Toggles the l297 reset
*
*/
void L297_Reset() {
	PIN_LOW(L297_RESET);
	PIN_HIGH(L297_ENABLE);
	PIN_HIGH(L297_RESET);
	PIN_LOW(L297_ENABLE);
}


/**
* \brief Has the L297 take a step
*
*/
void L297_Step(unsigned short waitTime_Micro) {
	PIN_TOGGLE(L297_CLOCK);
	DelayForMicroseconds(waitTime_Micro);
	PIN_TOGGLE(L297_CLOCK);
}


//#if L297_STEP_SELECT != -1

/**
* \brief Selects the step pattern for the L297
*
*/
void L297_SetStepPattern(uint8_t useHalfStep) {
	PIN_WRITE(L297_STEP_SELECT,useHalfStep);
}

//#endif


/**
* \brief Selects the direction for the L297
*
*/
void L297_SetDirection(uint8_t direction) {
	PIN_WRITE(L297_DIR_SELECT,direction);
	L297_Reset();
}



/**
* \brief Checks for a stall from the L297
*
*/
uint8_t L297_CheckForStall(adc_channel_t sense_1_channel, adc_channel_t sense_2_channel, unsigned short cutout_range_low, unsigned short cutout_range_high) {
	
	uint8_t hasStalled = 0; //If a stall is detected
	volatile unsigned short senseAdc_1 = 0; //Adc value from sense 1
	volatile unsigned short senseAdc_2 = 0; //Adc value from sense 2
	
	
	senseAdc_1 = ADC_Get(sense_1_channel);
	
	if(senseAdc_1 > cutout_range_high || senseAdc_1 < cutout_range_low)
	{
		hasStalled = 1;
	}
	else
	{
		senseAdc_2 = ADC_Get(sense_2_channel);
		if(senseAdc_2 > cutout_range_high || senseAdc_2 < cutout_range_low)
		{
			hasStalled = 1;
		}
		
	}
	
	
	return hasStalled;
}



#endif
