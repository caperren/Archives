/**
@file main.c
@brief Lab 6 Starter Code
@version .01 
@mainpage Lab 6 Starter Code

@section intro Code Overview
This first lab allows outputs to the LED array and single patteren of lights. You need to revise it so that it outputs lights based on the input from the switches.
 
@section hw Hardware Pin Out
PORTA:
Switches A7 - A0

PORTB:

*/

/** Includes */
#include <avr/io.h>
#include <util/delay.h>

/** Constants */
#define F_CPU 1000000U
#define DELAY .01
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

/** Main Function */

int main (void) {
	/** Local Varibles */
	//char red[8];
	//char green[8];
	char loop;

	initialize();
	clearArray();
	

	while(1){
		
		if (PINA == 0b01110000){
			clearArray();

			PORTB = 0b11000000;
			
			PORTE = 0b00000011;
			PORTC = 0b11011010;
			_delay_ms(DELAY);
			clearArray();


		}else if (PINA == 0b10110000){
			clearArray();
		
			PORTB = 0b11000000;
			
			PORTE = 0b00000011;
			PORTC = 0b01010101;
			_delay_ms(DELAY);
			
			clearArray();

			PORTB = 0b11000000;
			PORTE = 0b00000110;
			PORTC = 0b10101010;
			_delay_ms(DELAY);
			clearArray();
			

		}else if (PINA == 0b11010000){
			clearArray();
			
			PORTB = 0b10000000;
			
			PORTE = 0b00000011;
			PORTC = 0b01010101;
			_delay_ms(DELAY);

			clearArray();

			PORTB = 0b01000000;

			PORTE = 0b00000110;
			PORTC = 0b10101010;
			_delay_ms(DELAY);
			clearArray();

			
		}else if (PINA == 0b11111000){
			char red[8] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001};  
			char green[8] = {0b01111000, 0b10111100, 0b11011110, 0b11101111, 0b11110111, 0b01111011, 0b00111101, 0b00011110};  
			char orange[8] = {0b00000111, 0b00000011, 0b00000001, 0b00000000, 0b00000000, 0b10000000, 0b11000000, 0b11100000};  

			clearArray();

			for ( loop = 0 ; loop < 8 ; loop++ ){
				
				PORTB = 0b01000000;
				PORTE = loop;
				PORTC = red[loop];
				_delay_ms(DELAY);
				
				clearArray();

				PORTB = 0b10000000;
				PORTC = green[loop];
				_delay_ms(DELAY);
				
				clearArray();
				
				PORTB = 0b11000000;
				PORTC = orange[loop];
				_delay_ms(DELAY);
				
				clearArray();
				
			}
			
		}else if (PINA == 0b11100000){
			char red[8] = {0b11111111, 0b01110111, 0b00100010,0b00000000, 0b00000000, 0b00100010, 0b01110111, 0b11111111};
			char green[8] = {0b00000000, 0b10001000, 0b11011101, 0b11111111, 0b11111111, 0b11011101, 0b10001000, 0b00000000};  

			clearArray();

			for ( loop = 0 ; loop < 8 ; loop++ ){
				
				PORTB = 0b01000000;
				PORTE = loop;
				PORTC = red[loop];
				_delay_ms(DELAY);
				
				clearArray();
				
				PORTB = 0b10000000;
				PORTC = green[loop];
				_delay_ms(DELAY);

				clearArray();

			}
		}
	}	
	
	
	
}//main
