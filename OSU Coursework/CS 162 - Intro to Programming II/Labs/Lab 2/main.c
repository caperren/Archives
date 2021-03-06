/**
@file main.c
@brief Lab 2 Starter Code
@version .01 
@mainpage Lab 2 Starter Code

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

/** Constants */
#define DEBUG				//Comment or uncomment this to enable/disable debugging
#define F_CPU 1000000U

/** Sets the string send speed for debugging when defined */		
#ifdef DEBUG
	#define SENDSPEED .2	//If less than .2, there are problems
#endif	

/** Global Variables */

/** Functions */

/** When DEBUG is defined, this provides the functions needed to debug over serial */
#ifdef DEBUG
	/** This function needs to setup the variables used by the UART to enable the UART and tramsmit at 9600bps. This 
	function should always return 0. Remember, by defualt the Wunderboard runs at 1mHz for its system clock.*/
	unsigned char InitializeUART (){
		unsigned char baud = ((F_CPU/(9600*8UL))-1);
		
		/* Set baud rate */
		UBRR1H = (baud >> 8);
		UBRR1L = baud;
		/* Set the U2X1 bit */
		UCSR1A = (1 << U2X1);
		/* Enable transmitter */
		UCSR1B = (1 << TXEN1);
		/* Set frame format: 8data, 1stop bit */
		UCSR1C = (3 << UCSZ10);

		return 0;
	}

	/** This function needs to write a single byte to the UART. It must check that the UART is ready for a new byte 
	and return a 1 if the byte was not sent. 
	@param [in] data This is the data byte to be sent.
	@return The function returns a 1 or error and 0 on successful completion.*/

	unsigned char SendByteUART (unsigned char data){
			
		/* 
		** Checks the buffer empty bit so that data can either be placed in the buffer
		** or else reuturn a 1
		*/
		if((UCSR1A & (1 << UDRE1)) == (1 << UDRE1)){
			UDR1 = data;
			_delay_ms(SENDSPEED);			//Places data into the UDR1 8-bit data buffer
		}else {
			return 1;
		}

		return 0;
	}

	/** This function needs to writes a string to the UART. It must check that the UART is ready for a new byte and 
	return a 1 if the string was not sent. 
	@param [in] data This is a pointer to the data to be sent.
	@return The function returns a 1 or error and 0 on successful completion.*/

	unsigned char SendStringUART (unsigned char *data){
		
		for (int loop = 0 ; data[loop] != '\0'; loop++){
			unsigned char bytereturn;	//Storage for return value of SendByteUART
					
			/* Sends the current byte based on the loop umber to SendByte */
			bytereturn = SendByteUART(data[loop]);
			
			/* Stops sending if SendByteUART had an error */
			if(bytereturn == 1){
				return 1;
			}	
		}
		return 0;
	}
#endif

/** The initialize() function initializes all of the Data Direction Registers for the Wunderboard. Before making changes to DDRx registers, ensure that you have read the peripherals section of the Wunderboard user guide.*/
void initialize (void) {
	/** Port A is the switches and buttons. They should always be inputs. ( 0 = Input and 1 = Output )*/
	DDRA=0b00000000;

	/** Port B has the LED Array color control, SD card, and audio-out on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRB=0b11000000;

	/** Port C is for the 'row' of the LED array. They should always be outputs. ( 0 = Input and 1 = Output )*/
	DDRC=0b11111111;

	/** Port D has the Serial on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRD=0b00001000;

	/** Port E has the LED Array Column control out on it. Leave DDRE alone. ( 0 = Input and 1 = Output )*/
	DDRE=0b00000111;

	/** Port F has the accelerometer and audio-in on it. Leave DDRF alone. ( 0 = Input and 1 = Output )*/
	DDRF=0b00000000;
}

/** The clearArray() function turns off all LEDS on the Wunderboard array. It accepts no inputs and returns nothing*/
void clearArray(void) {
	PORTC = 0x00;
	PORTB |= (1 << PB6) | (1 << PB7);		/** Enable latches*/
	PORTB &= ~((1 << PB6) | (1 << PB7));	/** Disable latches*/
}

/** Main Function */

int main (void) {
	/** Local Varibles */
	
	initialize();
	clearArray();
	
	/** Initializes the serial interface if debugging is enabled */
	#ifdef DEBUG
		InitializeUART();
	#endif
	
	PORTB = 0b10000000;
	
	while(1){
		unsigned char pinastatus = PINA;
		unsigned char debugstring[50];
		
		#ifdef DEBUG
			sprintf(debugstring, "Port A is: %d.\n", pinastatus);
			SendStringUART(debugstring);
		#endif
		
		
		
		for (unsigned char loop = 0 ; loop < 8 ; loop++){
			PORTB = 0b10000000;
			PORTE = loop;
			PORTC = pinastatus;
			_delay_ms(.07);
			clearArray();
			
		}	
	}//while
}//main