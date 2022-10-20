/** 
 * \file spi.c
 * \author Tim Robbins - R&D Engineer, Atech Training
 * \brief Source file for SPI functions
 * \version v2.0
 * \todo PIC spi communication doesn't seem to be working.
 */ 
#include "spi.h"
#ifndef __SPI_C__
#define __SPI_C__

#include "mcuUtils.h"
#include "mcuPinUtils.h"

#if defined(__AVR)
#ifdef SPIPS
/**
 * \brief Initializes the chip as a parent/master for SPI
 * \param clockRate The clock rate for the SPI
 * \param msbFirst The endianess of values, True if the most significant bit is transmitted first
 * \param useSSAsInput If using the SS pin for SPI is input or not
 */
void SpiInitAltParent(uint8_t clockRate, bool msbFirst, bool useSSAsInput) 
{

	#ifdef PRR
		//Make sure that SPI is enabled in the power register
		PRR &= ~(1 << PRSPI);
	#else
		//Make sure that SPI is enabled in the power register
		PRR0 &= ~(1 << PRSPI);
	#endif

	SPI_select_alt();


	//Set the direction of the mosi and sck value to output, all others input
	PIN_OUTPUT(MOSI_A_PIN);
	PIN_OUTPUT(SCK_A_PIN);
	PIN_INPUT(MISO_A_PIN);

	if(useSSAsInput == true) {
		//The ss pin must be help high if using as in input for slave select. If it's pulled low,
		//It interprets this as another parent trying to to select this chip as a child..
		PIN_HIGH(SS_A_PIN);
		PIN_INPUT(SS_A_PIN);
	}

	//Set controls to Enable SPI as master, sets the bit for MSB or LSB to the one passed
	//And sets the value for the clock rate to the clock rate passed
	SPCR = ((1 << SPE) |( 1 << MSTR) | (msbFirst << DORD) | ((clockRate & 0x03) << SPR0));
}


/**
 * \brief Initializes the chip as a child/client for the ALT SPI
 */
void SpiInitAltChild() 
{

	#ifdef PRR
		//Make sure that SPI is enabled in the power register
		PRR &= ~(1 << PRSPI);
	#else
		//Make sure that SPI is enabled in the power register
		PRR0 &= ~(1 << PRSPI);
	#endif
	
	//Select the alt spi
	SPI_select_alt();

	//Set the directions of the SPI pins
	PIN_INPUT(MOSI_A_PIN);
	PIN_INPUT(SCK_A_PIN);
	PIN_OUTPUT(MISO_A_PIN);

	//As a child the SS pin will always be input. When low, spi is activated. When high, all pins are inputs and spi is set to passive.
	PIN_HIGH(SS_A_PIN);
	PIN_INPUT(SS_A_PIN);
	
	//Set controls to Enable SPI as child
	SPCR = ((1 << SPE));
}

#endif

#ifdef SPDR



/**
 * \brief Initializes the chip as a parent/master for SPI
 * \param clockRate The clock rate for the SPI
 * \param msbFirst The endianess of values, True if the most significant bit is transmitted first
 * \param useSSAsInput If using the SS pin for SPI is input or not
 */
void SpiInitParent(uint8_t clockRate, bool msbFirst, bool useSSAsInput) 
{

	#ifdef PRR
		//Make sure that SPI is enabled in the power register
		PRR &= ~(1 << PRSPI);
	#else
		//Make sure that SPI is enabled in the power register
		PRR0 &= ~(1 << PRSPI);
	#endif

	//If there's an alt port, Set it to use the normal SPI
	#ifdef SPIPS
		SpiSelectNorm();
	#endif


	//Set the direction of the mosi and sck value to output, all others input
	PIN_OUTPUT(MOSI_PIN);
	PIN_OUTPUT(SCK_PIN);
	PIN_INPUT(MISO_PIN);
	
	PIN_HIGH(SS_PIN);

	if(useSSAsInput == true) 
	{
		
		//The ss pin must be help high if using as in input for slave select. If it's pulled low,
		//It interprets this as another parent trying to to select this chip as a child.
		PIN_INPUT(SS_PIN);
	}
	else
	{
		PIN_OUTPUT(SS_PIN);
	}
	
	//Set controls to Enable SPI as master, sets the bit for MSB or LSB to the one passed
	//And sets the value for the clock rate to the clock rate passed
	SPCR = ((1 << SPE) |( 1 << MSTR) | (msbFirst << DORD) | ((clockRate & 0x03) << SPR0));
}



/**
 * \brief Initializes the chip as a child/client for SPI
 */
void SpiInitChild() 
{

	#ifdef PRR
		//Make sure that SPI is enabled in the power register
		PRR &= ~(1 << PRSPI);
	#else
		//Make sure that SPI is enabled in the power register
		PRR0 &= ~(1 << PRSPI);
	#endif

	//If there's an alt port, Set it to use the normal SPI
	#ifdef SPIPS
		SpiSelectNorm();
	#endif


	//Set the directions of the SPI pins
	PIN_INPUT(MOSI_PIN);
	PIN_INPUT(SCK_PIN);
	PIN_OUTPUT(MISO_PIN);

	//As a child the SS pin will always be input. When low, spi is activated. When high, all pins are inputs and spi is set to passive.
	PIN_HIGH(SS_PIN);
	PIN_INPUT(SS_PIN);


	//Set controls to Enable SPI as child
	SPCR = ((1 << SPE));
}



/**
 * \brief Sends char through SPI. From Atmel's data sheet
 * 
 * 
 * \param data - The char value to send
 */
void SpiTransmit(uint8_t data) 
{
    //Start transmission
    SPDR = data;

    //Wait for completed transmission
    while(!(SPSR & (1 << SPIF)));
}



/**
 * Send and receives a byte
 * \param transmissionByte
 * \return received byte
 */
unsigned char SpiExchangeByte(unsigned char transmissionByte)
{
	
    //Load data into the register
    SPDR = transmissionByte;
    
    //Wait for completed transmission
    while(!(SPSR & (1 << SPIF)));
    
    //Return the data register value
    return SPDR;
    
    
}



/**
 * Send and receives a byte
 * \param transmissionByte
 * \return received byte
 */
unsigned char SpiExchangeByteOrTimeout(unsigned char transmissionByte, uint16_t timeoutCount)
{
    //Load data into the register
    SPDR = transmissionByte;
    
    //Wait for completed transmission
    while(!(SPSR & (1 << SPIF)))
	{
		//Decrease the timeout, break when appropriate, if timeout error
		if(timeoutCount <= 0)
		{
			break;
		}
		else
		{
			timeoutCount--;
		}
	}
    
    //Return the data register value
    return SPDR;
    
    
}



/**
 * \brief Receives data through SPI. From Atmel's data sheet
 * 
 * 
 * \return uint8_t The data on the data register
 */
uint8_t SpiReceive() 
{
	
    //Wait for completed Reception
    while(!(SPSR & (1 << SPIF)));

    //Return data register
    return SPDR;
}



/**
 * \brief Sends char through SPI. From Atmel's data sheet. Times out after X counts
 * 
 * 
 * \param data - The value to send
 * \return uint8_t Status of transmission. 0 if good, 1 if timeout.
 */
uint8_t SpiTransmitOrTimeout(uint8_t data, uint16_t timeoutCount) 
{
	//Variables
	uint8_t status = 0;
	
    //Start transmission
    SPDR = data;

    //Wait for completed transmission
    while( (!(SPSR & (1 << SPIF)) && timeoutCount > 0 ))
    {
	    timeoutCount--;
    }

	if(timeoutCount == 0) status = 1;

	return status;
}



/**
 * \brief Receives a value through SPI. From Atmel's data sheet. Times out after X counts
 * 
 * 
 * \return uint8_t The data on the data register
 */
uint8_t SpiReceiveOrTimeout(uint16_t timeoutCount) 
{
	
    //Wait for completed Reception
    while( (!(SPSR & (1 << SPIF))) && timeoutCount > 0 )
	{
		timeoutCount--;
	}

    //Return data register
    return SPDR;
}



/**
 * Reads any byte on spi
 * \return The data done gotten
 */
uint8_t SpiRead() 
{
    
    //Return data register
    return SPDR;
}



/**
 * \brief Writes the passed data to the spi data register
 * 
 * 
 * \param data 
 */
void SpiWrite(uint8_t data)
{
    SPDR = data;
}


#endif


#elif defined(__XC)


#ifdef SSPBUF


/**
 * Initializes the controller as a parent
 * \param parentModeSettings The mode settings for the parent
 * \param doesClockIdleLow If the clock should idle low, set true, else false
 * \param baudRateDivider The divider for the sck pin. See data sheet
 */
void SpiParentInit(SPI_parent_modes_t parentModeSettings, bool doesClockIdleLow, uint8_t baudRateDivider)
{
    //Set the spi clock divider
    SSPADD = baudRateDivider; 
    
    //If the clock idles low, set to idle low else set to idle high
    if(doesClockIdleLow)
    {
        SSPCON1bits.CKP = 0;
    }
    else
    {
        SSPCON1bits.CKP = 1;
    }
    
    //Set the mode settings
    SSPCON1bits.SSPM = parentModeSettings;
}



/**
 * Initializes the controller as a child
 * \param childModeSettings The mode settings for the child
 * \param doesClockIdleLow If the clock should idle low, set true, else false
 * \param baudRateDivider The divider for the sck pin. See data sheet
 */
void SpiChildInit(SPI_child_modes_t childModeSettings, bool doesClockIdleLow)
{
    
    //If the clock idles low, set to idle low else set to idle high
    if(doesClockIdleLow)
    {
        SSPCON1bits.CKP = 0;
    }
    else
    {
        SSPCON1bits.CKP = 1;
    }
    
    //Set the mode settings
    SSPCON1bits.SSPM = childModeSettings;
}



/**
 * Send and receives a byte
 * \param transmissionByte
 * \return received byte
 */
unsigned char SpiExchangeByte(unsigned char transmissionByte)
{
    //Load data into the register
    SPI_DATA_REG = transmissionByte;
    
    //Wait for completed data exchange
    while(SpiIsFlagClear())
    {
        ;
    }
    
    //Return the data register value
    return SPI_DATA_REG;
    
    
}



/**
 * Sends a char through spi
 * \param dataByte the data to send
 */
void SpiTransmit(unsigned char dataByte) {
    
    //Load data into the register
    SSPBUF = dataByte;

    //Wait for completed transmission
    while(!SSPSTATbits.BF);
    
}



/**
 * Gets a byte through spi
 * \return The data done gotten
 */
unsigned char SpiReceive() {
    
    //The value received
    unsigned char receivedValue = 0;
    
    //Wait for completed Reception
    while(!PIR3bits.SSPIF);
    
    receivedValue = SSPBUF;
    
    PIR3bits.SSPIF = 0;

    //Return data register
    return receivedValue;
}


/**
 * Reads any byte on spi
 * \return The data done gotten
 */
unsigned char SpiRead() 
{
    
    //Return data register
    return SSPBUF;
}



/**
 * Just writes a byte to the buffer.
 * \param byte
 */
void SpiWrite(uint8_t byte)
{
    SSPBUF = byte;
}


#endif //End spi buff check





#ifdef SSP1BUF


/**
 * Initializes the controller as a parent
 * \param parentModeSettings The mode settings for the parent
 * \param doesClockIdleLow If the clock should idle low, set true, else false
 * \param baudRateDivider The divider for the sck pin. See data sheet
 */
void Spi1ParentInit(SPI_parent_modes_t parentModeSettings, bool doesClockIdleLow, uint8_t baudRateDivider)
{
    if(baudRateDivider > 0)
    {
        //Set the spi clock divider
        SSP1ADD = baudRateDivider; 
    }
    
    
    //If the clock idles low, set to idle low else set to idle high
    if(doesClockIdleLow)
    {
        SSP1CON1bits.CKP = 0;
    }
    else
    {
        SSP1CON1bits.CKP = 1;
    }
    
    //Set the mode settings
    SSP1CON1bits.SSPM = parentModeSettings;
}



/**
 * Initializes the controller as a child
 * \param childModeSettings The mode settings for the child
 * \param doesClockIdleLow If the clock should idle low, set true, else false
 * \param baudRateDivider The divider for the sck pin. See data sheet
 */
void Spi1ChildInit(SPI_child_modes_t childModeSettings, bool doesClockIdleLow)
{
    
    //If the clock idles low, set to idle low else set to idle high
    if(doesClockIdleLow)
    {
        SSP1CON1bits.CKP = 0;
    }
    else
    {
        SSP1CON1bits.CKP = 1;
    }
    
    //Sample selection is cleared in child modes
    SSP1STATbits.SMP = 0;
    
    //Set the mode settings
    SSP1CON1bits.SSPM = childModeSettings;
    
}



/**
 * Send and receives a byte
 * \param transmissionByte
 * \return received byte
 */
unsigned char Spi1ExchangeByte(unsigned char transmissionByte)
{
    //Load data into the register
    SSP1BUF = transmissionByte;
    
    //Wait for completed data exchange
    while(!PIR3bits.SSP1IF);
    
    PIR3bits.SSP1IF = 0;
    
    
    //Return the data register value
    return SSP1BUF;
    
    
}



/**
 * Sends a char through spi
 * \param dataByte the data to send
 */
unsigned char Spi1Transmit(unsigned char dataByte) 
{
    
    SSP1BUF = dataByte;
    
    while(!PIR3bits.SSP1IF)
    {
        ;
    }
    
    PIR3bits.SSP1IF = 0;
    
    //return to flush buffer
    return SSP1BUF;
}



/**
 * Gets a byte through spi
 * \return The data done gotten
 */
unsigned char Spi1Receive() 
{
    
    //The value received
    unsigned char receivedValue = 0;
    
    //Wait for completed Reception
    while(!PIR3bits.SSP1IF);
    
    receivedValue = SSP1BUF;
    
    PIR3bits.SSP1IF = 0;

    //Return data register
    return receivedValue;
}



/**
 * Reads any byte on spi
 * \return The data done gotten
 */
unsigned char Spi1Read() 
{
    
    //Return data register
    return SSP1BUF;
}



/**
 * Just writes a byte to the buffer.
 * \param byte
 */
void Spi1Write(uint8_t byte)
{
    SSP1BUF = byte;
}


#endif //End spi 1 buff check







#ifdef SSP2BUF

/**
 * Initializes the controller as a parent
 * \param parentModeSettings The mode settings for the parent
 * \param doesClockIdleLow If the clock should idle low, set true, else false
 * \param baudRateDivider The divider for the sck pin. See data sheet
 */
void Spi2ParentInit(SPI_parent_modes_t parentModeSettings, bool doesClockIdleLow, uint8_t baudRateDivider)
{
    //Set the spi clock divider
    SSP2ADD = baudRateDivider; 
    
    //If the clock idles low, set to idle low else set to idle high
    if(doesClockIdleLow)
    {
        SSP2CON1bits.CKP = 0;
    }
    else
    {
        SSP2CON1bits.CKP = 1;
    }
    
    //Set the mode settings
    SSP2CON1bits.SSPM = parentModeSettings;
}



/**
 * Initializes the controller as a child
 * \param childModeSettings The mode settings for the child
 * \param doesClockIdleLow If the clock should idle low, set true, else false
 * \param baudRateDivider The divider for the sck pin. See data sheet
 */
void Spi2ChildInit(SPI_child_modes_t childModeSettings, bool doesClockIdleLow)
{
    
    //If the clock idles low, set to idle low else set to idle high
    if(doesClockIdleLow)
    {
        SSP2CON1bits.CKP = 0;
    }
    else
    {
        SSP2CON1bits.CKP = 1;
    }
    
    //Set the mode settings
    SSP2CON1bits.SSPM = childModeSettings;
}



/**
 * Send and receives a byte
 * \param transmissionByte
 * \return received byte
 */
unsigned char Spi2ExchangeByte(unsigned char transmissionByte)
{
    //Load data into the register
    SPI2_DATA_REG = transmissionByte;
    
    //Wait for completed data exchange
    while(Spi2IsFlagClear())
    {
        ;
    }
    
    //Clear flag
    PIR3bits.SSP2IF = 0;
    
    //Return the data register value
    return SPI2_DATA_REG;
    
    
}



/**
 * Sends a char through spi
 * \param dataByte the data to send
 */
void Spi2Transmit(unsigned char dataByte) {
    
    //Load data into the register
    SPI2_DATA_REG = dataByte;

    //Wait for completed transmission
    while(!SSP2STATbits.BF);
    
    
}



/**
 * Gets a byte through spi
 * \return The data done gotten
 */
unsigned char Spi2Receive() {
    
    //The value received
    unsigned char receivedValue = 0;
    
    //Wait for completed Reception
    while(!PIR3bits.SSP2IF);
    
    receivedValue = SSP2BUF;
    
    PIR3bits.SSP2IF = 0;

    //Return data register
    return receivedValue;
}



/**
 * Reads any byte on spi
 * \return The data done gotten
 */
unsigned char Spi2Read() 
{
    //Return data register
    return SSP2BUF;
}



/**
 * Just writes a byte to the buffer.
 * \param byte
 */
void Spi2Write(uint8_t byte)
{
    SSP2BUF = byte;
}


#endif //end spi 2 buff check








#endif //end controller check

#endif //end file check