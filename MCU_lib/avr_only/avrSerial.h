/**
 * \file avrSerial.h
 * \brief Header file for avr serial utilities
 * \author Tim Robbins
 * \todo test LIN
 */
#ifndef __AVRSERIAL_H__
#define __AVRSERIAL_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

//Switch name to correct config file
#include "config.h"

#ifndef BAUD

#error avrSerial.h: BAUD must be defined in "config.h"

#endif

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "mcuUtils.h"
#include "mcuDelays.h"
#include "mcuPinUtils.h"



///Max size for the serial buffer
#ifndef MAX_SERIAL_SIZE
	#define MAX_SERIAL_SIZE	 64
#endif


#ifndef _SERIAL_USE_INT
	//#warning avrSerial.h: ISR(SERIAL_VECT) not in use in this file. Must be implemented on your own.
	#define _SERIAL_USE_INT	0
#endif


#include <util/setbaud.h>

#ifdef UDR0

	#if _SERIAL_USE_INT == 1
		unsigned char* USART0_get_buffer_string();
	#endif

	/**
	* \brief Helper for setting USART0 mode
	*
	*/
	#define USART0_async_noparity_1bit_stop_1byte_size() UCSR0C = (1 << UMSEL01) | (1 << UCSZ01) | (1 << UCSZ00)

	/**
	* \brief Enables rx interrupt
	*
	*/
	#define USART0_enable_rx_int() UCSR0B |= (1 << RXCIE0)

	/**
	* \brief Disables rx interrupt
	*
	*/
	#define USART0_disable_rx_int() UCSR0B &= ~(1 << RXCIE0)

	#ifdef U2X0
	/**
	* \brief Enables USART 2x speed mode
	*
	*/
	#define USART0_enable_2x() UCSR0A |= (1 << U2X0)

	/**
	* \brief Disables USART 2x speed mode
	*
	*/
	#define USART0_disable_2x() UCSR0A &= ~(1 << U2X0)

	#endif

	/**
	* \brief Enables USART RX
	*
	*/
	#define USART0_enable_rx()  UCSR0B |= (1 << RXEN0)

	/**
	* \brief Disables USART0 RX
	*
	*/
	#define USART0_disable_rx()  UCSR0B &= ~(1 << RXEN0)

	/**
	* brief Enables USART TX
	*
	*/
	#define USART0_enable_tx()  UCSR0B |= (1 << TXEN0)

	/**
	* \brief Disables USART0 TX
	*
	*/
	#define USART0_disable_tx()  UCSR0B &= ~(1 << TXEN0)

	/**
	* \brief Enables USART0 RX and TX
	*
	*/
	#define USART0_enable()  UCSR0B |= ((1 << TXEN0) | (1 << RXEN0))

	/**
	* \brief Disables USART0 RX and TX
	*
	*/
	#define USART0_disable()  UCSR0B &= ~((1 << TXEN0) | (1 << RXEN0))

	/**
	* \brief Helper for setting the baud for the USART
	* \param uchrBaudH The high byte
	* \param uchrBaudL The low byte
	*/
	#define USART0_set_baud(uchrBaudH, uchrBaudL) UBRR0H = uchrBaudH; UBRR0L = uchrBaudL
	
	
	
	/**
	* \brief Helper for setting the baud for the USART
	*/
	#define USART0SetBaud() UBRR0H = UBRRH_VALUE; UBRR0L = UBRRL_VALUE
	


	/**
	* \brief Returns if the USART can accept data to be transmitted
	* \return If the TX is ready or not
	*/
	#define USART0_tx_ready() (UCSR0A & (1 << UDRE0))

	/**
	* \brief Returns if the USART has received data
	* \return If the RX has received data
	*/
	#define USART0_rx_ready() (UCSR0A & (1 << RXC0))

	/**
	* \brief Returns if the USART TX is currently busy
	* \return If the TX is busy
	*/
	#define USART0_tx_busy() (!(UCSR0A & (1 << TXC0)))


	uint8_t USART0_read_byte();
	uint8_t USART0_read_byte_timout(uint16_t timeout);
	void USART0_write_byte(uint8_t uchrByteToWrite);
	void USART0_write_string(uint8_t* strStringtoWrite);

#endif


#ifdef UDR1

	#if _SERIAL_USE_INT == 1
	unsigned char* USART1_get_buffer_string();
	#endif

	/**
	* \brief Helper for setting USART1 mode
	*
	*/
	#define USART1_async_noparity_1bit_stop_1byte_size() UCSR1C = (1 << UMSEL11) | (1 << UCSZ11) | (1 << UCSZ10)
	
	/**
	* \brief Enables rx interrupt
	*
	*/
	#define USART1_enable_rx_int() UCSR1B |= (1 << RXCIE1)

	/**
	* \brief Disables rx interrupt
	*
	*/
	#define USART1_disable_rx_int() UCSR1B &= ~(1 << RXCIE1)

	#ifdef U2X1
	/**
	* \brief Enables USART 2x speed mode
	*
	*/
	#define USART1_enable_2x() UCSR1A |= (1 << U2X1)

	/**
	* \brief Disables USART 2x speed mode
	*
	*/
	#define USART1_disable_2x() UCSR1A &= ~(1 << U2X1)

	#endif

	/**
	* \brief Enables USART RX
	*
	*/
	#define USART1_enable_rx()  UCSR1B |= (1 << RXEN1)

	/**
	* \brief Disables USART1 RX
	*
	*/
	#define USART1_disable_rx()  UCSR1B &= ~(1 << RXEN1)

	/**
	* brief Enables USART TX
	*
	*/
	#define USART1_enable_tx()  UCSR1B |= (1 << TXEN1)

	/**
	* \brief Disables USART1 TX
	*
	*/
	#define USART1_disable_tx()  UCSR1B &= ~(1 << TXEN1)

	/**
	* \brief Enables USART1 RX and TX
	*
	*/
	#define USART1_enable()  UCSR1B |= ((1 << TXEN1) | (1 << RXEN1))

	/**
	* \brief Disables USART1 RX and TX
	*
	*/
	#define USART1_disable()  UCSR1B &= ~((1 << TXEN1) | (1 << RXEN1))

	

	/**
	* \brief Returns if the USART can accept data to be transmitted
	* \return If the TX is ready or not
	*/
	#define USART1_tx_ready() (UCSR1A & (1 << UDRE1))

	/**
	* \brief Returns if the USART has received data
	* \return If the RX has received data
	*/
	#define USART1_rx_ready() (UCSR1A & (1 << RXC1))

	/**
	* \brief Returns if the USART TX is currently busy
	* \return If the TX is busy
	*/
	#define USART1_tx_busy() (!(UCSR1A & (1 << TXC1)))

	/**
	* \brief Helper for setting the baud for the USART
	* \param uchrBaudH The high byte
	* \param uchrBaudL The low byte
	*/
	#define USART1_set_baud(uchrBaudH, uchrBaudL) UBRR1H = uchrBaudH; UBRR1L = uchrBaudL

	// void USART1_async_noparity_1bit_stop_1byte_size();
	// void USART1_enable_rx_int();
	// void USART1_disable_rx_int();
	// void USART1_enable_2x();
	// void USART1_disable_2x();
	// void USART1_set_baud(unsigned char uchrBaudH, unsigned char uchrBaudL);
	// void USART1_enable_rx();
	// void USART1_disable_rx();
	// void USART1_enable_tx();
	// void USART1_disable_tx();
	// void USART1_enable();
	// void USART1_disable();
	// bool USART1_tx_ready();
	// bool USART1_rx_ready();
	// bool USART1_tx_busy();
	uint8_t USART1_read_byte();
	void USART1_write_byte(uint8_t uchrByteToWrite);
	void USART1_write_string(uint8_t* strStringtoWrite);

#endif


#ifdef LINCR

	///Sets the baud rate for the LIN periphreal
	#define LIN_set_baud(v)				LINBRR = v

	///Sets the baud rate for the LIN periphreal using high and low bytes
	#define LIN_set_baudHL(highV, lowV)	LINBRRH = highV; LINBRRL = lowV

	///Sets the bit timing for the LIN periphreal
	#define LIN_set_timing(v)			LINBTR = v

	///Enables RX for lin
	#define LIN_enable_rx()	LINCR |= (0x06 << LCMD0)

	///Disables RX for lin
	#define LIN_disable_rx()	LINCR &= ~(0x06 << LCMD0)

	///Enables TX for lin
	#define LIN_enable_tx()	LINCR |= (0x05 << LCMD0)

	///Disables TX for lin
	#define LIN_disable_tx()	LINCR &= ~(0x05 << LCMD0)

	///Enables LIN
	#define LIN_enable()	LINCR |= (1 << LENA)

	///Disables LIN
	#define LIN_disable()	LINCR &= ~(1 << LENA)

	///Returns if LIN is ready for tx or recieved with rx
	#define LIN_ready() (LINSIR & (1 << LBUSY))

	///Returns if LIN is not ready for tx or has not recieved with rx
	#define LIN_busy() (!(LINSIR & (1 << LBUSY)))

	
	uint8_t LIN_read_byte();
	void LIN_write_byte(uint8_t uchrByteToWrite);
	void LIN_write_string(uint8_t* strStringtoWrite);

#endif




#ifdef __cplusplus
}
#endif


#endif /* __AVRSERIAL_H__ */