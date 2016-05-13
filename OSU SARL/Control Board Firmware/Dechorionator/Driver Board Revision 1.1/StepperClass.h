/* 
* StepperClass.h
*
* Created: 8/1/2014 5:13:57 PM
* Author: corwin
*
*	MSTEP	M2	M1	M0	DEC
*	1		0	0	0	0
*	2		0	0	1	1
*	4		0	1	0	2
*	8		0	1	1	3
*	16		1	0	0	4
*	32		1	0	1	5
*
*/


#ifndef __STEPPERCLASS_H__
#define __STEPPERCLASS_H__
#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <inttypes.h>

#define STEPPER_RPM_CONSTANT (16000000ul/1024)*60)

class StepperClass
{
//variables
public:
protected:
private:
	volatile uint8_t *enPort;
	volatile uint8_t *enDDR;
	int enPin;
	
	volatile uint8_t *dirPort;
	volatile uint8_t *dirDDR;
	int dirPin;
	
	volatile uint8_t *stepPort;
	volatile uint8_t *stepDDR;
	int stepPin;
	
	volatile uint8_t *faultPort;
	volatile uint8_t *faultDDR;
	int faultPin;
	
	volatile uint8_t *m0Port;
	volatile uint8_t *m0DDR;
	int m0Pin;
	
	volatile uint8_t *m1Port;
	volatile uint8_t *m1DDR;
	int m1Pin;
	
	volatile uint8_t *m2Port;
	volatile uint8_t *m2DDR;
	int m2Pin;

//functions
public:
	StepperClass(volatile uint8_t *enport, volatile uint8_t *enddr, int enpin,
				 volatile uint8_t *dirport, volatile uint8_t *dirddr, int dirpin,
				 volatile uint8_t *stepport, volatile uint8_t *stepddr, int steppin,
				 volatile uint8_t *faultport, volatile uint8_t *faultddr, int faultpin,
				 volatile uint8_t *m0port, volatile uint8_t *m0ddr, int m0pin,
				 volatile uint8_t *m1port, volatile uint8_t *m1ddr, int m1pin,
				 volatile uint8_t *m2port, volatile uint8_t *m2ddr, int m2pin
				 );
	void SetRPM(int rpm); 
	void OneStep();
				 
	~StepperClass();
protected:
private:
	StepperClass( const StepperClass &c );
	StepperClass& operator=( const StepperClass &c );
	void _setupPinAsOutput(volatile uint8_t *ddr, int pin);
	void _setupPinAsInput(volatile uint8_t *port, volatile uint8_t *ddr, int pin, bool enablePullup);

	void _enableStepper();
	void _disableStepper();
	
	void _setPinLow(volatile uint8_t *port, int pin);
	void _setPinHigh(volatile uint8_t *port, int pin);

}; //StepperClass

#endif //__STEPPERCLASS_H__
