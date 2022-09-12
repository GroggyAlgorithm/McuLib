/**
 * \file avrCanUtilities.cpp
 * \author Timothy Robbins
 * \brief avr CAN utilities
 */ 
#if defined(__cplusplus) && defined(__AVR)
#if defined(__AVR_ATmega32C1__) || defined(__AVR_ATmega64C1__) || defined(__AVR_ATmega16M1__) || defined(__AVR_ATmega32M1__) || defined(__AVR_ATmega64M1__)

#ifndef __AVR_CAN_UTILITIES_CPP__
#define __AVR_CAN_UTILITIES_CPP__


#include "avrCanUtilities.h"

/**
 * @brief Initializes the CAN node using a function pointer callback function
 * 
 * 
 * @param canBaud The baud rate to use
 * @param set_tx_box_count Function pointer for setting up the mailboxes
 * @param bigEndian Reading MSB to LSB or LSB to MSB
 * @return true If initialization was able to happen
 * @return false If no initialization was able to happen
 */
bool CAN_init(uint8_t canBaud, void(*set_tx_box_count)(void), bool bigEndian)
{
	bool isInitialized = false;

	// Initialize CAN0 Set the proper baud rates here
	if (Can0.begin(canBaud)) {
		Can0.setBigEndian(bigEndian);
        set_tx_box_count();
		isInitialized = true;

        //Can library is interrupt driven, so turn on interrupts
        sei();
	}
	
	
	return isInitialized;
}



/**
 * @brief Initializes the CAN node to accept all as RX
 * 
 * 
 * @param canBaud The baud rate to use
 * @param bigEndian Reading MSB to LSB or LSB to MSB
 * @return true If initialization was able to happen
 * @return false If no initialization was able to happen
 */
bool CAN_init_rx_all(uint8_t canBaud, bool bigEndian)
{
	bool isInitialized = false;

	// Initialize CAN0 Set the proper baud rates here
	if (Can0.begin(canBaud)) {
		Can0.setBigEndian(bigEndian);
        Can0.setNumTXBoxes(0);
		isInitialized = true;

        //Can library is interrupt driven, so turn on interrupts
        sei();
	}
	
	
	return isInitialized;
}



/**
 * @brief Initializes the CAN node to use the amount of tx boxes passed
 * 
 * 
 * @param canBaud The baud rate to use
 * @param bigEndian Reading MSB to LSB or LSB to MSB
 * @param txMobCount The amount of TX boxes to use
 * @return true If initialization was able to happen
 * @return false If no initialization was able to happen
 */
bool CAN_init_tx(uint8_t canBaud, bool bigEndian, uint8_t txMobCount)
{
	bool isInitialized = false;

	// Initialize CAN0 Set the proper baud rates here
	if (Can0.begin(canBaud)) {
		Can0.setBigEndian(bigEndian);
        Can0.setNumTXBoxes(txMobCount);
		isInitialized = true;
		
        //Can library is interrupt driven, so turn on interrupts
        sei();
	}
	
	
	return isInitialized;
}



/**
 * @brief Sends all bytes through the can bus
 * @param bytes The bytes to send
 * @param id The ID to send
 * @param priority The priority of the send
 * @return true If frame was able to be loaded
 * @return false If frame was NOT able to be loaded
 */
bool CAN_send(uint8_t bytes[], uint32_t id, uint8_t priority) {
	
	//Variables
	bool hasWorked = true;
	uint8_t index = 0; //Index for loops
	volatile uint8_t subIndex = 0; //Index for loops
	
	
	//Loop through while the value at index is not null
	while(bytes[index] != '\0' && hasWorked == true) {
		hasWorked = CAN_send_int((uint32_t)bytes[index],id,priority);
		if(hasWorked == false) {
			subIndex++;
		}
		
		index++;
	}

	return hasWorked;
}



/**
 * @brief Sends a char through the can bus
 * 
 * 
 * @param value The value to send
 * @param id The id for the frame
 * @param priority The priority of the frame
 * @return true If frame was able to be loaded
 * @return false If frame was NOT able to be loaded
 */
bool CAN_send_char(uint8_t value, uint32_t id, uint8_t priority) {
	
	//Variables
	CAN_FRAME outgoing; //The frame that's being sent
	
	outgoing.id = id; //Set the id for the frame
	outgoing.extended = false; //Set extended to false
	outgoing.priority = priority; //0-15 lower is higher priority
	outgoing.length = 1; //Set the length of the value.
	outgoing.data.value = (uint64_t)value; //Set the outgoing data to the value passed

	//Return the status of loading the frame.
	return(Can0.sendFrame(outgoing));
}



/**
 * @brief Sends a short through the can bus
 * 
 * 
 * @param value The value to send
 * @param id The id for the frame
 * @param priority The priority of the frame
 * @return true If frame was able to be loaded
 * @return false If frame was NOT able to be loaded
 */
bool CAN_send_short(uint16_t value, uint32_t id, uint8_t priority) {
	
	//Variables
	CAN_FRAME outgoing; //The frame that's being sent
	
	outgoing.id = id; //Set the id for the frame
	outgoing.extended = false; //Set extended to false
	outgoing.priority = priority; //0-15 lower is higher priority
	outgoing.length = 2; //Set the length of the value.
	outgoing.data.value = (uint64_t)value; //Set the outgoing data to the value passed

	//Return the status of loading the frame.
	return(Can0.sendFrame(outgoing));
}



/**
 * @brief Sends an integer through the can bus
 * 
 * 
 * @param value The value to send
 * @param id The id for the frame
 * @param priority The priority of the frame
 * @return true If frame was able to be loaded
 * @return false If frame was NOT able to be loaded
 */
bool CAN_send_int(uint32_t value, uint32_t id, uint8_t priority) {
	
	//Variables
	CAN_FRAME outgoing; //The frame that's being sent
	
	outgoing.id = id; //Set the id for the frame
	outgoing.extended = false; //Set extended to false
	outgoing.priority = priority; //0-15 lower is higher priority
	outgoing.length = 4; //Set the length of the value.
	outgoing.data.value = (uint64_t)value; //Set the outgoing data to the value passed

	//Return the status of loading the frame.
	return(Can0.sendFrame(outgoing));
}


/**
 * @brief Sends a long through the can bus
 * 
 * 
 * @param value The value to send
 * @param id The id for the frame
 * @param priority The priority of the frame
 * @return true If frame was able to be loaded
 * @return false If frame was NOT able to be loaded
 */
bool CAN_send_long(uint64_t value, uint32_t id, uint8_t priority) {
	
	//Variables
	CAN_FRAME outgoing; //The frame that's being sent
	
	outgoing.id = id; //Set the id for the frame
	outgoing.extended = false; //Set extended to false
	outgoing.priority = priority; //0-15 lower is higher priority
	outgoing.length = 8; //Set the length of the value.
	outgoing.data.value = value; //Set the outgoing data to the value passed

	//Return the status of loading the frame.
	return(Can0.sendFrame(outgoing));
}



/**
 * @brief Processes any recieved can frame and returns the state of processing
 * 
 * @param can_frame_callback The function to call to process the can data. The callback function must return int8_t and the arguments must be passed by reference
 * @return int8_t The state of processing. if -1, there was no can available. if -2, the can frame could not be read, 
 * else it will be the state returned by the frame callback
 */
int8_t CAN_process_frame(int8_t(*can_frame_callback)(CAN_FRAME&)) {
	
	//Variables
	int8_t processedFrameState = -1; //If the frame has been processed or not.

	//Check if the Mobs have anything available...
	if (Can0.available() > 0) {	
			
		//If they did...
		CAN_FRAME incoming;

		//Try to read the frame...
		if (Can0.read(incoming)) {
			
			//If the frame was read, call the callback function
			processedFrameState = can_frame_callback(incoming);
		}
		else  //If it could not be read...
		{
			//Set the frame state to the could not read value
			processedFrameState = -2;
		}
		
	}

	return processedFrameState;
}



#endif
#endif /* __AVR_CAN_UTILITIES_CPP__ */
#endif