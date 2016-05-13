/*
 * InitFunctions.h
 *
 * Created: 8/9/2014 4:24:44 PM
 *  Author: corwin
 */ 


#ifndef INITFUNCTIONS_H_
#define INITFUNCTIONS_H_
#define F_CPU 32000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "avr_compiler.h"
#include "DefinesAndMacros.h"
#include "usart_driver.h"
#include "Steppers.h"
#include "SystemInputFunctions.h"


void setupDriverBoardComplete();
void testDriverBoardComplete();

void setupXMEGAIO();
void setupXMEGAClock();
void setupXMEGAUART();
void setupXMEGATimers();
void setupStepperPararms();
void setupPinChangeInterrupts();
void setupSystemParams();

void testStatusLeds();
void testLightBoard();
void testSteppers();
void testUART();

void setupIOPin(PORT_t *port, int pinbm, char direction, char defaultstate);
void setupRTC();
void setupStepDriveTimer();


#endif /* INITFUNCTIONS_H_ */