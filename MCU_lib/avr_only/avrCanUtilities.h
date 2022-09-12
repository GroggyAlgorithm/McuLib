/**
 * \file avrCanUtilities.h
 * \author Timothy Robbins
 * \brief avr CAN utilities
 */ 

#if defined(__cplusplus) && defined(__AVR)
#if defined(__AVR_ATmega32C1__) || defined(__AVR_ATmega64C1__) || defined(__AVR_ATmega16M1__) || defined(__AVR_ATmega32M1__) || defined(__AVR_ATmega64M1__)

#ifndef __AVR_CAN_UTILITIES_H__
#define __AVR_CAN_UTILITIES_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr_can.h"
#include "string.h"

///Reads data into the passed frame buffer and returns 1 if frame was returned, else 0
#define CAN_getData(frame_buffer)   Can0.read((CAN_FRAME)frame_buffer)

///Enables the can in the power reduction register
#define CAN_enable()    PRR &= ~(1 << PRCAN)

///Disables the can in the power reduction register
#define CAN_disable()    PRR |= (1 << PRCAN)

///Helper for forming the id to send onto the can network
#define CAN_create_msg_id(mainId, offsetId1, offsetId2)		(mainId | offsetId1 | offsetId2)

bool CAN_init(uint8_t canBaud, void(*set_tx_box_count)(void), bool bigEndian);
bool CAN_init_rx_all(uint8_t canBaud, bool bigEndian);

bool CAN_init_tx(uint8_t canBaud, bool bigEndian, uint8_t txMobCount);

bool CAN_send(uint8_t bytes[], uint32_t id, uint8_t priority);

bool CAN_send_char(uint8_t value, uint32_t id, uint8_t priority);
bool CAN_send_short(uint16_t value, uint32_t id, uint8_t priority);
bool CAN_send_int(uint32_t value, uint32_t id, uint8_t priority);
bool CAN_send_long(uint64_t value, uint32_t id, uint8_t priority);

int8_t CAN_process_frame(int8_t(*can_frame_callback)(CAN_FRAME&));

#endif /* __AVR_CAN_UTILITIES_H_ */
#endif
#endif