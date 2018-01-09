/**
@file main.c
@brief Lab 8 Starter Code
@version .01 
@mainpage Lab 8 Starter Code

@section intro Code Overview
 
@section hw Hardware Pin Out
Port A:
A0 - A3 : Push Buttons
A4 - A7 : Slide Switches 

Port B:
B0 - B3 : SPI (SD Card)
B4		: Nothing
B5		: Audio Out
B6 		: Red Enable 
B7 		: Green Enable

Port C:
C0 - C7 : LED Array (Row)

Port D:
D0 - D1 : Nothing
D2		: Serial RX
D3		: Serial TX
D4 - D7	: Nothing

Port E:
E0 - E2 : LED Array (Column)
E3		: USB (UID)
E4 - E5	: Nothing
E6		: Relay
E7		: Nothing

Port F:
F0 		: ADC Channel 0
F1 		: ADC Channel 1
F2 		: ADC Channel 2
F3 		: ADC Channel 3
F4		: ADC Channel 4 (Audio In)
F5 		: ADC Channel 5 (Accel X Axis)
F6 		: ADC Channel 6 (Accel Y Axis)
F7 		: ADC Channel 7 (Accel Z Axis (if installed))

 */

/** Includes */
#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#include <stdlib.h>
#include <math.h>

/** Constants */
#define F_CPU 1000000U
#define DELAY .02

#define YMIN 180.0
#define STEPSIZE 1.375
/** Global Variables */

/** Functions */

/** The initialize() function initializes all of the Data Direction Registers for the Wunderboard. Before making changes to DDRx registers, ensure that you have read the peripherals section of the Wunderboard user guide.*/
void initialize (void) {
	/** Port A is the switches and buttons. They should always be inputs. ( 0 = Input and 1 = Output )*/
	DDRA=0b00000000;

	/** Port B has the LED Array color control, SD card, and audio-out on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRB=0b11000000;

	/** Port C is for the 'row' of the LED array. They should always be outputs. ( 0 = Input and 1 = Output )*/
	DDRC=0b11111111;

	/** Port D has the Serial on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRD=0b00000000;

	/** Port E has the LED Array Column control out on it. Leave DDRE alone. ( 0 = Input and 1 = Output )*/
	DDRE=0b00000111;

	/** Port F has the accelerometer and audio-in on it. Leave DDRF alone. ( 0 = Input and 1 = Output )*/
	DDRF=0b00000000;
}

void clearArray(void)
{
	PORTB &= ~((1 << PB6) | (1 << PB7));	// Disable latches
	PORTC = 0x00;
	PORTB |= (1 << PB6) | (1 << PB7);		// Enable latches
	PORTB &= ~((1 << PB6) | (1 << PB7));	// Disable latches
}

int readadcval(void){
	int loop4;
	int adcreturn;
	char adcread = read_adc(5);
					
	for(loop4 = 0 ; loop4 < 16 ; loop4++){
		if(((YMIN + (loop4*STEPSIZE)) <= adcread) & (adcread < (YMIN + ((loop4 + 1)*STEPSIZE)))){	
		adcreturn = 2*loop4;
		}
	}
	return adcreturn;
}	

/** Main Function */
 
int main (void) {
	/** Local Varibles */	
	int loop = 50;
	int loop2;
	int loop3;
	int speed = 20;
	int *speedpoint = &speed;
	
	const int Red[120] = {0b00000000, 0b00000000, 0b01000000, 0b11000000, 0b01000000, 0b00000000, 0b00000000, 0b00001000, 0b00000000, 0b00001000, 0b00101000, 0b00001000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b11000000, 0b01000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b00000000, 0b00000000, 0b01000000, 0b11000000, 0b11100000, 0b01000000, 0b00000000, 0b00000100, 0b00000100, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00010100, 0b00000000, 0b00000000, 0b01001000, 0b11001000, 0b11000000, 0b10000000, 0b00001000, 0b00000000, 0b00101000, 0b00000000, 0b00001000, 0b00000000, 0b00000000, 0b00100000, 0b00100000, 0b00000000, 0b00000000, 0b00101000, 0b00100000, 0b00000000, 0b00000000, 0b00000010, 0b00000110, 0b00001110, 0b00000000, 0b00001110, 0b00000110, 0b01000010, 0b11000000, 0b01000000, 0b00000010, 0b00000110, 0b00001110, 0b00001110, 0b00000000, 0b00001110, 0b00000110, 0b00000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b00001000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b00000110, 0b00001110, 0b00011110, 0b00011110, 0b00000000, 0b01000000, 0b11100000, 0b11100010, 0b01000000, 0b00000000, 0b00001110, 0b00111110, 0b00011000, 0b00111110, 0b00001110};
	const int Green[120] = {0b00000001, 0b00000001, 0b01000001, 0b11000001, 0b01000001, 0b00000001, 0b00000001, 0b00001001, 0b00000001, 0b00001001, 0b00101001, 0b00001001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b10000111, 0b11000111, 0b01000001, 0b00000001, 0b00000001, 0b00001111, 0b00001111, 0b00000001, 0b00000001, 0b00001111, 0b00001111, 0b00000001, 0b00000001, 0b00001111, 0b00001111, 0b00000001, 0b00000001, 0b00001001, 0b00000001, 0b00000001, 0b01000001, 0b11000001, 0b11100000, 0b01000000, 0b00000001, 0b00000101, 0b00000101, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00000001, 0b00000001, 0b00000000, 0b00000000, 0b00000000, 0b00010001, 0b00010101, 0b00000001, 0b00000001, 0b01001001, 0b11001001, 0b11000001, 0b10000001, 0b00001001, 0b00000001, 0b00101001, 0b00000001, 0b00001001, 0b00000001, 0b00000001, 0b00100001, 0b00100001, 0b00000001, 0b00000001, 0b00101001, 0b00100001, 0b00000001, 0b00000001, 0b00000011, 0b00000111, 0b00001111, 0b00000001, 0b00001111, 0b00000111, 0b01000011, 0b11000001, 0b01000001, 0b00000011, 0b00000111, 0b00001111, 0b00001111, 0b00000000, 0b00001111, 0b00000111, 0b00000011, 0b00000001, 0b00000111, 0b00000111, 0b00000001, 0b00000001, 0b00001001, 0b00001001, 0b00000001, 0b00000001, 0b00000001, 0b00000111, 0b00000111, 0b00000011, 0b00000111, 0b00001111, 0b00011111, 0b00011111, 0b00000001, 0b01000001, 0b11100001, 0b11100011, 0b01000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001};
	
	initialize();
	clearArray();

	while(1){
		
		speedpoint = 25;
		
		for(loop ; loop >= 0 ; loop--){
			speedpoint = readadcval();
			
			if(speedpoint == 0){
				while(speedpoint == 0){	
					for(loop2 = 0 ; loop2 < 150 ; loop2++){
						for(loop3 = 7 ; loop3 > -1 ; loop3--){
						
													
							int arrval;
							clearArray();
							PORTE = loop3;
							arrval = (loop+loop3);
							
							PORTB = 0b10000000;
							PORTC = Green[arrval];
							_delay_ms(DELAY);
									
							clearArray();
					
							PORTB= 0b01000000;
							PORTC = Red[arrval];
							_delay_ms(DELAY);

							clearArray();
						}	
					}
					speedpoint = readadcval();
				}	
			}else{	
				for(loop2 = 0 ; loop2 < speedpoint ; loop2++){
					for(loop3 = 7 ; loop3 > -1 ; loop3--){
					
												
						int arrval;
						clearArray();
						PORTE = loop3;
						arrval = (loop+loop3);
						
						PORTB = 0b10000000;
						PORTC = Green[arrval];
						_delay_ms(DELAY);
								
						clearArray();
				
						PORTB= 0b01000000;
						PORTC = Red[arrval];
						_delay_ms(DELAY);

						clearArray();
					}	
				}
			}
			
			if(loop == 0){
				loop = 112;
			}
		}	
	}
}
	

