/*
 * DefineMacrosAndPinDefs.h
 *
 * Created: 7/30/2014 4:56:56 PM
 *  Author: corwin
 */ 


#ifndef DEFINEMACROSANDPINDEFS_H_
#define DEFINEMACROSANDPINDEFS_H_

//////////LCD Definitions//////////
#define P_LCD_TX				PORTB0														//Pin B0 runs tx pin for serial lcd
#define LCD_TX_PORT				PORTB														//Port that the tx lcd pin is on
#define LCD_TX_DDR				DDRB														//Direction port that the tx lcd pin is on
#define LCD_TX_BM				(1 << P_LCD_TX)												//Bit mask for pin
#define LCD_TX_INIT()			DDRB |= LCD_TX_BM											//Sets tx pin to output
#define LCD_TX_SET()			PORTB |= LCD_TX_BM											//Macro for setting pin high
#define LCD_TX_CLR()			PORTB &= ~LCD_TX_BM											//Macro for setting pin low


#define P_LCD_PWR				PORTC5														//Pin C5 controls the power to the lcd through a mosfet
#define LCD_PWR_PORT			PORTC														//Port that the lcd power pin is on
#define LCD_PWR_DDR				DDRC														//Direction port that the lcd power pin is on
#define LCD_PWR_BM				(1 << P_LCD_PWR)											//Bit mask for pin
#define LCD_PWR_INIT()			DDRC |= LCD_PWR_BM											//Sets lcd power control pin to an output
#define LCD_PWR_SET()			PORTC |= LCD_PWR_BM											//Macro for setting pin high
#define LCD_PWR_CLR()			PORTC &= ~LCD_PWR_BM										//Macro for setting pin low
//////////End LCD Definitions

//////////Pump Definitions//////////
#define P_PUMP_PWR				PORTD5														//Pin D5 controls power to the water pump through a mosfet
#define PUMP_PWR_BM				(1 << P_PUMP_PWR)											//Bit mask for pin
#define PUMP_PWR_INIT()			DDRD |= PUMP_PWR_BM											//Sets pump power control pin to an output
#define PUMP_PWR_SET()			PORTD |= PUMP_PWR_BM										//Macro for setting pin high
#define PUMP_PWR_CLR()			PORTD &= ~PUMP_PWR_BM										//Macro for setting pin low
//////////End Pump Definitions//////////

//////////DRV8825 Stepper Motor Driver Definitions//////////
#define P_STEPPER_DIR			PORTD6														//Pin D6 controls the direction of the stepper motor
#define STEPPER_DIR_PORT		PORTD
#define STEPPER_DIR_DDR			DDRD
#define STEPPER_DIR_BM			(1 << P_STEPPER_DIR)										//Bit mask for pin
#define STEPPER_DIR_INIT()		DDRD |= STEPPER_DIR_BM										//Sets stepper direction pin to an output
#define STEPPER_DIR_SET()		PORTD |= STEPPER_DIR_BM										//Macro for setting pin high
#define STEPPER_DIR_CLR()		PORTD &= ~STEPPER_DIR_BM									//Macro for setting pin low


#define P_STEPPER_STEP			PORTD7														//Pin D7 controls the step pin of the driver
#define STEPPER_STEP_PORT		PORTD
#define STEPPER_STEP_DDR		DDRD
#define STEPPER_STEP_BM			(1 << P_STEPPER_STEP)										//Bit mask for pin
#define STEPPER_STEP_INIT()		DDRD |= STEPPER_STEP_BM										//Sets the step pin to an output
#define STEPPER_STEP_SET()		PORTD |= STEPPER_STEP_BM									//Macro for setting pin high
#define STEPPER_STEP_CLR()		PORTD &= ~STEPPER_STEP_BM									//Macro for setting pin low


#define P_STEPPER_EN			PORTC1														//Pin C1 controls whether the stepper motor is enabled and running or not
#define STEPPER_EN_PORT			PORTC
#define STEPPER_EN_DDR			DDRC
#define STEPPER_EN_BM			(1 << P_STEPPER_EN)											//Bit mask for pin
#define STEPPER_EN_INIT()		DDRC |= STEPPER_EN_BM										//Sets stepper enable control pin to an output
#define STEPPER_EN_SET()		PORTC &= ~STEPPER_EN_BM										//Macro for setting pin high (Note inverted logic!!!)
#define STEPPER_EN_CLR()		PORTC |= STEPPER_EN_BM										//Macro for setting pin low (Note inverted logic!!!)


#define P_STEPPER_FAULT			PORTC0														//Pin C0 senses whether the stepper driver is in a fault state due to over current or over temp conditions
#define STEPPER_FAULT_PORT		PORTC
#define STEPPER_FAULT_DDR		DDRC
#define STEPPER_FAULT_BM		(1 << P_STEPPER_FAULT)										//Bit mask for pin
#define STEPPER_FAULT_INIT()	DDRC &= ~STEPPER_FAULT_BM									//Sets stepper fault check pin to an input (May need to enable internal pull up?)
#define STEPPER_FAULT_GET()		!(PINC & STEPPER_FAULT_BM)									//Returns true if in fault state (0 is fault, 1 is non-fault)



#define P_STEPPER_M0			PORTC2														//Pin C2 sets the lowest bit of the driver micro-stepping
#define STEPPER_M0_PORT			PORTC
#define STEPPER_M0_DDR			DDRC
#define STEPPER_M0_BM			(1 << P_STEPPER_M0)											//Bit mask for pin
#define STEPPER_M0_INIT()		DDRC |= STEPPER_M0_BM										//Sets the micro-step pin to an output
#define STEPPER_M0_SET()		PORTC |= STEPPER_M0_BM										//Macro for setting pin high
#define STEPPER_M0_CLR()		PORTC &= ~STEPPER_M0_BM										//Macro for setting pin low


#define P_STEPPER_M1			PORTC3														//Pin C3 sets the middle bit of the driver micro-stepping
#define STEPPER_M1_PORT			PORTC
#define STEPPER_M1_DDR			DDRC
#define STEPPER_M1_BM			(1 << P_STEPPER_M1)											//Bit mask for pin
#define STEPPER_M1_INIT()		DDRC |= STEPPER_M1_BM										//Sets the micro-step pin to an output
#define STEPPER_M1_SET()		PORTC |= STEPPER_M1_BM										//Macro for setting pin high
#define STEPPER_M1_CLR()		PORTC &= ~STEPPER_M1_BM										//Macro for setting pin low


#define P_STEPPER_M2			PORTC4														//Pin C4 sets the upper bit of the driver micro-stepping
#define STEPPER_M2_PORT			PORTC
#define STEPPER_M2_DDR			DDRC
#define STEPPER_M2_BM			(1 << P_STEPPER_M2)											//Bit mask for pin
#define STEPPER_M2_INIT()		DDRC |= STEPPER_M2_BM										//Sets the micro-step pin to an output
#define STEPPER_M2_SET()		PORTC |= STEPPER_M2_BM										//Macro for setting pin high
#define STEPPER_M2_CLR()		PORTC &= ~STEPPER_M2_BM										//Macro for setting pin low
//////////End DRV8825 Stepper Motor Driver Definitions//////////

//////////Rotary Encoder Definitions//////////
#define P_ENCODER_A				PORTD2														//Pin D2 senses the state for channel A on the rotary encoder
#define ENCODER_A_BM			(1 << P_ENCODER_A)											//Bit mask for pin
#define ENCODER_A_INIT()		{DDRD &= ~ENCODER_A_BM; PORTD |= ENCODER_A_BM;}				//Sets channel A pin to an input and enables the internal pull up resistor
#define ENCODER_A_GET()			!(PIND & ENCODER_A_BM)										//Returns state of the pin (Note inverted logic!!! Makes gray code logic less confusing...)


#define P_ENCODER_B				PORTD3														//Pin D3 sense the state for channel B on the rotary encoder
#define ENCODER_B_BM			(1 << P_ENCODER_B)											//Bit mask for pin
#define ENCODER_B_INIT()		{DDRD &= ~ENCODER_B_BM; PORTD |= ENCODER_B_BM;}				//Sets channel B pin to an input and enables the internal pull up resistor
#define ENCODER_B_GET()			!(PIND & ENCODER_B_BM)										//Returns state of the pin (Note inverted logic!!! Makes gray code logic less confusing...)


#define P_ENCODER_BUTTON		PORTD4														//Pin D4 senses whether the tactile button on the rotary encoder has been pushed
#define ENCODER_BUTTON_BM		(1 << P_ENCODER_BUTTON)										//Bit mask for pin
#define ENCODER_BUTTON_INIT()	{DDRD &= ~ENCODER_BUTTON_BM; PORTD |= ENCODER_BUTTON_BM;}	//Sets encoder button pin to an input and enables the internal pull up resistor
#define ENCODER_BUTTON_GET()	!(PIND & ENCODER_BUTTON_BM)									//Returns whether the button has been pushed (Note inverted logic!!!)
//////////End Rotary Encoder Definitions//////////

//////////Hardware Serial Definitions//////////
#define P_UART_TX				PORTD1														//Pin D1 is the transmit line for the hardware uart
#define UART_TX_BM				(1 << P_UART_TX)											//Bit mask for pin
#define UART_TX_INIT()			DDRD |= UART_TX_BM											//Sets the transmit line to an output


#define P_UART_RX				PORTD0														//Pin D0 is the receive line for the hardware uart
#define UART_RX_BM				(1 << P_UART_RX)											//Bit mask for pin
#define UART_RX_INIT()			DDRD &= ~UART_RX_BM											//Sets the receive line to an input
//////////End Hardware Serial Definitions//////////

//////////Control Switch Definitions//////////
#define P_SWITCH_TOP			PORTB2														//Pin B1 senses whether the top button on the control switch has been pushed (RINSE)
#define SWITCH_TOP_BM			(1 << P_SWITCH_TOP)											//Bit mask for pin
#define SWITCH_TOP_INIT()		{DDRB |= SWITCH_TOP_BM; PORTB |= SWITCH_TOP_BM;}			//Sets the button to an input and enables the internal pull up resistor
#define SWITCH_TOP_GET()		!(PINB & SWITCH_TOP_BM)									//Returns whether the button has been pushed (Note inverted logic!!!)


#define P_SWITCH_BOTTOM			PORTB1														//Pin B2 senses whether the bottom button on the control switch has been pushed (ESTOP)
#define SWITCH_BOTTOM_BM		(1 << P_SWITCH_BOTTOM)										//Bit mask for pin
#define SWITCH_BOTTOM_INIT()	{DDRB |= SWITCH_BOTTOM_BM; PORTB |= SWITCH_BOTTOM_BM;}		//Sets the button to an input and enables the internal pull up resistor
#define SWITCH_BOTTOM_GET()		!(PINB & SWITCH_BOTTOM_BM)									//Returns whether the button has been pushed (Note inverted logic!!!)
//////////Control Switch Definitions//////////


#endif /* DEFINEMACROSANDPINDEFS_H_ */