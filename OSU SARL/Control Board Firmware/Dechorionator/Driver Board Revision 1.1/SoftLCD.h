/* 
* SoftLCD.h
*
* Created: 7/31/2014 11:20:02 AM
* Author: corwin
*/


#ifndef __SOFTLCD_H__
#define __SOFTLCD_H__
#define F_CPU 16000000UL

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "DefineMacrosAndPinDefs.h"

#define REFRESH_RATE_CONSTANT (16000000ul/1024)

#define BAUD 9600
#define STOP_BITS 1

#define MICROSECONDS_OVERHEAD_ADJUST 0
#define MICROSECONDS_PER_BIT ((1000000ul / BAUD) - MICROSECONDS_OVERHEAD_ADJUST)

class SoftLCD
{
//variables
public:
protected:
private:
	FILE static usartout;

	volatile uint8_t *serPort;
	volatile uint8_t *serDDR;
	int serPin;
	
	volatile uint8_t *pwrPort;
	volatile uint8_t *pwrDDR;
	int pwrPin;
	
	char TopLine[17];
	char BottomLine[17];
	
	char TopLinePrev[17];
	char BottomLinePrev[17];
//functions
public:
	SoftLCD(volatile uint8_t *txport, volatile uint8_t *txddr, int txpin, volatile uint8_t *pwrport, volatile uint8_t *pwrddr, int pwrpin);
	
	void disableSparkfunLogo();
	void showNameScreen();
	
	void putChar(char c);
	void setLCDOn(bool on);
	void setRefreshRate(int rate);
	void show();
	
	void setTopText(char *top);
	void setBottomText(char *bottom);
	void setTopText(const char *top);
	void setBottomText(const char *bottom);
	
	~SoftLCD();
protected:
private:
	SoftLCD(const SoftLCD & c);
	SoftLCD& operator=( const SoftLCD &c );
	void _printLine(char *text);
	void _moveToTop();
	void _moveToBottom();

}; //SoftLCD

#endif //__SOFTLCD_H__
