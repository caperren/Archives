/* 
* SoftLCD.cpp
*
* Created: 7/31/2014 11:20:02 AM
* Author: corwin
*/


#include "SoftLCD.h"

// default constructor
SoftLCD::SoftLCD(volatile uint8_t *txport, volatile uint8_t *txddr, int txpin, volatile uint8_t *pwrport, volatile uint8_t *pwrddr, int pwrpin)
{
	//Set up public variables
	//strncpy(TopLine, "Test", 16);
	snprintf(TopLine, 16, "                ");
	snprintf(BottomLine, 16,  "                ");
	
	strcpy(TopLinePrev, TopLine);
	strcpy(BottomLinePrev, BottomLine);
	
	//Set up private variables
	serPort = txport;
	serDDR = txddr;
	serPin = txpin;
	
	pwrPort = pwrport;
	pwrDDR = pwrddr;
	pwrPin = pwrpin;
	
	//Set up tx pin for transmission
	*serDDR |= (1 << serPin);
	*serPort |= (1 << serPin);
	
	//Set up power control pin for output
	*pwrDDR |= (1 << pwrPin);
	setLCDOn(true);	//On by default
	putChar(124);
	putChar(157);
	
	setRefreshRate(5);	//Keep at 10fps or lower to keep screen looking nice. 5 fps seems like a good middle ground

	
} //SoftLCD

// default destructor
SoftLCD::~SoftLCD()
{
} //~SoftLCD

void SoftLCD::putChar(char c) {
	//TCCR1B &= ~((1 << CS10) | (1 << CS11) | (1 << CS12));
	//Start bit
	*serPort &= ~(1 << serPin);									//Write a zero for the start bit
	_delay_us(MICROSECONDS_PER_BIT);							//Wait one bit period

	//Begin data bits
	for (uint8_t bit_mask = 0x01; bit_mask ; bit_mask <<= 1) {	//Move through each bit in the char starting with the least significant bit
		if (c & bit_mask) {										//If that position contains a 1
			*serPort |= (1 << serPin);							//Write a 1 to the tx pin
		}else {													//Otherwise...
			*serPort &= ~(1 << serPin);							//Write a 0 to the tx pin
		}
		_delay_us(MICROSECONDS_PER_BIT);						//Delay for the correct period of time for one bit
	}													

	//Stop bit(s)
	*serPort |= (1<<serPin);									//Write a one for the stop bit
	_delay_us(MICROSECONDS_PER_BIT * STOP_BITS);				//Wait the period of time required for the number of stop bits desired
	//TCCR1B |= ((1 << CS10) | (1 << CS11));
}


void SoftLCD::setLCDOn(bool on)
{
	if(on == true){
		*pwrPort |= (1 << pwrPin);
	}else if(on == false){
		*pwrPort &= ~(1 << pwrPin);
	}
}

void SoftLCD::setRefreshRate(int rate)
{/*
	TCCR0A = 0;
	TCCR0B = ((1 << CS10) | (1 << CS12));
	TCNT0 = 0;
	TIMSK0 = (1 << TOIE0);
*/
}

void SoftLCD::show()
{
	if(strcmp(TopLine, TopLinePrev) != 0){
		_moveToTop();
		_printLine((char *)"                ");
		_moveToTop();
		_printLine(TopLine);
		strcpy(TopLinePrev, TopLine);
	}
	
	if(strcmp(BottomLine, BottomLinePrev) != 0){
		_moveToBottom();
		_printLine((char *)"                ");
		_moveToBottom();
		_printLine(BottomLine);
		strcpy(BottomLinePrev, BottomLine);
	}
	
}

void SoftLCD::_moveToTop()
{
	putChar(254);	//Puts lcd into command mode
	putChar(128);	//Moves to first character on top
}

void SoftLCD::_moveToBottom()
{
	putChar(254);	//Puts lcd into command mode
	putChar(192);	//Moves to first character on bottom
}

void SoftLCD::_printLine(char *text)
{
	for(int i = 0 ; ((i < 16) && (text[i] != '\0')) ; i++){
		putChar(text[i]);
	}
}

void SoftLCD::setTopText(char *top)
{
	snprintf(TopLine, 16, top);
}

void SoftLCD::setTopText(const char *top)
{
	snprintf(TopLine, 16, top);
}

void SoftLCD::setBottomText(char *bottom)
{
	snprintf(BottomLine, 16, bottom);
}

void SoftLCD::setBottomText(const char *bottom)
{
	snprintf(BottomLine, 16, bottom);
}

void SoftLCD::showNameScreen()
{
	_delay_ms(2000);						//Delay for LCD
	setTopText(" Dechorionator");		//Startup Text Top
	setBottomText(" Revision 2.0");		//Startup Text Bottom
	show();
	_delay_ms(2750);						//Pause for effect
}

void SoftLCD::disableSparkfunLogo()
{
	putChar(124);
	putChar(9);
}
