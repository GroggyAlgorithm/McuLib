/**
 * \file    steppers.h
 * \author  Tim Robbins - R&D Engineer, Atech Training
 * \brief   Header file for stepper motor functions
 * \version v2.0.0
 */
#ifndef STEPPERS_H
#define	STEPPERS_H 1

#ifdef	__cplusplus
extern "C" {
#endif
    
    
#ifdef __AVR

#include <avr/io.h>

#elif defined(__XC)

#include <xc.h>        /* XC8 General Include File */

#elif defined(HI_TECH_C)

#include <htc.h>       /* HiTech General Include File */

#elif defined(__18CXX)

#include <p18cxxx.h>   /* C18 General Include File */

#elif (defined __XC8)

#include <xc.h>            /* XC8 General Include File */

#endif
    
    
#include <stdbool.h>

    
//These are using arrays for the values. Is probably better?
    
extern void StepperWaveStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise);
extern void StepperHalfStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise);
extern void StepperFullStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise);




//These are using switch statements for the values

extern void StepperGetFullStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise);
extern void StepperGetHalfStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise);
extern void StepperGetWaveStep(volatile uint8_t* stepperPhasePortValue, uint8_t phaseLinePinPositions[4], uint8_t* stepSequenceIndex, bool isCounterClockwise);
    




#ifdef	__cplusplus
}
#endif

#endif	/* STEPPERS_H */

