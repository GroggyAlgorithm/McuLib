/**
 *  \file font.c
 *  \author Tim Robbins
 *  \brief Header file for fonts
 */
#ifndef __FONT_H__
#define __FONT_H__


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__AVR)

#ifndef _AVR_IO_H_
#include <avr/io.h>
#endif

#include <avr/pgmspace.h>

extern const uint8_t ATECH_LOGO[] PROGMEM;


#define FONT_BIG_NUM_WIDTH	60
#define FONT_BIG_NUM_HEIGHT 38

extern const uint8_t BIG_0[] PROGMEM;
//extern const uint8_t BIG_1[] PROGMEM;
//extern const uint8_t BIG_2[] PROGMEM;
//extern const uint8_t BIG_3[] PROGMEM;
//extern const uint8_t BIG_4[] PROGMEM;
//extern const uint8_t BIG_5[] PROGMEM;
//extern const uint8_t BIG_6[] PROGMEM;
//extern const uint8_t BIG_7[] PROGMEM;
//extern const uint8_t BIG_8[] PROGMEM;
//extern const uint8_t BIG_9[] PROGMEM;

#elif defined(__XC)

#include <xc.h>

#elif defined(HI_TECH_C)

#include <htc.h>       /* HiTech General Include File */

#elif defined(__18CXX)

#include <p18cxxx.h>   /* C18 General Include File */

#elif (defined __XC8)

#include <xc.h>            /* XC8 General Include File */
    
#endif


#define ssd1306oled_font_A_char_length	6
extern const char ssd1306oled_font_A[];


#define ATECH_LOGO_WIDTH	80
#define ATECH_LOGO_HEIGHT	19



#ifdef __cplusplus
}
#endif

#endif
