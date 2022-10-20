/**
 * \file ssd1306.h
 * \author Tim Robbins - R&D Engineer, Atech Training
 * \brief Header file for ssd1306 oled functions. If there are missing size definitions, defaults to 64x128. \n
 * REQUIREMENTS: \n
 * Requires "config.h" file with defined macros: SSD1306_I2C 1 if using i2c or SSD1306_SPI 1 if using spi \n
 * If using SPI, it is required to define SSD1306_CON_PIN_PORT, SSD1306_DC_PIN_POSITION, SSD1306_CS_PORT, SSD1306_CS_PIN_POSITIONS, and SSD1306_RES_PIN_POSITION. \n
 * OPTIONS: SSD1306_DRAW_IMMEDIATE for skipping buffer use and SSD1306_AUTO_CLEAR_BUFF_ON_UPDATE for auto clearing the buffer when updating the display \n
 * You can also specify SSD1306_WIDTH and SSD1306_HEIGHT for the size of the display. \n
 * Defining SSD1306_SHOW_ERRORS as 1 will have the errors display on the ide console \n
 * It is also required to have "spi.h" and/or "i2c.h" depending on the mode chosen. \n
 * as well as "mcuDelays.h", "mcuUtils.h, and mcuPinUtils" \n
 * EXAMPLE DEFINITIONS: \n
 * Having 2 displays: #define SSD1306_CS_PIN_POSITIONS 1, 2 else #define SSD1306_CS_PIN_POSITIONS 1 \n
 * #define SSD1306_SPI 1 OR SSD1306_I2C 1 \n
 * If using SPI... \n
 * #define SSD1306_CON_PIN_PORT PORTA \n
 * #define SSD1306_DC_PIN_POSITION 1 \n
 * #define SSD1306_RES_PIN_POSITION 2 \n
 * 
 * \version v2.0
 */ 
#ifndef __SSD1306_H__

#define __SSD1306_H__        1
#define __INCLUDED_SSD1306__ 1

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include <stdlib.h>
#include <stdbool.h>
#include "mcuDelays.h"
#include "mcuUtils.h"
#include "mcuPinUtils.h"

#if defined(__AVR)

#include <avr/io.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
    
#elif defined(__XC)

#include <xc.h>        /* XC8 General Include File */
    
#endif
    
#ifndef SSD1306_SPI
#define SSD1306_SPI 1
#endif
    
#ifndef SSD1306_I2C
#define SSD1306_I2C 0
#endif
    

    
#if SSD1306_SPI == 1
    #include "spi.h"

    #if !defined(SSD1306_CON_PIN_PORT)
        #undef __INCLUDED_SSD1306__
    #endif
    
    #if !defined(SSD1306_DC_PIN_POSITION)
        #undef __INCLUDED_SSD1306__
    #endif

    #if !defined(SSD1306_CS_PORT)
        #undef __INCLUDED_SSD1306__
    #endif

    #if !defined(SSD1306_CS_PIN_POSITIONS)
        #undef __INCLUDED_SSD1306__
    #endif

    #if !defined(SSD1306_RES_PIN_POSITION)
        #undef __INCLUDED_SSD1306__
    #endif
    

#define SSD1306_SET_DC() SSD1306_CON_PIN_PORT |= (1 << SSD1306_DC_PIN_POSITION)
#define SSD1306_SET_RES() SSD1306_CON_PIN_PORT |= (1 << SSD1306_RES_PIN_POSITION)

#define SSD1306_CLEAR_DC()  SSD1306_CON_PIN_PORT &= ~(1 << SSD1306_DC_PIN_POSITION)
#define SSD1306_CLEAR_RES() SSD1306_CON_PIN_PORT &= ~(1 << SSD1306_RES_PIN_POSITION)
    

    
#elif SSD1306_I2C == 1
    #include "i2c.h"
#else
    #undef __INCLUDED_SSD1306__
#endif


#if defined(__INCLUDED_SSD1306__)    
   
#define MAX_SSD1306_HEIGHT					64
    
#if !defined(SSD1306_HEIGHT) || SSD1306_HEIGHT > MAX_SSD1306_HEIGHT
#define SSD1306_HEIGHT						64
#endif
    
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH						128
#endif

#if SSD1306_HEIGHT == 32
    
#define SSD1306_ADDRESS 0x3C

#else
    
#define SSD1306_ADDRESS 0x3D    

#endif

#define SSD1306_CMD_DISPLAY_OFF             0xAE
#define SSD1306_CMD_DISPLAY_ON              0xAF
#define SSD1306_CMD_HORIZONTAL_SCROLL_RIGHT 0x26
#define SSD1306_CMD_HORIZONTAL_SCROLL_LEFT  0x27
#define SSD1306_CMD_SCROLL_VERT_AND_RIGHT   0x29
#define SSD1306_CMD_SCROLL_VERT_AND_LEFT    0x2A
#define SSD1306_CMD_DEACTIVATE_SCROLL       0x2E
#define SSD1306_CMD_ACTIVATE_SCROLL         0x2F
#define SSD1306_CMD_SET_VERT_SCROLL_AREA    0xA3
#define SSD1306_CMD_SEND_CMD				0x00
#define SSD1306_CMD_SEND_DATA				0x40
#define SSD1306_CMD_SET_CONTRAST			0x81
#define SSD1306_WHITE						0x01
#define SSD1306_BLACK						0x00
#define SSD1306_CMD_NORMAL_DISPLAY          0xA6
#define SSD1306_CMD_INVERT_DISPLAY          0xA7
#define SSD1306_CMD_DISPLAY_OFF             0xAE
#define SSD1306_CMD_DISPLAY_ON              0xAF
    



#define SSD1306_NORMALSIZE              1
#define SSD1306_DOUBLESIZE              2
#define SSD1306_LASTLINE                0x07
#define SSD1306_COMMAND                 0x00
#define SSD1306_DATA                    0xC0
#define SSD1306_DATA_CONTINUE           0x40
#define SSD1306_SET_CONTRAST_CONTROL    0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME   0xA4
#define SSD1306_DISPLAY_ALL_ON          0xA5
#define SSD1306_NOP                     0xE3
#define SSD1306_HORIZONTAL_SCROLL_RIGHT 0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT  0x27
#define SSD1306_SCROLL_VERT_AND_RIGHT   0x29
#define SSD1306_SCROLL_VERT_AND_LEFT    0x2A
#define SSD1306_DEACTIVATE_SCROLL       0x2E
#define SSD1306_ACTIVATE_SCROLL         0x2F
#define SSD1306_SET_VERT_SCROLL_AREA    0xA3
#define SSD1306_SET_LOWER_COLUMN        0x00
#define SSD1306_SET_HIGHER_COLUMN       0x10
#define SSD1306_MEMORY_ADDR_MODE        0x20
#define SSD1306_SET_COLUMN_ADDR         0x21
#define SSD1306_SET_PAGE_ADDR           0xB0
#define SSD1306_SET_START_LINE          0x40
#define SSD1306_SET_SEGMENT_REMAP       0xA0
#define SSD1306_SET_MULTIPLEX_RATIO     0xA8
#define SSD1306_COM_SCAN_DIR_INC        0xC0
#define SSD1306_COM_SCAN_DIR_DEC        0xC8
#define SSD1306_SET_DISPLAY_OFFSET      0xD3
#define SSD1306_SET_COM_PINS            0xDA
#define SSD1306_CHARGE_PUMP             0x8D
#define SSD1306_SET_CLOCK_DIV_RATIO     0xD5
#define SSD1306_SET_PRECHARGE_PERIOD    0xD9
#define SSD1306_SET_VCOM_DESELECT       0xDB
    



extern void SSD1306SelectDisplay(uint8_t display);
extern void SSD1306Initialize(bool displayOn, uint8_t currentDisplaySelection);
extern void SSD1306SendCommand(uint8_t cmd);
extern void SSD1306SendMoreCommands(uint8_t* cmd);
extern void SSD1306SendData(uint8_t data);
extern void SSD1306SendMoreData(uint8_t* data);
extern void SSD1306SendCommandArray(uint8_t cmds[], uint16_t cmdlen);
extern void SSD1306SendDataArray(uint8_t data[], uint16_t datalen);
extern void SSD1306ClearScreen();
extern void SSD1306StopScroll();
extern void SSD1306SetInvert(bool invert);
extern void SSD1306SetSleep(bool gotoSleep);
extern void SSD1306SetContrast(uint8_t contrast);
extern void SSD1306StartScrollRight(uint8_t start, uint8_t stop);
extern void SSD1306StartScrollLeft(uint8_t start, uint8_t stop);
extern void SSD1306GoToPosition(uint8_t x, uint8_t y, uint8_t fontSize);
extern void SSD1306GoToPixelPosition(uint8_t x, uint8_t y);
extern void SSD1306PutChar(char c);
extern void SSD1306PutString(char* s);

#if defined(__AVR)
extern void PutP(PGM_P progmemS);
#endif

extern void SSD1306PutFontChar(char c, const char fontSheet[], uint8_t fontSheetCharacterLength);

extern void SSD1306PutFontString
(

char* s,

uint8_t fontSheetCharacterLength,

char fontSheet[]

);

extern void SSD1306PutFontStringAtLocation(
char* s, 
uint8_t fontSheetCharacterLength,
char fontSheet[], 
uint8_t x, uint8_t y
);
extern void SSD1306WriteFontLine(const char fontSheet[], uint8_t fontSheetCharacterLength) ;
extern void SSD1306WriteFontToLocation(
uint8_t fontSheetCharacterLength,
uint8_t fontSheetCharacterWidth,
char fontSheet[],
uint8_t x, uint8_t y
);


extern uint8_t SSD1306DrawPixel(uint8_t x, uint8_t y, uint8_t color);
extern uint8_t SSD1306DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
extern uint8_t SSD1306DrawRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color);
extern uint8_t SSD1306FillRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color);
extern uint8_t SSD1306DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, uint8_t color);
extern uint8_t SSD1306FillCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, uint8_t color);
extern uint8_t SSD1306DrawBitmap(uint8_t x, uint8_t y, const uint8_t *picture, uint8_t width, uint8_t height, uint8_t color);


extern uint8_t SSD1306DrawArea(uint8_t x, uint8_t y, uint8_t *picture, uint8_t width, uint8_t height, uint8_t color);

#if !defined(SSD1306_DRAW_IMMEDIATE) || SSD1306_DRAW_IMMEDIATE < 1
extern void SSD1306WriteToBuffer(uint8_t data, unsigned char x, unsigned char y);
extern void SSD1306ClearBuffer();
extern void SSD1306Update();
extern void SSD1306UpdateAll() ;
extern uint8_t SSD1306CheckBuffer(uint8_t x, uint8_t y);

#endif








#else

#warning ssd1306.h: Check required macro definitions to include file.

#if defined(SSD1306_SHOW_ERRORS) && SSD1306_SHOW_ERRORS == 1


    
#if SSD1306_SPI == 1

    #if !defined(SSD1306_CON_PIN_PORT)
        #error ssd1306.h: SSD1306_CON_PIN_PORT must be defined
    #endif
    
    #if !defined(SSD1306_DC_PIN_POSITION)
       #error ssd1306.h: SSD1306_DC_PIN_POSITION must be defined
    #endif

    #if !defined(SSD1306_CS_PORT)
       #error ssd1306.h: SSD1306_CS_PORT must be defined
    #endif

    #if !defined(SSD1306_CS_PIN_POSITIONS)
       #error ssd1306.h: SSD1306_CS_PIN_POSITIONS must be defined with all cs pins on the cs port
    #endif

    #if !defined(SSD1306_RES_PIN_POSITION)
       #error ssd1306.h: SSD1306_RES_PIN_POSITION must be defined
    #endif
    

    


#else
    #if !defined(SSD1306_I2C) || SSD1306_I2C != 1
        #error ssd1306.h: SSD1306_I2C or SSD1306_SPI must be defined as 1 for mode selection
    #endif
#endif

#endif

#endif


#ifdef __cplusplus
}
#endif


#endif /* SSD1306_H_ */