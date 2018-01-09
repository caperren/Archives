/**
@file main.c
@brief Lab 3 Starter Code
@version .01 
@mainpage Lab 3 Starter Code

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
#define F_CPU 1000000UL
#define DEBUG 1
#ifdef DEBUG == 1
	#define SENDSPEED 3	//If less than .12, there are problems
#endif	

/** Global Variables */

/** Functions */

#ifdef DEBUG == 1
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
		UCSR1B = (1 << TXEN1 | 1 << RXEN1);
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
	
	unsigned char GetByteUART(){
		if (UCSR1A & (1 << RXC1)){
			return(UDR1);
		} else{
			return(-1);
		}	
	}
#endif

/** The clearArray() function turns off all LEDS on the Wunderboard array. It accepts no inputs and returns nothing*/
void clearArray(void){
	PORTC = 0x00;
	PORTB |= (1 << PB6) | (1 << PB7);		/** Enable latches*/
	PORTB &= ~((1 << PB6) | (1 << PB7));	/** Disable latches*/
}

/** The initialize() function initializes all of the Data Direction Registers for the Wunderboard. Before making changes to DDRx registers, ensure that you have read the peripherals section of the Wunderboard user guide.*/
void initialize(void){
	/** Port A is the switches and buttons. They should always be inputs. ( 0 = Input and 1 = Output )*/
	DDRA=0b00000000;

	/** Port B has the LED Array color control, SD card, and audio-out on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRB=0b11000111;

	/** Port C is for the 'row' of the LED array. They should always be outputs. ( 0 = Input and 1 = Output )*/
	DDRC=0b11111111;

	/** Port D has the Serial on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRD=0b00000000;

	/** Port E has the LED Array Column control out on it. Leave DDRE alone. ( 0 = Input and 1 = Output )*/
	DDRE=0b00000111;

	/** Port F has the accelerometer and audio-in on it. Leave DDRF alone. ( 0 = Input and 1 = Output )*/
	DDRF=0b00000000;
}

unsigned char read_adc(uint8_t channel){

	unsigned char test;

	ADMUX = 0x60 | channel; // Set the channel to the one we want
	ADCSRA = 0b11000110; // Start a new sample.
	while ((ADCSRA & 0b00010000) == 0 ); // Wait for a Valid Sample
	ADCSRA |= 0b00010000; // Tell ADC you have the sample you want.
	ADCSRA |= 0b01000000; // Start a new sample.
	while ((ADCSRA & 0b00010000) == 0 ); // Wait for a Valid Sample
	ADCSRA |= 0b00010000; // Tell ADC you have the sample you want.
	
	test = ADCH; 
	ADCSRA = 0x00; // Disable the ADC

	return (test);
}

	
/** This function needs to setup the variables used by TIMER0 Compare Match (CTC) mode with 
a base clock frequency of clk/1024. This function should return a 1 if it fails and a 0 if it 
does not. Remember, by default the Wunderboard runs at 1mHz for its system clock.
@return This function returns a 1 is unsuccessful, else return 0.*/
unsigned char initializeTIMER0(void){
	/* Set the CTC mode */
	TCCR0A = (1 << WGM01);
	
	/* Set the Clock Frequency */
	TCCR0B = ((1 << CS02) | (1 << CS00));
	
	/* Set initial count value */
	OCR0A = 0;
}

/** This function checks if TIMER0 has elapsed. 
@return This function should return a 1 if the timer has elapsed, else return 0*/
unsigned char checkTIMER0(void){
	if((TIFR0 & (1 << OCF0A)) == (1 << OCF0A)){
		TIFR0 |= (1 << OCF0A);
		return 1;
	}else {
		return 0;
	}
}

/** This function takes two values, clock and count. The value of count should be copied into OCR0A and the value of clock should be used to set CS02:0. The TCNT0 variable should also be reset to 0 so that the new timer rate starts from 0.  
@param [in] clock Insert Comment
@param [in] count Insert Comment
@return The function returns a 1 or error and 0 on successful completion.*/
unsigned char setTIMER0(unsigned char clock, unsigned char count){

	if(clock > 7){
		return 1;
	}
	TCCR0B &= 0b11111000;		//Zeros out last three bits
	TCCR0B |= clock;
	OCR0A = count;
	TCNT0 = 0;
	return 0;
}

/** Main Function */
int main (void){
	
	enum states {
		setup,
		idle,
		running
	} state = setup;
	
	unsigned char numelapsed = 0;
	unsigned char debug;
	unsigned char received;
	unsigned char send[150];	
	unsigned char adcreturn1;
	unsigned char adcreturn2;
	
	while(1){
	
		switch(state){
			case setup:
				
				initialize();
				initializeTIMER0();
				InitializeUART();
				clearArray();
				setTIMER0(5, 121);
				PORTB = 0b11000000;
				state = idle;
				sprintf(send, "Initialized...\n");
				SendStringUART(send);
				break;
			
			case idle:
				if(debug != 1){
					sprintf(send, "Idling...\n");
					SendStringUART(send);
					debug = 1;
				}
				received = GetByteUART();
				
				if (received == 's'){
					state = running;
					debug = 0;
					break;
				}
			
				break;
			
			case running:
				
				received = GetByteUART();
				adcreturn1 = read_adc(5);
				adcreturn2 = read_adc(6);
				
				if (received == 's'){
					state = idle;
				}
				
				if (checkTIMER0() == 1){
					if(numelapsed == 3){
						PORTC = ~PORTC;
						numelapsed = 0;
						sprintf(send, "X value: %d\tY value: %d\n", adcreturn1, adcreturn2);
						SendStringUART(send);
						
					}else {
						numelapsed++;
					}	
				}
				break;
		}		
	}
}