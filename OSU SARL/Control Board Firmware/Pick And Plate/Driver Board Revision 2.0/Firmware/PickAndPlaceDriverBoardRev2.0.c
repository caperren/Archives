/*
 * PickAndPlaceDriverBoardRev2.c
 *
 * Created: 8/6/2014 5:56:07 PM
 *  Author: corwin
 */ 
#define F_CPU 32000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "DefinesAndMacros.h"
#include "usart_driver.h"
#include "InitFunctions.h"
#include "TimerCounterFunctionsAndISR.h"
#include "SystemInputFunctions.h"
#include "Steppers.h"
#include "SystemInputFunctions.h"

#define MANUALCONTROLSTEPSIZE 1

enum SystemStates{
	systemInitialization,
	connectToHost,
	connectedWaitForStart,
	disconnectedWaitForStart,
	runSequence,
	homeAxes,
	calibrateAxes,
	manualControl
}systemState = systemInitialization, systemStatePrevious = systemInitialization;

bool switchToManualMode(){
	PreviousTime = CurrentTime;
	while(!SYS_CHNG_STATE_GET()){
		if((CurrentTime-PreviousTime) >= 2){
			return true;
		}
	}
	return false;
}

void roughManualControl(){
	static char manState = 0;
	
	if(manState == 0){
		if(!SYS_START_GET()){
			XAxis.desiredPosition = (getAxisPosition(&XAxis) - MANUALCONTROLSTEPSIZE);
			XAxis.shouldMove = TRUE;
		}else if(!SYS_STOP_GET()){
			XAxis.desiredPosition = (getAxisPosition(&XAxis) + MANUALCONTROLSTEPSIZE);
			XAxis.shouldMove = TRUE;
		}
		
		if(!SYS_HOME_GET()){
			YAxis.desiredPosition = (getAxisPosition(&YAxis) - MANUALCONTROLSTEPSIZE);
			YAxis.shouldMove = TRUE;
		}else if(!SYS_CAL_GET()){
			YAxis.desiredPosition = (getAxisPosition(&YAxis) + MANUALCONTROLSTEPSIZE);
			YAxis.shouldMove = TRUE;
		}
	}else if(manState == 1){
		if(!SYS_HOME_GET()){
			ZAxis.desiredPosition = (getAxisPosition(&ZAxis) + MANUALCONTROLSTEPSIZE);
			ZAxis.shouldMove = TRUE;
		}else if(!SYS_CAL_GET()){
			ZAxis.desiredPosition = (getAxisPosition(&ZAxis) - MANUALCONTROLSTEPSIZE);
			ZAxis.shouldMove = TRUE;
		}
		
	}
	_delay_us(500);
	if(!SYS_CHNG_STATE_GET()){
		manState = !manState;
		_delay_ms(500);
	}
}

int main(void)
{

    while(1){
//////////Initialize System//////////
		if(systemState == systemInitialization){
			setupDriverBoardComplete();
			testDriverBoardComplete();
			RDY_LED_SET();
			systemState = connectToHost;
//////////End initialize system//////////

//////////Wait for host connection//////////
		}else if(systemState == connectToHost){
			if(btnState.startPressed == TRUE){
				enableSteppers();
				char jump = 5;
				
				XAxis.desiredPosition = 10;
				YAxis.desiredPosition = 10;
				
				while(1){
					//sprintf((char *)sendArray,  "Current jump size is %d.\r\n", jump);

				//	for(int i = 0 ; sendArray[i] != '\0' ; i++){
				//		while (!USART_IsTXDataRegisterEmpty(&USARTF0));
				//		USART_TXBuffer_PutByte(&usartData ,sendArray[i]);
				//	}

					YAxis.desiredPosition += jump;
					YAxis.shouldMove = TRUE;
					while(XAxis.shouldMove || YAxis.shouldMove);
					
					XAxis.desiredPosition += jump;
					XAxis.shouldMove = TRUE;
					while(XAxis.shouldMove || YAxis.shouldMove);
					

					YAxis.desiredPosition = 10;

					YAxis.shouldMove = TRUE;
					while(XAxis.shouldMove || YAxis.shouldMove);
					
					XAxis.desiredPosition = 10;
					XAxis.shouldMove = TRUE;
					while(XAxis.shouldMove || YAxis.shouldMove);
					jump += 5;
					
				}
				disableSteppers();
			}else if(btnState.homePressed){
				systemState = homeAxes;
				systemStatePrevious = connectToHost;
			}else if(btnState.calibratePressed == TRUE){
				systemState = calibrateAxes;
				systemStatePrevious = connectToHost;
			}else if(switchToManualMode() == true){
				systemState = manualControl;
				systemStatePrevious = connectToHost;
				enableSteppers();
				XAxis.inManualMode = TRUE;
				YAxis.inManualMode = TRUE;
				ZAxis.inManualMode = TRUE;
				AAxis.inManualMode = TRUE;
				_delay_ms(1500);
			}
//////////End wait for host connection//////////

//////////Connected to Host and wait for start//////////
		}else if(systemState == connectedWaitForStart){
			

//////////End connected to host and wait for start//////////			
		}else if(systemState == disconnectedWaitForStart){
		
//////////End connect to host and wait for start//////////

//////////Start pick and place run sequence//////////
		}else if(systemState == runSequence){
			
//////////End start pick and place run sequence//////////

//////////Home system axes to zero position//////////
		}else if(systemState == homeAxes){
			enableSteppers();
			homeAllSteppers();
			disableSteppers();
			btnState.homePressed = FALSE;
			systemState = systemStatePrevious;
			systemStatePrevious = homeAxes;		
//////////End home system axes//////////
			
/////////Calibrate system stepper axes//////////
		}else if(systemState == calibrateAxes){
			ERR_LED_CLR();
			enableSteppers();
			calibrateAllSteppers();
			btnState.calibratePressed = FALSE;
			if(systemStatePrevious == systemInitialization){
				systemState = connectToHost;
			}else{
				systemState = systemStatePrevious;
			}
			systemStatePrevious = calibrateAxes;
//////////End calibrate system stepper axes//////////


//////////Manual control of system axes//////////
		}else if(systemState == manualControl){
			roughManualControl();
			if(switchToManualMode() == true){
				XAxis.inManualMode = FALSE;
				YAxis.inManualMode = FALSE;
				ZAxis.inManualMode = FALSE;
				AAxis.inManualMode = FALSE;
				btnState.homePressed = FALSE;
				btnState.startPressed = FALSE;
				btnState.stopPressed = FALSE;
				btnState.calibratePressed = FALSE;
				disableSteppers();
				systemState = systemStatePrevious;
				systemStatePrevious = manualControl;
			}
		}
//////////End manual control of system axes//////////
	}
}