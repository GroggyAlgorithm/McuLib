/**
 * \file i2c.c
 * \author Tim Robbins
 * \brief Source file for i2c functions
 */ 
#ifndef __I2C_C__
#define __I2C_C__


#if (defined TWCR || defined SSPBUF)

#include "i2c.h"


/**********************************************
 \name I2CStart
 
 \brief Start TWI/I2C interface
 
 \param uint8_t i2c_address address of receiver
 \return uint8_t status of start
 **********************************************/
uint8_t I2CStart(uint8_t i2c_address){
	// i2c start
	uint16_t index = 0;
    __I2C_START();
	
	uint16_t timeout =__I2C_TIMEOUT();
    __I2C_WAIT_TILL_RDY() {
		index++;
		if(index >= timeout){
			return  (1 << I2C_TIMEOUT_START);
		}
	};
    // send address
    __I2C_LOAD_DATA_REG(i2c_address);
    __I2C_START_EN();
    timeout = F_CPU/F_I2C*2.0;
    __I2C_WAIT_TILL_RDY() {
		index++;
		if(index >= timeout){
			return (1 << I2C_TIMEOUT_SENDADDRESS);
		}
	};
	
	return 0;
   
}



uint8_t I2CByte(uint8_t byte) {
	uint16_t index = 0;
	__I2C_LOAD_DATA_REG(byte);
    __I2C_START_EN();
    uint16_t timeout = __I2C_TIMEOUT();
    __I2C_WAIT_TILL_RDY(){
	    index++;
	    if(index >= timeout){
		    return (1 << I2C_TIMEOUT_BYTE);
	    }
    };
	
	return 0;
	
}



uint8_t I2CByte(uint8_t i2c_address, uint8_t byte) {
	uint8_t I2C_status = 0;
	uint16_t index = 0;

	I2C_status = I2CStart(i2c_address);

	if(I2C_status == 0) {
		__I2C_LOAD_DATA_REG(byte);
		__I2C_START_EN();
		uint16_t timeout = __I2C_TIMEOUT();
		__I2C_WAIT_TILL_RDY() {
			index++;
			if(index >= timeout){
				I2C_status = (1 << I2C_TIMEOUT_BYTE);
			}
		};
	}

	i2c_stop();

	return I2C_status;
}



uint8_t I2CBytes(uint8_t i2c_address, uint8_t *bytes) {
	uint8_t I2C_status = 0;
	uint16_t index = 0;

	I2C_status = I2CStart(i2c_address);

	if(I2C_status == 0) {
		while (*bytes)
		{
			__I2C_LOAD_DATA_REG(bytes++);
			__I2C_START_EN();
			uint16_t timeout = __I2C_TIMEOUT();
			__I2C_WAIT_TILL_RDY() {
				index++;
				if(index >= timeout){
					I2C_status = (1 << I2C_TIMEOUT_BYTE);
				}
			};
		}
		
		
	}

	i2c_stop();

	return I2C_status;
}




/**********************************************
	\name I2CReadAck
 
 	\brief read acknowledge from TWI/I2C Interface
 
 **********************************************/
uint8_t I2CReadAck(void){
    uint16_t index = 0;
	__I2C_START_ACK();
    uint16_t timeout = __I2C_TIMEOUT();
    __I2C_WAIT_TILL_RDY() {
		index++;
		if(index >= timeout){
			return (1 << I2C_TIMEOUT_READACK);
		}
	};
	I2CStopAck();
    return __I2C_DATA_REG;
}


 /**********************************************
	\name I2CReadNack
 
 	\brief read non-acknowledge from TWI/I2C Interface
 
 **********************************************/
uint8_t I2CReadNack(void){
    uint16_t index = 0;
	__I2C_START_EN();
    uint16_t timeout =__I2C_TIMEOUT();
    __I2C_WAIT_TILL_RDY(){
		index++;
		if(index >= timeout){
			return (1 << I2C_TIMEOUT_READACK);
		}
	};
	I2CStopAck();
    return __I2C_DATA_REG;
}

/**********************************************
 \name I2CInit
 
 \brief Initialize TWI/I2C interface
 
 **********************************************/
void I2CInit(void){
    
    // set clock and prescaler
	__I2C_SET_PRESCALER();
    __I2C_SET_CLK(F_I2C);
	
	
	
	__I2C_SET_EN();
}



 #endif

#endif