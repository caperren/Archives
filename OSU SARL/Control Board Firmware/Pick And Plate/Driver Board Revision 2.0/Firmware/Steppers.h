/*
 * Steppers.h
 *
 * Created: 8/9/2014 1:10:58 PM
 *  Author: corwin
 */ 


#ifndef STEPPERS_H_
#define STEPPERS_H_

#define  F_CPU 32000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "DefinesAndMacros.h"

#define XLEFT		0
#define XRIGHT		1
#define YFORWARDS	1
#define YBACKWARDS	0
#define ZUP			0
#define ZDOWN		1

#define XSTEPPERMM  250
#define YSTEPERMM   250
#define ZSTEPPERMM	323
#define ASTEPPERMM  250	//Needs to be found

typedef struct Axes{
	PORT_t *dirPort;
	int dirPin;
	
	PORT_t *stepPort;
	int stepPin;
	
	char directionReversed;
	char inManualMode;
	
	int stepsPerMM;
	double mmFromZero;
	double acceptableError;
	
	volatile long long steps;
	volatile double desiredPosition;
	
	volatile char shouldMove;
		
}Axes_t;

Axes_t XAxis;
Axes_t YAxis;
Axes_t ZAxis;
Axes_t AAxis;

char steppersEnabled;

void enableSteppers();
void disableSteppers();

void calibrateAllSteppers();
void homeAllSteppers();

void homeXStepper();
void homeYStepper();
void homeZStepper();
void homeAStepper();

double getAxisPosition(Axes_t *axis);

void setStepsOffset(Axes_t *axis);

#endif /* STEPPERS_H_ */