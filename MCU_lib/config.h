/**
 * \file config.h
 * \author Tim Robbins
 * \brief The project mcu_lib configuration file for mcu_lib definitions.
 */
#ifndef CONFIG_H_
#define CONFIG_H_   1




#if defined(__XC)




///The frequency being used for the controller
#define _XTAL_FREQ                                  64000000


#else


///The frequency being used for the controller
#define F_CPU                                       12000000UL

///The frequency being used for the controller. Using internal 8MHz
//#define F_CPU                                       8000000UL



#endif























































///The output register for the SPI pins
#define SPI_OUTPUT_REGISTER                             LATC

///The direction register for the spi pins
#define SPI_DIR_REGISTER                                TRISC

///The read register for the spi pins
#define SPI_READ_REGISTER                               PORTC



///The output register for the spi child select pin on child nodes
#define SPI_CHILD_CS_OUTPUT_REGISTER                    LATA

///The direction register for the spi child select pin on child nodes
#define SPI_CHILD_CS_DIR_REGISTER                       TRISA

///The read register for the spi child select pin on child nodes
#define SPI_CHILD_CS_READ_REGISTER                      PORTA



///SPI clock pin position
#define SPI_SCK_PIN_POSITION                            0

///Data in pin position
#define SPI_SDI_PIN_POSITION                            1

///Data out pin position
#define SPI_SDO_PIN_POSITION                            2

///Chip select pin position
#define SPI_CS_PIN_POSITION                             3

///SPI interrupt control register for enabling interrupts on low for the CS pin
#define SPI_CS_LOW_INT_EN                               IOCEN

///Chip select pin bit in the interrupt control register
#define SPI_CS_LOW_INT_BIT                              3



#if defined(__XC)

///Weak pull up for SPI
#define SPI_WPU_REGISTER                                WPUC

///peripheral selects
#define SPI_SCK_PIN_PPS                                 RC0PPS

///peripheral selects
#define SPI_SCK_PPS                                     SSP1CLKPPS
#define SPI_SCK_PPS_BITS                                SSP1CLKPPSbits

#define SPI_SCK_PPS_PORT_SEL                            0b10

#define SPI_SCK_PPS_PIN_SEL                             0b000

///peripheral selects
#define SPI_SDI_PIN_PPS                                 RC1PPS

#define SPI_SDI_PPS_PORT_SEL                            0b10
#define SPI_SDI_PPS_PIN_SEL                             0b001

///peripheral selects
#define SPI_SDI_PPS                                     SSP1DATPPS
#define SPI_SDI_PPS_BITS                                SSP1DATPPSbits

///peripheral selects
#define SPI_SDO_PPS                                     RC2PPS

///peripheral selects
#define SPI_CS_PIN_PPS                                  RC3PPS

///peripheral selects
#define SPI_CS_PPS                                      SSP1SSPPS
#define SPI_CS_PPS_BITS                                 SSP1SSPPSbits
#define SPI_CS_PPS_PORT_SEL                             0b10
#define SPI_CS_PPS_PIN_SEL                              0b011



///The spi selection channel for the chip select pin
#define SPI_PPS_CS_CHANNEL                              0x13

///The spi selection channel for the SDO pin
#define SPI_SDO_PPS_PIN_CHANNEL                         0x10

///The spi selection channel for the SDO
//#define SPI_SDO_PPS_CHANNEL                             0x12

///SPI channel value for setting SDI pin
#define SPI_PPS_SDI_CHANNEL                             0x11

///SPI channel value for setting SDI
#define SPI_PPS_SDI_PIN_CHANNEL                         0x11



///The spi selection channel for the SCK pin
#define SPI_SCK_PIN_CHANNEL                             0x0F

///The spi selection channel for the SCK
#define SPI_SCK_CHANNEL                                 0x10







#endif


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */



#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

