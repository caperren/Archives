/*
 * DechorionatorV1.cpp
 *
 * Created: 7/30/2014 4:55:13 PM
 *  Author: corwin
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "DefineMacrosAndPinDefs.h"
#include "SoftLCD.h"
#include "StepperClass.h"
#include "PumpClass.h"
#include "EncoderClass.h"
#include "InterruptServiceRoutines.h"

SoftLCD LCD(&LCD_TX_PORT, &LCD_TX_DDR, P_LCD_TX, &LCD_PWR_PORT, &LCD_PWR_DDR, P_LCD_PWR);
StepperClass Stepper(&STEPPER_EN_PORT, &STEPPER_EN_DDR, P_STEPPER_EN,
					 &STEPPER_DIR_PORT, &STEPPER_DIR_DDR, P_STEPPER_DIR,
					 &STEPPER_STEP_PORT, &STEPPER_STEP_DDR, P_STEPPER_STEP,
					 &STEPPER_FAULT_PORT, &STEPPER_FAULT_DDR, P_STEPPER_FAULT,
					 &STEPPER_M0_PORT, &STEPPER_M0_DDR, P_STEPPER_M0,
					 &STEPPER_M1_PORT, &STEPPER_M1_DDR, P_STEPPER_M1,
					 &STEPPER_M2_PORT, &STEPPER_M2_DDR, P_STEPPER_M2
					 );
					 
char TopTemp[17];
char BottomTemp[17];

void TemporaryInit(){
	//////////Pump Initialization//////////
	PUMP_PWR_INIT();
	//////////End Pump Initialization//////////
		
	//////////Rotary Encoder Initialization//////////
	ENCODER_A_INIT();
	
	ENCODER_B_INIT();
	
	ENCODER_BUTTON_INIT();
	//////////End Rotary Encoder Initialization//////////
	
	
	//////////Hardware Serial Initialization//////////
	UART_TX_INIT();
	UART_RX_INIT();
	//////////End Hardware Serial Initialization//////////
	
	
	//////////Control Switch Initialization//////////
	SWITCH_TOP_INIT();
	SWITCH_BOTTOM_INIT();
	//////////End Control Switch Initilization//////////
	
}

int TimeCount = 0;

int i = 0;

int main(void)
{
		TemporaryInit();
	TCCR2A = 0;
	TCCR2B = ((1 << CS22) | (1 << CS21) | (1 << CS20));
	TCNT2 = 0;
	TIMSK2 = (1 << TOIE0);
	
	sei();						//Enable all system interrupts
	
	//////////Startup Display and System Initialization//////////
	LCD.showNameScreen();

	LCD.setTopText("      Time     ");
	STEPPER_EN_SET();
    while(1)
    {
		for(int i = 2000 ; i > 1200 ; i--){
			LCD.show();
			snprintf(BottomTemp, 16, "%d", TimeCount);
			LCD.setBottomText(BottomTemp);
			OCR1A = i;
			_delay_us(2000);
			if(SWITCH_TOP_GET() || SWITCH_BOTTOM_GET()){
				STEPPER_EN_SET();
			}else{
				STEPPER_EN_CLR();
			}
		}
		for(int i = 1200 ; i < 2000 ; i++){
			LCD.show();
			snprintf(BottomTemp, 16, "%d", TimeCount);
			LCD.setBottomText(BottomTemp);
			OCR1A = i;
			_delay_us(2000);
			if(SWITCH_TOP_GET() || SWITCH_BOTTOM_GET()){
				STEPPER_EN_SET();
				}else{
				STEPPER_EN_CLR();
			}
		}
		

    }
}