/**
 * \file spi.h
 * \author Tim Robbins - R&D Engineer, Atech Training
 * \brief Header file for SPI functions
 * \version v2.0
 */ 
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>
#include <stdbool.h>



///Selects a child node by pulling its select pin low     
#define SPI_CHILD_SELECT(childSelectPort, childSelectPin)    childSelectPort &= ~(1 << childSelectPin)    

///Deselects a child node
#define SPI_CHILD_DESELECT(childSelectPort, childSelectPin)    childSelectPort |= (1 << childSelectPin) 
    
#if defined(__AVR)

#include <avr/io.h>

#ifdef PRR0
	#define SpiEnablePower() PRR0 &= ~(1 << PRSPI)
	#define SpiDisablePower() PRR0 |= (1 << PRSPI)
#else
	#ifdef PRR
		#define SpiEnablePower() PRR &= ~(1 << PRSPI)
		#define SpiDisablePower() PRR |= (1 << PRSPI)
	#endif
#endif

///Enables the SPI module by setting the enable-bit in the SPI control register
#define SpiEnable()	    SPCR |= (1 << SPE)

///Disables the SPI module by clearing the enable-bit in the SPI control register
#define SpiDisable()	SPCR &= ~(1 << SPE)

///Clears the spi and closes connection
#define SpiClose()		SPCR = 0

///Sets the enable bit for the SPI
#define SpiOpen()		SPCR |= ((1 << SPE) )




//Contains register for another SPI
#ifdef SPIPS

///Sets the bit that selects alt spi port(MISO_A, MOSI_A, SCK_A, SS_A)
#define SpiSelectAlt()      MCUCR |= (1 << SPIPS)

///Clears the bit that selects alt spi port(MISO_A, MOSI_A, SCK_A, SS_A)
#define SpiSelectNorm()	    MCUCR &= ~(1 << SPIPS)



extern void SpiInitAltParent(uint8_t clockRate, bool msbFirst, bool useSSAsInput);
extern void SpiInitAltChild();

#endif



#ifdef SPDR
	extern void SpiInitParent(uint8_t clockRate, bool msbFirst, bool useSSAsInput);
	extern void SpiInitChild();
	extern void SpiTransmit(uint8_t data);
	extern unsigned char SpiExchangeByte(unsigned char transmissionByte);
	extern unsigned char SpiExchangeByteOrTimeout(unsigned char transmissionByte, uint16_t timeoutCount);
	extern uint8_t SpiReceive();
	extern uint8_t SpiTransmitOrTimeout(uint8_t data, uint16_t timeoutCount);
	extern uint8_t SpiReceiveOrTimeout(uint16_t timeoutCount);
	extern uint8_t SpiRead();
	extern void SpiWrite(uint8_t data);
#endif

#elif defined(__XC)

#include <xc.h>        /* XC8 General Include File */

    
    typedef enum _SPI_PARENT_MODE_SETTINGS {
        
        SPI_PARENT_FOSC4 = 0b0000,
        SPI_PARENT_FOSC16 = 0b0001,
        SPI_PARENT_FOSC64 = 0b0010,
        SPI_PARENT_CLK_TMR2OUT = 0b0011,
        SPI_PARENT_FOSC4_SSP_ADD = 0b1010
        
    } SPI_parent_modes_t;
    
    typedef enum _SPI_CHILD_MODE_SETTINGS {
        
        SPI_CHILD_SCK_CON_EN =    0b0100,
        SPI_CHILD_SCK_CON_DISEN = 0b0101
        
    } SPI_child_modes_t;

#ifdef SSPBUF

#define SPI_DATA_REG       SSPBUF
#define SPI_BUFFER_BUSY    SSPSTATbits.BF
#define SPI_FLAG           PIR3bits.SSPIF
#define SpiIsBufferBusy()  (SSPSTATbits.BF != 0)
#define SpiIsFlagClear()   (PIR3bits.SSPIF == 0)
#define SpiOpen()          SSPCON1bits.SSPEN = 1
#define SpiClose()         SSPCON1bits.SSPEN = 0
#define SpiEnable()        SSPCON1bits.SSPEN = 1
#define SpiDisable()       SSPCON1bits.SSPEN = 0

extern void SpiParentInit(SPI_parent_modes_t parentModeSettings, bool doesClockIdleLow, uint8_t baudRateDivider);
extern void SpiChildInit(SPI_child_modes_t childModeSettings, bool doesClockIdleLow);
extern unsigned char SpiExchangeByte(unsigned char transmissionByte);
extern void SpiTransmit(unsigned char dataByte);
extern unsigned char SpiReceive();
extern unsigned char SpiRead();
extern void SpiWrite(uint8_t byte);

#endif

#ifdef SSP1BUF

#define SPI1_DATA_REG       SSP1BUF
#define SPI1_BUFFER_BUSY    SSP1STATbits.BF
#define SPI1_FLAG           PIR3bits.SSP1IF
#define Spi1IsBufferBusy()  (SSP1STATbits.BF != 0)
#define Spi1IsFlagClear()   (PIR3bits.SSP1IF == 0)
#define Spi1Open()          SSP1CON1bits.SSPEN = 1
#define Spi1Close()         SSP1CON1bits.SSPEN = 0
#define Spi1Enable()        SSP1CON1bits.SSPEN = 1
#define Spi1Disable()       SSP1CON1bits.SSPEN = 0

extern void Spi1ParentInit(SPI_parent_modes_t parentModeSettings, bool doesClockIdleLow, uint8_t baudRateDivider);
extern void Spi1ChildInit(SPI_child_modes_t childModeSettings, bool doesClockIdleLow);
extern unsigned char Spi1ExchangeByte(unsigned char transmissionByte);
extern unsigned char Spi1Transmit(unsigned char dataByte);
extern unsigned char Spi1Receive();
extern unsigned char Spi1Read();
extern void Spi1Write(uint8_t byte);


#endif

#ifdef SSP2BUF

#define SPI2_DATA_REG       SSP2BUF
#define SPI2_BUFFER_BUSY    SSP2STATbits.BF
#define SPI2_FLAG           PIR3bits.SSP2IF
#define Spi2IsBufferBusy()  (SSP2STATbits.BF != 0)
#define Spi2IsFlagClear()   (PIR3bits.SSP2IF == 0)
#define Spi2Open()          SSP2CON1bits.SSPEN = 1
#define Spi2Close()         SSP2CON1bits.SSPEN = 0
#define Spi2Enable()        SSP2CON1bits.SSPEN = 1
#define Spi2Disable()       SSP2CON1bits.SSPEN = 0

extern void Spi2ParentInit(SPI_parent_modes_t parentModeSettings, bool doesClockIdleLow, uint8_t baudRateDivider);
extern void Spi2ChildInit(SPI_child_modes_t childModeSettings, bool doesClockIdleLow);
extern unsigned char Spi2ExchangeByte(unsigned char transmissionByte);
extern void Spi2Transmit(unsigned char dataByte);
extern unsigned char Spi2Receive();
extern unsigned char Spi2Read();
extern void Spi2Write(uint8_t byte);

#endif


#elif defined(HI_TECH_C)

#include <htc.h>       /* HiTech General Include File */

#elif defined(__18CXX)

#include <p18cxxx.h>   /* C18 General Include File */

#elif (defined __XC8)

#include <xc.h>            /* XC8 General Include File */

#endif


#ifdef __cplusplus
}
#endif
#endif /* __SPI_H__ */