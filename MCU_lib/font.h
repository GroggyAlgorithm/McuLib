/**
 *  \file font.c
 *  \author Tim Robbins
 *  \brief Header file for fonts.
 */
#ifndef __FONT_H__
#define __FONT_H__	1


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__AVR)

#ifndef _AVR_IO_H_
#include <avr/io.h>
#endif

#include <avr/pgmspace.h>

extern const uint8_t ATECH_LOGO[] PROGMEM;
extern uint8_t BIG_NUMBERS[11][57];

#define FONT_BIG_NUM_WIDTH	24
#define FONT_BIG_NUM_HEIGHT 19


#elif defined(__XC)

#include <xc.h>

#elif defined(HI_TECH_C)

#include <htc.h>       /* HiTech General Include File */

#elif defined(__18CXX)

#include <p18cxxx.h>   /* C18 General Include File */

#elif (defined __XC8)

#include <xc.h>            /* XC8 General Include File */
    
#endif


#define ssd1306oled_font_A_length		95
#define ssd1306oled_font_A_char_length	6
extern char ssd1306oled_font_A[];
extern uint8_t AtechLogoA_3x19_Font[57];


#define ATECH_LOGO_WIDTH	80
#define ATECH_LOGO_HEIGHT	19

#define BIG_NUMS_WIDTH		24
#define BIG_NUMS_HEIGHT		19

#ifdef __cplusplus
}
#endif

#endif
