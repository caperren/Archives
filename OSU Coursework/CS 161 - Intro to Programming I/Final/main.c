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
#define STEPSIZE 2.75

#define SLOW 50
#define FAST 25
#define FASTEST 10
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
	
	char adcread = read_adc(5);
	
	if(adcread <= 181){
		return 0;
	}else if((adcread == 182) | (adcread == 183)){
		return 1;
	}else if((adcread == 184) | (adcread == 185) | (adcread == 186)){
		return 2;
	}else if((adcread >= 187) & (adcread <= 191)){
		return 4;
	}else if((adcread == 192) | (adcread == 193) | (adcread == 194)){
		return 6;
	}else if((adcread == 195) | (adcread == 196) | (adcread == 197)){
		return 7;
	}else if(adcread >= 198){
		return 8;
	}		
	
}

void printscreen(int loop, int loop3, const int *Red, const int *Green){
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
	
/** Main Function */
 
int main (void) {
	/** Local Varibles */	
	int loop = 0;
	int *looppoint = &loop;
	int loop2;
	int loop3;
	int speed = 20;
	int adcreturn;
	
	const int Red[136] = {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000,0b00000000,0b00001000,0b00101000,0b00001000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000100,0b00000100,0b00010000,0b00010000,0b00010000,0b00010000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00010000,0b00010100,0b00000000,0b00000000,0b00001000,0b00001000,0b00000000,0b00000000,0b00001000,0b00000000,0b00101000,0b00000000,0b00001000,0b00000000,0b00000000,0b00100000,0b00100000,0b00000000,0b00000000,0b00101000,0b00100000,0b00000000,0b00000000,0b00000010,0b00000110,0b00001110,0b00000000,0b00001110,0b00000110,0b00000010,0b0000000,0b00000000,0b00000010,0b00000110,0b00001110,0b00001110,0b00000000,0b00001110,0b00000110,0b00000010,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000,0b00001000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000010,0b00000110,0b00001110,0b00011110,0b00011110,0b00000000,0b00000000,0b00000000,0b00000010,0b00000000,0b00000000,0b00001110,0b00111110,0b00011000,0b00111110,0b00001110,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000};
	const int Green[136] = {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000001,0b00000001,0b01000001,0b11000001,0b01000001,0b00000001,0b00000001,0b00001001,0b00000001,0b00001001,0b00101001,0b00001001,0b00000001,0b00000001,0b00000001,0b00000001,0b10000111,0b11000111,0b01000001,0b00000001,0b00000001,0b00001111,0b00001111,0b00000001,0b00000001,0b00001111,0b00001111,0b00000001,0b00000001,0b00001111,0b00001111,0b00000001,0b00000001,0b00001001,0b00000001,0b00000001,0b01000001,0b11100001,0b11100000,0b01000000,0b00000001,0b00000101,0b00000101,0b00010001,0b00010001,0b00010001,0b00010001,0b00000001,0b00000001,0b00000000,0b00000000,0b00000000,0b00010001,0b00010101,0b00000001,0b00000001,0b01001001,0b11001001,0b11000001,0b10000001,0b00001001,0b00000001,0b00101001,0b00000001,0b00001001,0b00000001,0b00000001,0b00100001,0b00100001,0b00000001,0b00000001,0b00101001,0b00100001,0b00000001,0b00000001,0b00000011,0b00000111,0b00001111,0b00000001,0b00001111,0b00000111,0b01000011,0b11000001,0b01000001,0b00000011,0b00000111,0b00001111,0b00001111,0b00000000,0b00001111,0b00000111,0b00000011,0b00000001,0b00000111,0b00000111,0b00000001,0b00000001,0b00001001,0b00001001,0b00000001,0b00000001,0b00000001,0b00000111,0b00000111,0b00000011,0b00000111,0b00001111,0b00011111,0b00011111,0b00000001,0b01000001,0b11100001,0b11100011,0b01000001,0b00000001,0b00000001,0b00000001,0b00000001,0b00000001,0b00000001,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000};
	
	initialize();
	clearArray();

	while(1){
		
		adcreturn = readadcval();
		
		if(adcreturn == 4){
			
			for(loop2 = 0; loop2 < 50 ; loop2++){
				for(loop3 = 7 ; loop3 > -1 ; loop3--){
					printscreen(loop, loop3, Red, Green);
				}				
			}
			
		}else if(adcreturn < 4){
			for(loop ; loop >= 0 ; loop--){
				
				if(adcreturn == 2){
					speed = SLOW;
				}else if(adcreturn == 1){
					speed = FAST;
				}else if(adcreturn == 0){
					speed = FASTEST;
				}	
				
				for(loop2 = 0 ; loop2 < speed ; loop2++){
					for(loop3 = 7 ; loop3 > -1 ; loop3--){
					
						printscreen(loop, loop3, Red, Green);
								
						if(loop == 0){
							loop = 128;
						}
					}	
				}
				
				adcreturn = readadcval();
				if((adcreturn < 4) != 1){
					break;
				}	
			}
		
		
	
		}else if(adcreturn > 4){
			for(loop ; loop <= 128 ; loop++){
					
				if(adcreturn == 6){
					speed = SLOW;
				}else if(adcreturn == 7){
					speed = FAST;
				}else if(adcreturn == 8){
					speed = FASTEST;
				}
				
				for(loop2 = 0 ; loop2 < speed ; loop2++){
					for(loop3 = 7 ; loop3 > -1 ; loop3--){
					
						printscreen(loop, loop3, Red, Green);
								
						if(loop == 128){
							loop = 0;
						}
					}	
				}
				
				adcreturn = readadcval();
				if((adcreturn > 4) != 1){
					break;
				}
			}
		}
	}
}