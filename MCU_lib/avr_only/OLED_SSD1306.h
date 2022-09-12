/**
 * \file OLED_SSD1306.h
 * \author Tim Robbins
 * \brief Header file for SSD1306 OLED display
 * \todo - Fix display buffer sizing
 */

#define __NO_INCLUDE_OLED_CPP	1


#if defined(__cplusplus) && defined(__AVR) && !defined(__NO_INCLUDE_OLED_CPP)
#ifndef OLED_SSD1306_H_
#define OLED_SSD1306_H_



#include <avr/io.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "mcuUtils.h"
#include "mcuDelays.h"
#include "spi.h"
#include "i2c.h"
#include "font.h"

#define MAX_SSD1306_HEIGHT					64

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
#define SSD1306_NORMALSIZE 					1
#define SSD1306_DOUBLESIZE 					2




class OLED_SSD1306 {
	private:
	
	volatile uint8_t* controlPinPort;
	uint8_t chipSelectPin;
	uint8_t dcPin;
	uint8_t resetPin;
	
	#ifdef TWCR
	bool useI2C;
	#endif
	
	bool useGraphicMode;
	uint8_t charMode;
	uint8_t displayBuffer[8][128]; //Current max possible size, need to fix this
	uint16_t cursorPosX;
	uint16_t cursorPosY;
	
	uint8_t address;
	uint8_t screenWidth;
	uint8_t screenHeight;
	
	protected:
	

	
	public:
	

	OLED_SSD1306(volatile uint8_t* newControlPinPort, uint8_t csPinPosition, uint8_t dcPinPosition, uint8_t resetPinPosition
	#ifdef TWCR
	,bool useI2CTransmit
	#endif
	);
	OLED_SSD1306(volatile uint8_t* newControlPinPort, uint8_t csPinPosition, uint8_t dcPinPosition, uint8_t resetPinPosition,
	#ifdef TWCR
	bool useI2CTransmit,
	#endif
	bool newUseGraphicMode
	);
	OLED_SSD1306(volatile uint8_t* newControlPinPort, uint8_t csPinPosition, uint8_t dcPinPosition, uint8_t resetPinPosition,
	#ifdef TWCR
	bool useI2CTransmit,
	#endif
	bool newUseGraphicMode,
	uint8_t newCharMode
	);
	

	~OLED_SSD1306();
	

	void Initialize(bool displayOn,
		uint8_t newScreenWidth, uint8_t newScreenHeight
	);
	
	void SendCommand(uint8_t cmd);
	void SendCommand(uint8_t* cmd);
	void SendCommand(uint8_t cmd[], uint16_t size);
	void SendData(uint8_t data);
	void SendData(uint8_t *data);
	void SendData(uint8_t data[], uint16_t size);
	
	inline void SetGraphicMode(bool graphicMode) {
		useGraphicMode = graphicMode;
	}

	inline void StopScroll() {
		SendCommand(SSD1306_CMD_DEACTIVATE_SCROLL);
	}
	
	void ClearScreen();
	void GoToPosition(uint8_t x, uint8_t y, uint8_t fontSize);
	void GoToPixelPosition(uint8_t x, uint8_t y);
	
	void SetInvert(bool invert);
	void SetSleep(bool gotoSleep);
	void SetContrast(uint8_t contrast);
	
	
	void PrintText(char c, const char fontSheet[], uint8_t fontSheetCharacterLength);

	void Put(char c);
	void Put(char* s);
	void Put(PGM_P progmemS);
	
	void Put(char c, const char fontSheet[], uint8_t fontSheetCharacterLength);
	void Put(char* s, const char fontSheet[], uint8_t fontSheetCharacterLength);
	void Put(const char* s, const char fontSheet[], uint8_t fontSheetCharacterLength);

	void SetCharMode(uint8_t mode);
	void StartScrollRight(uint8_t start, uint8_t stop);
	void StartScrollLeft(uint8_t start, uint8_t stop);
	
	uint8_t DrawPixel(uint8_t x, uint8_t y, uint8_t color);
	uint8_t DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
	uint8_t DrawRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color);
	uint8_t FillRect(uint8_t px1, uint8_t py1, uint8_t px2, uint8_t py2, uint8_t color);
	uint8_t DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, uint8_t color);
	uint8_t FillCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, uint8_t color);
	uint8_t DrawBitmap(uint8_t x, uint8_t y, const uint8_t *picture, uint8_t width, uint8_t height, uint8_t color);
	void Update();
	void ClearBuffer();
	uint8_t CheckBuffer(uint8_t x, uint8_t y);
	void DisplayBlock(uint8_t x, uint8_t y, uint8_t width);

};



#endif 
#endif 