/**
 * \file   steppers.c
 * \author Tim Robbins - R&D Engineer, Atech Training
 * \brief Source file for stepper motor functions
 * \version v1.5
 */

#include "steppers.h"

#ifndef STEPPERS_C
#define	STEPPERS_C 1



/**
 * @brief Gets the value for steppers wave step. \n
 * Example use: \n
 * uchrStepperPortValue = STEPPER_PORT_READ_REGISTER; \n
 * StepperGetFullStep(&uchrStepperPortValue, auchrPinPositions, &uchrStepperIndex, blnIsCounterClockwise); \n
 * STEPPER_PORT_OUT_REGISTER = uchrStepperPortValue; \n
 * @author Tim Robbins
 * @param stepperPhasePortValue The port value the stepper motors phase lines are connected to
 * @param phaseLinePinPositions The positions of the phase line pins
 * @param stepSequenceIndex The index for the current step position
 * @param isCounterClockwise If taking a counter clockwise step
 */
void StepperWaveStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise)
{
    //Variables
    uint8_t stepSequenceIndexCopy = *stepSequenceIndex; //Copy of the step sequence index
    uint8_t sequenceValue = *stepperPhasePortValue; //Copy of the passed value that's on the phase line port
    
    //Array of the step sequencies, for easy setting
    uint8_t stepSequence[4] = 
    {    
        ((uint8_t)(1 << phaseLinePinPositions[0]) | (uint8_t)(0 << phaseLinePinPositions[1]) | (uint8_t)(0 << phaseLinePinPositions[2]) | (uint8_t)(0 << phaseLinePinPositions[3])),
        ((uint8_t)(0 << phaseLinePinPositions[0]) | (uint8_t)(1 << phaseLinePinPositions[1]) | (uint8_t)(0 << phaseLinePinPositions[2]) | (uint8_t)(0 << phaseLinePinPositions[3])),
        ((uint8_t)(0 << phaseLinePinPositions[0]) | (uint8_t)(0 << phaseLinePinPositions[1]) | (uint8_t)(1 << phaseLinePinPositions[2]) | (uint8_t)(0 << phaseLinePinPositions[3])),
        ((uint8_t)(0 << phaseLinePinPositions[0]) | (uint8_t)(0 << phaseLinePinPositions[1]) | (uint8_t)(0 << phaseLinePinPositions[2]) | (uint8_t)(1 << phaseLinePinPositions[3]))

    };
    
    //Range check
    if(stepSequenceIndexCopy > 3)
    {
        stepSequenceIndexCopy = (isCounterClockwise) ? 3 : 0;
    }
    
    //Clear the pin positions in the sequence value so they can be set without changing the rest of the ports value
	sequenceValue &= ~(1 << phaseLinePinPositions[0] | 1 << phaseLinePinPositions[1] | 1 << phaseLinePinPositions[2] | 1 << phaseLinePinPositions[3]);
    
    //Set the index copy to the appropriate current step
    sequenceValue |= (stepSequence[stepSequenceIndexCopy]);
    
    //If we're at the final step and we're moving clockwise...
    if(stepSequenceIndexCopy >= 3 && isCounterClockwise == false)
    {
        //Set to the first step
        stepSequenceIndexCopy = 0;
    }
    //If we're at the first step and we're moving counter clockwise...
    else if(stepSequenceIndexCopy == 0 && isCounterClockwise == true)
    {
        //Set to the last step
        stepSequenceIndexCopy = 3;
    }
    //Else...
    else
    {
        //Adjust the step index based on direction
        stepSequenceIndexCopy = (isCounterClockwise == true) ? stepSequenceIndexCopy - 1: stepSequenceIndexCopy + 1;
    }
    
    
    
	
    
    //After getting the new sequence value and index value, set the values in the pointer parameters
	*stepperPhasePortValue = sequenceValue;
    *stepSequenceIndex = stepSequenceIndexCopy;
    
}



/**
 * @brief Gets the value for steppers half step. \n
 * Example use: \n
 * uchrStepperPortValue = STEPPER_PORT_READ_REGISTER; \n
 * StepperGetFullStep(&uchrStepperPortValue, auchrPinPositions, &uchrStepperIndex, blnIsCounterClockwise); \n
 * STEPPER_PORT_OUT_REGISTER = uchrStepperPortValue; \n
 * @author Tim Robbins
 * @param stepperPhasePortValue The port value the stepper motors phase lines are connected to
 * @param phaseLinePinPositions The positions of the phase line pins
 * @param stepSequenceIndex The index for the current step position
 * @param isCounterClockwise If taking a counter clockwise step
 */
void StepperHalfStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise)
{
    //Variables
    uint8_t stepSequenceIndexCopy = *stepSequenceIndex; //Copy of the step sequence index
    uint8_t sequenceValue = *stepperPhasePortValue; //Copy of the passed value that's on the phase line port
    
    //Array of the step sequencies, for easy setting
    uint8_t stepSequence[8] = 
    {
        ((uint8_t)(1 << phaseLinePinPositions[0]) | (uint8_t)(0 << phaseLinePinPositions[1]) | (uint8_t)(0 << phaseLinePinPositions[2]) | (uint8_t)(1 << phaseLinePinPositions[3])),
        ((uint8_t)(1 << phaseLinePinPositions[0]) | (uint8_t)(0 << phaseLinePinPositions[1]) | (uint8_t)(0 << phaseLinePinPositions[2]) | (uint8_t)(0 << phaseLinePinPositions[3])),
        ((uint8_t)(1 << phaseLinePinPositions[0]) | (uint8_t)(1 << phaseLinePinPositions[1]) | (uint8_t)(0 << phaseLinePinPositions[2]) | (uint8_t)(0 << phaseLinePinPositions[3])),
        ((uint8_t)(0 << phaseLinePinPositions[0]) | (uint8_t)(1 << phaseLinePinPositions[1]) | (uint8_t)(0 << phaseLinePinPositions[2]) | (uint8_t)(0 << phaseLinePinPositions[3])),
        ((uint8_t)(0 << phaseLinePinPositions[0]) | (uint8_t)(1 << phaseLinePinPositions[1]) | (uint8_t)(1 << phaseLinePinPositions[2]) | (uint8_t)(0 << phaseLinePinPositions[3])),
        ((uint8_t)(0 << phaseLinePinPositions[0]) | (uint8_t)(0 << phaseLinePinPositions[1]) | (uint8_t)(1 << phaseLinePinPositions[2]) | (uint8_t)(0 << phaseLinePinPositions[3])),
        ((uint8_t)(0 << phaseLinePinPositions[0]) | (uint8_t)(0 << phaseLinePinPositions[1]) | (uint8_t)(1 << phaseLinePinPositions[2]) | (uint8_t)(1 << phaseLinePinPositions[3])),
        ((uint8_t)(0 << phaseLinePinPositions[0]) | (uint8_t)(0 << phaseLinePinPositions[1]) | (uint8_t)(0 << phaseLinePinPositions[2]) | (uint8_t)(1 << phaseLinePinPositions[3]))
    };
    
    //Range check
    if(stepSequenceIndexCopy > 7)
    {
        stepSequenceIndexCopy = (isCounterClockwise) ? 7 : 0;
    }
    
    //Clear the pin positions in the sequence value so they can be set without changing the rest of the ports value
	sequenceValue &= ~(1 << phaseLinePinPositions[0] | 1 << phaseLinePinPositions[1] | 1 << phaseLinePinPositions[2] | 1 << phaseLinePinPositions[3]);
    
    //Set the index copy to the appropriate current step
    sequenceValue |= (stepSequence[stepSequenceIndexCopy]);
    
    //If we're at the final step and we're moving clockwise...
    if(stepSequenceIndexCopy >= 7 && isCounterClockwise == false)
    {
        //Set to the first step
        stepSequenceIndexCopy = 0;
    }
    //If we're at the first step and we're moving counter clockwise...
    else if(stepSequenceIndexCopy == 0 && isCounterClockwise == true)
    {
        //Set to the last step
        stepSequenceIndexCopy = 7;
    }
    //Else...
    else
    {
        //Adjust the step index based on direction
        stepSequenceIndexCopy = (isCounterClockwise == true) ? stepSequenceIndexCopy - 1: stepSequenceIndexCopy + 1;
    }
    
    
    
	
    
    //After getting the new sequence value and index value, set the values in the pointer parameters
	*stepperPhasePortValue = sequenceValue;
    *stepSequenceIndex = stepSequenceIndexCopy;
    
}



/**
 * @brief Gets the value for steppers full step. \n
 * Example use: \n
 * uchrStepperPortValue = STEPPER_PORT_READ_REGISTER; \n
 * StepperGetFullStep(&uchrStepperPortValue, auchrPinPositions, &uchrStepperIndex, blnIsCounterClockwise); \n
 * STEPPER_PORT_OUT_REGISTER = uchrStepperPortValue; \n
 * @author Tim Robbins
 * @param stepperPhasePortValue The port value the stepper motors phase lines are connected to
 * @param phaseLinePinPositions The positions of the phase line pins
 * @param stepSequenceIndex The index for the current step position
 * @param isCounterClockwise If taking a counter clockwise step
 */
void StepperFullStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise)
{
    //Variables
    uint8_t stepSequenceIndexCopy = *stepSequenceIndex; //Copy of the step sequence index
    uint8_t sequenceValue = *stepperPhasePortValue; //Copy of the passed value that's on the phase line port
    
    //Array of the step sequencies, for easy setting
    uint8_t stepSequence[4] = 
    {
        ((uint8_t)(1 << phaseLinePinPositions[0]) | (uint8_t)(0 << phaseLinePinPositions[1]) | (uint8_t)(0 << phaseLinePinPositions[2]) | (uint8_t)(1 << phaseLinePinPositions[3])),
        ((uint8_t)(1 << phaseLinePinPositions[0]) | (uint8_t)(1 << phaseLinePinPositions[1]) | (uint8_t)(0 << phaseLinePinPositions[2]) | (uint8_t)(0 << phaseLinePinPositions[3])),
        ((uint8_t)(0 << phaseLinePinPositions[0]) | (uint8_t)(1 << phaseLinePinPositions[1]) | (uint8_t)(1 << phaseLinePinPositions[2]) | (uint8_t)(0 << phaseLinePinPositions[3])),
        ((uint8_t)(0 << phaseLinePinPositions[0]) | (uint8_t)(0 << phaseLinePinPositions[1]) | (uint8_t)(1 << phaseLinePinPositions[2]) | (uint8_t)(1 << phaseLinePinPositions[3]))
        
    };
    
    //Range check
    if(stepSequenceIndexCopy > 3)
    {
        stepSequenceIndexCopy = (isCounterClockwise) ? 3 : 0;
    }
    
    //Clear the pin positions in the sequence value so they can be set without changing the rest of the ports value
	sequenceValue &= ~(1 << phaseLinePinPositions[0] | 1 << phaseLinePinPositions[1] | 1 << phaseLinePinPositions[2] | 1 << phaseLinePinPositions[3]);
    
    //Set the index copy to the appropriate current step
    sequenceValue |= (stepSequence[stepSequenceIndexCopy]);
    
    //If we're at the final step and we're moving clockwise...
    if(stepSequenceIndexCopy >= 3 && isCounterClockwise == false)
    {
        //Set to the first step
        stepSequenceIndexCopy = 0;
    }
    //If we're at the first step and we're moving counter clockwise...
    else if(stepSequenceIndexCopy == 0 && isCounterClockwise == true)
    {
        //Set to the last step
        stepSequenceIndexCopy = 3;
    }
    //Else...
    else
    {
        //Adjust the step index based on direction
        stepSequenceIndexCopy = (isCounterClockwise == true) ? stepSequenceIndexCopy - 1: stepSequenceIndexCopy + 1;
    }
    
    
    
	
    
    //After getting the new sequence value and index value, set the values in the pointer parameters
	*stepperPhasePortValue = sequenceValue;
    *stepSequenceIndex = stepSequenceIndexCopy;
}





/**
 * @brief Has the directly connected stepper motor take a full step. \n
 * Example use: \n
 * uchrStepperPortValue = STEPPER_PORT_READ_REGISTER; \n
 * StepperGetFullStep(&uchrStepperPortValue, auchrPinPositions, &uchrStepperIndex, blnIsCounterClockwise); \n
 * STEPPER_PORT_OUT_REGISTER = uchrStepperPortValue; \n
 * @author Tim Robbins
 * @param stepperPhasePortValue The port value the stepper motors phase lines are connected to
 * @param phaseLinePinPositions The positions of the phase line pins
 * @param stepSequenceIndex The index for the current step position
 * @param isCounterClockwise If taking a counter clockwise step
 */
void StepperGetFullStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise)
{
    //Variables
    uint8_t stepSequenceIndexCopy = *stepSequenceIndex; //Copy of the step sequence index
    uint8_t sequenceValue = *stepperPhasePortValue; //Copy of the passed value that's on the phase line port
    
    //Clear the pin positions in the sequence value so they can be set without changing the rest of the ports value
	sequenceValue &= ~(1 << phaseLinePinPositions[0] | 1 << phaseLinePinPositions[1] | 1 << phaseLinePinPositions[2] | 1 << phaseLinePinPositions[3]);
    
    //Check the step sequence index and set the out value appropriately
	switch(stepSequenceIndexCopy)
	{
		case 0:
			sequenceValue |= (uint8_t)((1 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3]));
			stepSequenceIndexCopy = (isCounterClockwise) ? 3 : 1;
			break;
			
		case 1:
			sequenceValue |= (uint8_t)((1 << phaseLinePinPositions[0]) | (1 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));
			stepSequenceIndexCopy = (isCounterClockwise) ? stepSequenceIndexCopy - 1 : stepSequenceIndexCopy + 1;
			break;
		
		case 2:
			sequenceValue |= (uint8_t)((0 << phaseLinePinPositions[0]) | (1 << phaseLinePinPositions[1]) | (1 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));
			stepSequenceIndexCopy = (isCounterClockwise) ? stepSequenceIndexCopy - 1 : stepSequenceIndexCopy + 1;
			break;
		
		case 3:
			sequenceValue |= (uint8_t)((0 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (1 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3]));
			stepSequenceIndexCopy = (isCounterClockwise) ? 2 : 0;
			break;
			
		default:
			sequenceValue |= (isCounterClockwise) ? (uint8_t)((0 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (1 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3])) : (uint8_t)((1 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3]));
			stepSequenceIndexCopy = (isCounterClockwise) ? 2 : 1;
			break;
	};
    
    
	
    
    //After getting the new sequence value and index value, set the values in the pointer parameters
	*stepperPhasePortValue = sequenceValue;
    *stepSequenceIndex = stepSequenceIndexCopy;
}




/**
 * Has the directly connected stepper motor take a half step \n
 * Example use: \n
 * uchrStepperPortValue = STEPPER_PORT_READ_REGISTER; \n
 * StepperGetFullStep(&uchrStepperPortValue, auchrPinPositions, &uchrStepperIndex, blnIsCounterClockwise); \n
 * STEPPER_PORT_OUT_REGISTER = uchrStepperPortValue; \n
 * @author Tim Robbins
 * @param stepperPhasePortValue The port value the stepper motors phase lines are connected to
 * @param phaseLinePinPositions The positions of the phase line pins
 * @param stepSequenceIndex The index for the current step position
 * @param isCounterClockwise If taking a counter clockwise step
 */
void StepperGetHalfStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise)
{
    //Variables
    uint8_t stepSequenceIndexCopy = *stepSequenceIndex; //Copy of the step sequence index
    uint8_t sequenceValue = *stepperPhasePortValue; //Copy of the passed value that's on the phase line port
    
    //Clear the pin positions in the sequence value so they can be set without changing the rest of the ports value
	sequenceValue &= ~(1 << phaseLinePinPositions[0] | 1 << phaseLinePinPositions[1] | 1 << phaseLinePinPositions[2] | 1 << phaseLinePinPositions[3]);
    
    //Check the step sequence index and set the out value appropriately
	switch(stepSequenceIndexCopy)
	{
		case 0:
			stepSequenceIndexCopy = (isCounterClockwise) ? 7 : 1;
			sequenceValue |= (uint8_t)((1 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3]));
			break;
			
		case 1:
			stepSequenceIndexCopy = (isCounterClockwise) ? stepSequenceIndexCopy - 1 : stepSequenceIndexCopy + 1;
			sequenceValue |= (uint8_t)((1 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));
			break;
		
		case 2:
			stepSequenceIndexCopy = (isCounterClockwise) ? stepSequenceIndexCopy - 1 : stepSequenceIndexCopy + 1;
			sequenceValue |= (uint8_t)((1 << phaseLinePinPositions[0]) | (1 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));

			break;
		
		case 3:
			stepSequenceIndexCopy = (isCounterClockwise) ? stepSequenceIndexCopy - 1 : stepSequenceIndexCopy + 1;
			sequenceValue |= (uint8_t)((0 << phaseLinePinPositions[0]) | (1 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));
			break;
		
		case 4:
			stepSequenceIndexCopy = (isCounterClockwise) ? stepSequenceIndexCopy - 1 : stepSequenceIndexCopy + 1;
			sequenceValue |= (uint8_t)((0 << phaseLinePinPositions[0]) | (1 << phaseLinePinPositions[1]) | (1 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));
			break;
		
		case 5:
			stepSequenceIndexCopy = (isCounterClockwise) ? stepSequenceIndexCopy - 1 : stepSequenceIndexCopy + 1;
			sequenceValue |= (uint8_t)((0 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (1 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));
			break;
			
		case 6:
			stepSequenceIndexCopy = (isCounterClockwise) ? stepSequenceIndexCopy - 1 : stepSequenceIndexCopy + 1;
			sequenceValue |= (uint8_t)((0 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (1 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3]));
			break;
			
		case 7:
			stepSequenceIndexCopy = (isCounterClockwise) ? 6 : 0;
			sequenceValue |= (uint8_t)((0 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3]));
			break;
				
			
		default:
			stepSequenceIndexCopy = (isCounterClockwise) ? 6 : 1;
			sequenceValue |= (isCounterClockwise) ? (uint8_t)((0 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3])) : (uint8_t)((1 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3]));
			
			break;
	};

    
    //After getting the new sequence value and index value, set the values in the pointer parameters
	*stepperPhasePortValue = sequenceValue;
    *stepSequenceIndex = stepSequenceIndexCopy;
}



/**
 * Has the directly connected stepper motor take a wave step \n
 * Example use: \n
 * uchrStepperPortValue = STEPPER_PORT_READ_REGISTER; \n
 * StepperGetFullStep(&uchrStepperPortValue, auchrPinPositions, &uchrStepperIndex, blnIsCounterClockwise); \n
 * STEPPER_PORT_OUT_REGISTER = uchrStepperPortValue; \n
 * @author Tim Robbins
 * @param stepperPhasePortValue The port value the stepper motors phase lines are connected to
 * @param phaseLinePinPositions The positions of the phase line pins
 * @param stepSequenceIndex The index for the current step position
 * @param isCounterClockwise If taking a counter clockwise step
 */
void StepperGetWaveStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise)
{
    //Variables
    uint8_t stepSequenceIndexCopy = *stepSequenceIndex; //Copy of the step sequence index
    uint8_t sequenceValue = *stepperPhasePortValue; //Copy of the passed value that's on the phase line port
    
    //Clear the pin positions in the sequence value so they can be set without changing the rest of the ports value
	sequenceValue &= ~(1 << phaseLinePinPositions[0] | 1 << phaseLinePinPositions[1] | 1 << phaseLinePinPositions[2] | 1 << phaseLinePinPositions[3]);
    
    //Check the step sequence index and set the out value appropriately
    switch(stepSequenceIndexCopy)
	{
		case 0:
			sequenceValue |= (uint8_t)((1 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));
			stepSequenceIndexCopy = (isCounterClockwise) ? 3 : 1;
			break;
			
		case 1:
			sequenceValue |= (uint8_t)((0 << phaseLinePinPositions[0]) | (1 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));
			stepSequenceIndexCopy = (isCounterClockwise) ? stepSequenceIndexCopy - 1 : stepSequenceIndexCopy + 1;
			break;
		
		case 2:
			sequenceValue |= (uint8_t)((0 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (1 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));
			stepSequenceIndexCopy = (isCounterClockwise) ? stepSequenceIndexCopy - 1 : stepSequenceIndexCopy + 1;
			break;
		
		case 3:
			sequenceValue |= (uint8_t)((0 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3]));
			stepSequenceIndexCopy = (isCounterClockwise) ? 2 : 0;
			break;
			
		default:
			stepSequenceIndexCopy = (isCounterClockwise) ? 2 : 1;
			sequenceValue |= (isCounterClockwise) ? (uint8_t)((0 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (1 << phaseLinePinPositions[3])) : (uint8_t)((1 << phaseLinePinPositions[0]) | (0 << phaseLinePinPositions[1]) | (0 << phaseLinePinPositions[2]) | (0 << phaseLinePinPositions[3]));
			
			break;
	};
    
    
    //After getting the new sequence value and index value, set the values in the pointer parameters
    *stepperPhasePortValue = sequenceValue;
    *stepSequenceIndex = stepSequenceIndexCopy;
}







#endif
