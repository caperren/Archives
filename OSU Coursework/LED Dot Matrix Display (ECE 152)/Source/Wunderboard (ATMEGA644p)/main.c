#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "font1.h"
#include "ht1632c.h"
#include "uart.h"


/* Global Definitions */
#define F_CPU 1000000UL

/* Protypes */
void initialize(void);
void clearArray(void);
void setBPin(unsigned char pin, unsigned char *mode);
void clockPulse(void);

/* Main Function */
int main(void){
	
	enum states{
		setup,
		drawing,
		readusart,
	} currentstate = setup;

	unsigned char STRING1[]="YAY THIS";
	unsigned char STRING2[]="WORKS!!!";
	unsigned char STR1CMP[9];
	unsigned char STR2CMP[9];
	unsigned char x, i, j;
	unsigned char send[50];
				
	while(1){
	
		switch(currentstate){
			case setup:
				initialize();
				clearArray();
				PORTB = 0b10000000;
				PORTC |= 0b10000000;
				ht1632_initialize();
				PORTC |= 0b01000000;
				InitializeUART();
				PORTC |= 0b00100000;
				
				sprintf(send, "Initialized...\n");
				PORTC |= 0b00010000;
				SendStringUART(send);
				_delay_ms(200);
				PORTC = 0;
				strcpy(STRING1, "YAY THIS");
				strcpy(STRING2, "WORKS!!!");
				currentstate = drawing;
				break;
			case drawing:
				for(x=1;x<sizeof(STRING1);x++) {
					null_buffer();
					for(i=0;i<8;i++){
						for(j=0;j<8;j++){
							set_buffer(STRING1[x-1]);
							if (~buffer[i][j] & (1<<0)) {
                                ht1632_plot(j+(8*(x-1))-1,i,GREEN);
								PORTC = ~PORTC;
							} else {
								ht1632_plot(j+(8*(x-1))-1,i,BLACK);
								PORTC = ~PORTC;
							}
						}
					}
				}
				for(x=1;x<sizeof(STRING2);x++) {
					null_buffer();
					for(i=0;i<8;i++){
						for(j=0;j<8;j++){
							set_buffer(STRING2[x-1]);
							if (~buffer[i][j] & (1<<0)) {
                                ht1632_plot(j+(8*(x-1))-1,i+8,GREEN);
								PORTC = ~PORTC;
							} else {
								ht1632_plot(j+(8*(x-1))-1,i+8,BLACK);
								PORTC = ~PORTC;
							}
						}
					}
				}
				PORTC = 0;
				currentstate = readusart;
				break;
			case readusart:
				strcpy(STR1CMP, STRING1);
				strcpy(STR2CMP, STRING2);
				NewScreen(STRING1, STRING2);
				
				if((strcmp(STRING1, "RESETRES") == 0)){
					currentstate = setup;
				}else if((strcmp(STRING1, STR1CMP) != 0) | (strcmp(STRING2, STR2CMP) != 0)){
					currentstate = drawing;
				}
				
				break;
		}
	}
}

/* Other functions */

/* Initialize function */
void initialize(void){
	/** Port A is the switches and buttons. They should always be inputs. ( 0 = Input and 1 = Output )*/
	DDRA=0b00000000;

	/** Port B has the LED Array color control, SD card, and audio-out on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRB=0b11001111;

	/** Port C is for the 'row' of the LED array. They should always be outputs. ( 0 = Input and 1 = Output )*/
	DDRC=0b11111111;

	/** Port D has the Serial on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRD=0b00001000;

	/** Port E has the LED Array Column control out on it. Leave DDRE alone. ( 0 = Input and 1 = Output )*/
	DDRE=0b00000111;

	/** Port F has the accelerometer and audio-in on it. Leave DDRF alone. ( 0 = Input and 1 = Output )*/
	DDRF=0b00000000;
}

void clearArray(void){
	PORTC = 0x00;
	PORTB |= (1 << PB6) | (1 << PB7);		/** Enable latches*/
	PORTB &= ~((1 << PB6) | (1 << PB7));	/** Disable latches*/
}

/* Set Individual Pins High / Low (3.3V, 0V), Takes in a 0-7 */
void setBPin(unsigned char pin, unsigned char *mode){
	if((strcmp(mode, "HIGH") == 0)){
		PORTB |= (1 << pin);
	}else{
		PORTB &= ~(1 << pin);
	}
}
	
/* Clock Pulse for Mode Setting / Readying Data Transmission */
void clockPulse(void){
	setBPin(CLK_PIN, "HIGH");	//Set main clock pin high
	setBPin(CLK_PIN, "LOW");	//Set main clock pin low
}
	
