/* 
* StepperClass.cpp
*
* Created: 8/1/2014 5:13:57 PM
* Author: corwin
*/


#include "StepperClass.h"

// default constructor
StepperClass::StepperClass(volatile uint8_t *enport, volatile uint8_t *enddr, int enpin,
						   volatile uint8_t *dirport, volatile uint8_t *dirddr, int dirpin,
						   volatile uint8_t *stepport, volatile uint8_t *stepddr, int steppin,
						   volatile uint8_t *faultport, volatile uint8_t *faultddr, int faultpin,
						   volatile uint8_t *m0port, volatile uint8_t *m0ddr, int m0pin,
						   volatile uint8_t *m1port, volatile uint8_t *m1ddr, int m1pin,
						   volatile uint8_t *m2port, volatile uint8_t *m2ddr, int m2pin
						   )
{
	enPort = enport;
	enDDR = enddr;
	enPin = enpin;
	_setupPinAsOutput(enDDR, enPin);
	_disableStepper();
	
	dirPort = dirport;
	dirDDR = dirddr;
	dirPin = dirpin;
	_setupPinAsOutput(enDDR, enPin);
	_setPinLow(dirPort, dirPin);
	
	stepPort = stepport;
	stepDDR = stepddr;
	stepPin = steppin;
	_setupPinAsOutput(enDDR, enPin);
	_setPinLow(stepPort, stepPin);
	
	faultPort = faultport;
	faultDDR = faultddr;
	faultPin = faultpin;
	_setupPinAsInput(faultPort, faultDDR, faultPin, false);
	
	m0Port = m0port;
	m0DDR = m0ddr;
	m0Pin = m0pin;
	_setupPinAsOutput(enDDR, enPin);
	_setPinHigh(m0Port, m0Pin);
	
	m1Port = m1port;
	m1DDR = m1ddr;
	m1Pin = m1pin;
	_setupPinAsOutput(enDDR, enPin);
	_setPinLow(m1Port, m1Pin);
	
	m2Port = m2port;
	m2DDR = m2ddr;
	m2Pin = m2pin;
	_setupPinAsOutput(enDDR, enPin);
	_setPinLow(m2Port, m2Pin);
	
	
	TCCR1A = 0;												//Set output compare mode for channel A and B to normal operation and waveform generation to normal
	TCCR1B = ((1 << WGM12) | (1 << CS10) | (1 << CS11));	//Set clk prescalar for timer to clk/1024 and enable clear on timer compare match
	TCNT1 = 0;												//Zero the count register
	TIMSK1 = (1 << OCIE1A);									//Set interrupt flag to trigger on counter 1 overflow
	OCR1A = 2000;				//Set the count value based on what's needed to update at a specific number of fps	
	
} //StepperClass

// default destructor
StepperClass::~StepperClass()
{
} //~StepperClass

void StepperClass::_setupPinAsOutput(volatile uint8_t *ddr, int pin)
{
	*ddr |= (1 << pin);
}

void StepperClass::_setupPinAsInput(volatile uint8_t *port, volatile uint8_t *ddr, int pin, bool enablePullup = false)
{
	*ddr &= ~(1 << pin);
	if(enablePullup){
		*port |= (1 << pin);
	}
}

void StepperClass::_enableStepper()
{
	*enPort &= ~(1 << enPin);
}

void StepperClass::_disableStepper()
{
	*enPort |= (1 << enPin);
}

void StepperClass::_setPinLow(volatile uint8_t *port, int pin)
{
	*port &= ~(1 << pin);
}

void StepperClass::_setPinHigh(volatile uint8_t *port, int pin)
{
	*port |= (1 << pin);
}

void StepperClass::OneStep()
{
	_setPinHigh(stepPort, stepPin);
	_delay_us(2);
	_setPinLow(stepPort, stepPin);
}

void StepperClass::SetRPM(int rpm)
{
	OCR1B = 35000;//(long(STEPPER_RPM_CONSTANT)/rpm);				//Set the count value based on what's needed to update at a specific number of fps
}
