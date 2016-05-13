/*
 * SystemInputFunctions.h
 *
 * Created: 8/12/2014 3:32:03 PM
 *  Author: corwin
 */ 


#ifndef SYSTEMINPUTFUNCTIONS_H_
#define SYSTEMINPUTFUNCTIONS_H_


typedef struct ButtonStates{
	volatile char estopPressed;
	volatile char startPressed;
	volatile char stopPressed;
	volatile char homePressed;
	volatile char calibratePressed;
}ButtonStates_t;
 
ButtonStates_t btnState;

#endif /* SYSTEMINPUTFUNCTIONS_H_ */