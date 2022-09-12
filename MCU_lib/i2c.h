/**
 * \file i2c.h
 * \author Tim Robbins
 * \brief Header file for i2c functions. 
 * REQUIREMENTS: "config.h" and "mcuUtils.h" must be included for use \n
 * \todo Need to make work for PIC microcontrollers, need to add explanations of required macro definitions
 */ 
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "mcuPinUtils.h"
#include "mcuUtils.h"

#define I2C_TIMEOUT_START		0			// bit 0: timeout start-condition
#define I2C_TIMEOUT_SENDADDRESS	1			// bit 0: timeout device-address
#define I2C_TIMEOUT_BYTE		2			// bit 0: timeout byte-transmission
#define I2C_TIMEOUT_READACK		3			// bit 0: timeout read acknowledge
#define I2C_TIMEOUT_READNACK	4			// bit 0: timeout read nacknowledge


#ifndef F_I2C
#define F_I2C			100000UL// clock i2c
#endif

#ifndef I2C_BITRATE
#define I2C_BITRATE		3
#endif

#ifndef PSC_I2C
#define PSC_I2C			1		// prescaler i2c
#endif

#ifdef __AVR
	
#ifndef _AVR_IO_H_
#include <avr/io.h>
#endif


#ifdef TWCR
	





#ifndef SET_TWBR
	#define SET_TWBR		(F_CPU/F_I2C-16UL)/(PSC_I2C*2UL)
#elif SET_TWBR < 0 || SET_TWBR > 255
	#error "TWBR out of range, change PSC_I2C or F_I2C."
#endif





#define __I2C_TIMEOUT()			(F_CPU/F_I2C*2)
#define i2c_stop()				TWCR = (1 << TWINT)|(1 << TWSTO)|(1 << TWEN)
#define I2CStopAck()			noP();
#define __I2C_START()			TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN)
#define __I2C_START_ACK()		TWCR = ((1<<TWINT)|(1<<TWEN)|(1<<TWEA))
#define __I2C_START_SEND()		TWCR = (1 << TWINT)|( 1 << TWEN)
#define __I2C_START_EN()		TWCR = (1 << TWINT)|( 1 << TWEN)
#define __I2C_WAIT_TILL_RDY()   while((TWCR & (1 << TWINT)) == 0)
#define __I2C_LOAD_DATA_REG(v)	TWDR = v
#define __I2C_SET_CLK(v)		TWBR = (uint8_t)SET_TWBR
#define __I2C_DATA_REG			TWDR
#define __I2C_SET_EN()			TWCR = (1 << TWEN)
#define __I2C_SET_PRESCALER()   switch (PSC_I2C) {\
	case 4:\
	TWSR = 0x1;\
	break;\
	case 16:\
	TWSR = 0x2;\
	break;\
	case 64:\
	TWSR = 0x3;\
	break;\
	default:\
	TWSR = 0x00;\
	break;\
}
	
	
extern void I2CInit(void);					// init hw-i2c
extern uint8_t I2CStart(uint8_t i2c_address);	// send I2CStart_condition
extern uint8_t I2CByte(uint8_t byte);
extern uint8_t I2CSendByte(uint8_t i2c_address, uint8_t byte);
extern uint8_t I2CSendBytes(uint8_t i2c_address, uint8_t *bytes);
extern uint8_t I2CReadAck(void);      	   	// read byte with ACK
extern uint8_t I2CReadNack(void);        	// read byte with NACK

#endif


#elif defined(__XC)

#include <xc.h>        /* XC8 General Include File */

#elif defined(HI_TECH_C)

#include <htc.h>       /* HiTech General Include File */

#elif defined(__18CXX)

#include <p18cxxx.h>   /* C18 General Include File */

#elif (defined __XC8)

#include <xc.h>            /* XC8 General Include File */

#endif
	
#if (defined(__XC8) || defined(__XC) || defined(HI_TECH_C) || defined(__18CXX))


#define __I2C_TIMEOUT()			(F_CPU/F_I2C*2)



#if defined(SSP2CON2)

#if !defined(I2C_USE_SSP1)

#define i2c_stop()				SSP2CON2bits.PEN = 1
#define __I2C_START()			SSP2CON2bits.SEN = 1
#define __I2C_START_ACK()		SSP2CON2bits.RCEN = 1
#define __I2C_WAIT_TILL_RDY()	while (   (SSP2CON2 & 0b00011111)    ||    (SSP2STAT & 0b00000100)   ) //check the bis on registers to make sure the IIC is not in progress

#else

#define i2c_stop()				SSP1CON2bits.PEN = 1
#define __I2C_START()			SSP1CON2bits.SEN = 1
#define __I2C_START_ACK()		SSP1CON2bits.RCEN = 1
#define __I2C_WAIT_TILL_RDY()	while (   (SSP1CON2 & 0b00011111)    ||    (SSP1STAT & 0b00000100)   ) //check the bis on registers to make sure the IIC is not in progress


#endif


#endif


#define __I2C_LOAD_DATA_REG(v)  SSPBUF = v
#define __I2C_SET_CLK(v)		SSPADD = ((F_CPU/(4*v*100))-1)
#define __I2C_DATA_REG			SSPBUF
#define I2CStopAck()			ACKEN = 1
#define __I2C_SET_EN()			SSPSTAT = 0b00000000
#define __I2C_SET_PRESCALER()	SSPCON  = 0b00101000; SSPCON2 = 0b00000000

extern void I2CInit(void);					// init hw-i2c
extern uint8_t I2CStart(uint8_t i2c_address);	// send I2CStart_condition
extern uint8_t I2CByte(uint8_t byte);
extern uint8_t I2CSendByte(uint8_t i2c_address, uint8_t byte);
extern uint8_t I2CSendBytes(uint8_t i2c_address, uint8_t *bytes);
extern uint8_t I2CReadAck(void);      	   	// read byte with ACK
extern uint8_t I2CReadNack(void);        	// read byte with NACK


#endif





#ifdef __cplusplus
}
#endif



#endif /* __I2C_H__ */