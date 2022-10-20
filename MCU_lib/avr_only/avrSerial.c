/**
 * \file avrSerial.c
 * \brief Header file for avr serial utilities
 * \author Tim Robbins
 * \todo test LIN, add functionality for atmegaXXm1 chips UART
 */
#ifndef __AVRSERIAL_C__
#define __AVRSERIAL_C__ 1

#include "avrSerial.h"
#include <util/atomic.h>


#ifdef UDR0
	
	#if _SERIAL_USE_INT == 1

		///Buffer for the USART
		volatile unsigned char uchrSerial0ReadBuffer[MAX_SERIAL_SIZE];

		///Index for the read serial data
		volatile unsigned char uchrBuffer0ReadIndex = 0;
	
		/**
		* \brief RX receive vector
		*
		*/
		ISR(USART0_RX_vect) {
			uchrSerial0ReadBuffer[uchrBuffer0ReadIndex] = UDR0;
			uchrBuffer0ReadIndex++;
			if(uchrBuffer0ReadIndex >= MAX_SERIAL_SIZE) {
				uchrBuffer0ReadIndex = 0;
			}
		}

		/**
		 * @brief Gets and returns the USART0 int buffer string
		 * 
		 * 
		 * @return unsigned* 
		 */
		unsigned char* USART0_get_buffer_string() {
			return uchrSerial0ReadBuffer;
		}

	#endif
	

	
	
	// /**
	// * \brief Sets the baud for the USART
	// * \param uchrBaudH The high byte
	// * \param uchrBaudL The low byte
	// */
	// void USART0_set_baud(unsigned char uchrBaudH, unsigned char uchrBaudL) {
	// 	UBRR0H = uchrBaudH;
	// 	UBRR0L = uchrBaudL;
	// }
	
	
	/**
	* \brief Reads and waits for a single byte from RX
	* \return The received byte
	*/
	uint8_t USART0_read_byte()
	{
		while (!(UCSR0A & (1 << RXC0)));
		return UDR0;
	}
	
	
	/**
	* \brief Reads and waits for a single byte from RX unless it hits a timeout
	* \return The received byte
	*/
	uint8_t USART0_read_byte_timout(uint16_t timeout)
	{
		while (!(UCSR0A & (1 << RXC0))) {
			if(--timeout == 0) return 0;
		} 
		return UDR0;
	}



	
	/**
	* \brief Writes single byte to TX
	* \param uchrByteToWrite The byte to write
	*/
	void USART0_write_byte(uint8_t uchrByteToWrite)
	{
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = uchrByteToWrite;
	}
	
	
	/**
	* \brief Writes string to TX
	* \param strStringtoWrite The string to write
	*/
	void USART0_write_string(uint8_t* strStringtoWrite)
	{
		//Variables
		uint8_t currentByte = 1;
		
		
		
		for(uint8_t i = 0; currentByte != '\0'; i++) {
			currentByte = *(strStringtoWrite + i);

			while (!(UCSR0A & (1 << UDRE0)));
			UDR0 = currentByte;
				
		}
		
		//Clear usart buffer
		UDR0 = 0;
		
	}


#endif



#ifdef UDR1
	
	#if _SERIAL_USE_INT == 1

		///Buffer for the USART
		volatile unsigned char uchrSerial1ReadBuffer[MAX_SERIAL_SIZE];

		///Index for the read serial data
		volatile unsigned char uchrBuffer1ReadIndex = 0;
	
		/**
		* \brief RX receive vector
		*
		*/
		ISR(USART1_RX_vect) {
			uchrSerial1ReadBuffer[uchrBuffer1ReadIndex] = UDR1;
			uchrBuffer1ReadIndex++;
			if(uchrBuffer1ReadIndex >= MAX_SERIAL_SIZE) {
				uchrBuffer1ReadIndex = 1;
			}
		}

		/**
		 * @brief Gets and returns the USART1 int buffer string
		 * 
		 * 
		 * @return unsigned* 
		 */
		unsigned char* USART1_get_buffer_string() {
			return uchrSerial1ReadBuffer;
		}

	#endif
	
	
	/**
	* \brief Reads single byte from RX
	* \return The received byte
	*/
	uint8_t USART1_read_byte()
	{
		while (!(UCSR1A & (1 << RXC1)));
		return UDR1;
	}
	
	/**
	* \brief Writes single byte to TX
	* \param uchrByteToWrite The byte to write
	*/
	void USART1_write_byte(uint8_t uchrByteToWrite)
	{
		while (!(UCSR1A & (1 << UDRE1)));
		UDR1 = uchrByteToWrite;
	}
	
	
	/**
	* \brief Writes string to TX
	* \param strStringtoWrite The string to write
	*/
	void USART1_write_string(uint8_t* strStringtoWrite)
	{
		if(strlen(strStringtoWrite) == 0 && strStringtoWrite != NULL) {
			while (!(UCSR1A & (1 << UDRE1)));
			UDR1 = *strStringtoWrite;
		}
		else {
			unsigned short i;
			unsigned char j;
			for(i = 0; *(strStringtoWrite + i) != 0; i++) {
				j = *(strStringtoWrite + i);
				USART1_write_byte(j);
			}
		}
	}


#endif



#ifdef LINCR
	
	
	
	
	/**
	* \brief Reads single byte from RX
	* \return The received byte
	*/
	uint8_t LIN_read_byte()
	{
		while (!(LINSIR & (1 << LBUSY)));
		return LINDAT;
	}
	
	/**
	* \brief Writes single byte to TX
	* \param uchrByteToWrite The byte to write
	*/
	void LIN_write_byte(uint8_t uchrByteToWrite)
	{
		
		
		while ((LINSIR & (1 << LBUSY))){
			if(readBit(LINSIR,LERR)) return;
		}
		
		LINDAT = uchrByteToWrite;
	}
	
	
	/**
	* \brief Writes string to TX
	* \param strStringtoWrite The string to write
	*/
	void LIN_write_string(uint8_t* strStringtoWrite)
	{
		if(strlen(strStringtoWrite) == 0 && strStringtoWrite != NULL) {
			LIN_write_byte(*strStringtoWrite);
		}
		else {
			unsigned short i;
			unsigned char j;
			for(i = 0; *(strStringtoWrite + i) != 0; i++) {
				j = *(strStringtoWrite + i);
				LIN_write_byte(j);
			}
		}
	}


#endif







#endif




