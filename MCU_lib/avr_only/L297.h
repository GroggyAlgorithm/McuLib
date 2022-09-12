/**
 * \file L297.h
 * \author Tim Robbins
 * \brief Header file for the L297 stepper controller and L298 motor driver
 */


#ifndef __L297_H__
#define __L297_H__

#ifdef __cplusplus
extern "C" {
#endif
	
#include <avr/io.h>
#include <stdbool.h>
#include "config.h"
#include "mcuUtils.h"
#include "mcuPinUtils.h"
#include "mcuAdc.h"




#ifndef L297_HOME_READ
	//#warning L297.h: L297_HOME_READ not defined, will be unable to read from home pin
	#define L297_HOME_READ	-1
	#define __L297_C__ -1
#endif

#ifndef L297_CONTROL_PIN
	//#warning L297.h: L297_CONTROL_PIN not defined, will be unable to set chopper selections
	#define L297_CONTROL_PIN -1
	#define __L297_C__ -1
#endif

#ifndef L297_DIR_SELECT
	//#warning L297.h: L297_DIR_SELECT not defined, will be unable to set direction
	#define L297_DIR_SELECT -1
	#define __L297_C__ -1
#endif

#ifndef L297_STEP_SELECT
	//#warning L297.h: L297_STEP_SELECT not defined, will be unable to set step type
	#define L297_STEP_SELECT -1
	#define __L297_C__ -1

#endif


#ifndef L297_CLOCK
	//#warning L297.h: L297_CLOCK not defined.
	#define __L297_C__ -1
#endif

#ifndef L297_RESET
	//#warning L297.h: L297_RESET not defined
	#define __L297_C__ -1
#endif

#ifndef L297_ENABLE
	//#warning L297.h: L297_ENABLE not defined.
	#define __L297_C__ -1
#endif

#ifndef __L297_C__

typedef struct L297_STEPPERS {
	
	///The position of the pin to enable the motor
	uint8_t enablePinPosition;
	
	///The position of the reset pin
	uint8_t resetPinPosition;
	
	///The position of the clock pin
	uint8_t clockPinPosition;
	
	///The position of the home read pin
	uint8_t homeReadPinPosition;
	
	///The position of the direction pin
	uint8_t directionPinPosition;
	
	///The position of the step type selection pin
	uint8_t stepSelectPinPosition;
	
	///Boolean for the stepper step direction
	uint8_t direction;
	
	///Boolean for if the motor should be half stepping
	uint8_t useHalfSteps;
	
	///Timing delay for motor toggle full steps
	uint16_t fullStep_DelayTiming;
	
	///Timing delay for motor toggle half steps
	uint16_t halfStep_DelayTiming;
	
	///ADC channel for current sensing 1
	adc_channel_t sense_1_channel;
	
	///ADC channels for current sensing 2
	adc_channel_t sense_2_channel;
	
	///Low end of where the current sensing can be
	unsigned short rangeCutout_Low;
	
	///High end of where the current sensing can be
	unsigned short rangeCutout_High;
	
	
	
	
} L297_t;

void L297_Reset();
void L297_Step(unsigned short waitTime_Micro);
void L297_SetStepPattern(uint8_t useHalfStep);
void L297_SetDirection(uint8_t direction);
uint8_t L297_CheckForStall(adc_channel_t sense_1_channel, adc_channel_t sense_2_channel,unsigned short cutout_range_low, unsigned short cutout_range_high);


#else

#warning L297.h: Definitions missing. View required definitions to include.


#endif



#ifdef __cplusplus
}
#endif



#endif /* L297_H_ */