/*
 * TimerCounterFunctionsAndISR.h
 *
 * Created: 8/12/2014 3:56:59 PM
 *  Author: corwin
 */ 


#ifndef TIMERCOUNTERFUNCTIONSANDISR_H_
#define TIMERCOUNTERFUNCTIONSANDISR_H_
#include "DefinesAndMacros.h"
#include "Steppers.h"
#include "SystemInputFunctions.h"
#include <stdlib.h>

volatile long int CurrentTime = 0, PreviousTime = 0;
volatile char secondCounter = 0;

ISR(RTC_OVF_vect)
{
	secondCounter++;
	if(secondCounter >= 100){
		CurrentTime++;
		if(CurrentTime == 0){
			PreviousTime = 0;
		}
		secondCounter = 0;
	}
	
	if((secondCounter % 25) == 0){
		RDY_LED_TGL();
	}
	
}

ISR(TCC0_OVF_vect){
	static double desiredPositionPrevious = 0, currentPosition = 0;
	static double middlePosition = 0, middleOffset = 0, maxDiff = 0, scalar = 0;
	static double axisDiff = 0;			  
	static char doneOnce = FALSE;
	
	
	
	if(XAxis.shouldMove && steppersEnabled){
		currentPosition = getAxisPosition(&XAxis);
		
		if(doneOnce == FALSE){
			desiredPositionPrevious = currentPosition;
			middlePosition = (((XAxis.desiredPosition-desiredPositionPrevious)/2) + desiredPositionPrevious);
			maxDiff = abs(middlePosition - desiredPositionPrevious);
			doneOnce = TRUE;
		}
		
		axisDiff = (XAxis.desiredPosition - currentPosition);
		if(XAxis.inManualMode == TRUE){
			TCC0.PER = 14336;
		}else{
			middleOffset = abs(middlePosition - currentPosition);
			if(middleOffset >= (((double)7*maxDiff)/10)){
				double ratio = (maxDiff - (((double)7*maxDiff)/10));
				double prescalar = (ratio-((double)maxDiff-middleOffset));
				scalar = ((prescalar*10000)/ratio);
				TCC0.PER = 5500+(int)scalar;
			}
		}

		if(abs(axisDiff) <= XAxis.acceptableError){
			XAxis.shouldMove = FALSE;
			doneOnce = FALSE;
		}else if(axisDiff > 0){
			X_DIR_SET();
			_delay_us(2);
			X_STEP_SET();
			_delay_us(2);
			X_STEP_CLR();
			XAxis.steps++;
		}else if(axisDiff < 0){
			if(X_LIM_LEFT_GET()){
				X_DIR_CLR();
				_delay_us(2);
				X_STEP_SET();
				_delay_us(2);
				X_STEP_CLR();
				XAxis.steps--;
			}else{
				XAxis.steps = 0;
			}
		}else{
			ERR_LED_SET();
			while(1);
		}
	}
}

ISR(TCD0_OVF_vect){
	static double desiredPositionPrevious = 0, currentPosition = 0;
	static double middlePosition = 0, middleOffset = 0, maxDiff = 0, scalar = 0;
	static double axisDiff = 0;
	static char doneOnce = FALSE;
	
	
	
	if(YAxis.shouldMove && steppersEnabled){
		currentPosition = getAxisPosition(&YAxis);
		
		if(doneOnce == FALSE){
			desiredPositionPrevious = currentPosition;
			middlePosition = (((YAxis.desiredPosition-desiredPositionPrevious)/2) + desiredPositionPrevious);
			maxDiff = abs(middlePosition - desiredPositionPrevious);
			doneOnce = TRUE;
		}
		
		axisDiff = (YAxis.desiredPosition - currentPosition);
		if(YAxis.inManualMode == TRUE){
			TCD0.PER = 10752;
		}else{
			middleOffset = abs(middlePosition - currentPosition);
			if(middleOffset >= (((double)7*maxDiff)/10)){
				double ratio = (maxDiff - (((double)7*maxDiff)/10));
				double prescalar = (ratio-((double)maxDiff-middleOffset));
				scalar = ((prescalar*10000)/ratio);
				TCD0.PER = 5500+(int)scalar;
			}
		}
		
		if(abs(axisDiff) <= YAxis.acceptableError){
			YAxis.shouldMove = FALSE;
			doneOnce = FALSE;
		}else if(axisDiff > 0){
			Y_DIR_CLR();
			_delay_us(2);
			Y_STEP_SET();
			_delay_us(2);
			Y_STEP_CLR();
			YAxis.steps++;
		}else if(axisDiff < 0){
			if(Y_LIM_FORW_GET()){
				Y_DIR_SET();
				_delay_us(2);
				Y_STEP_SET();
				_delay_us(2);
				Y_STEP_CLR();
				YAxis.steps--;
			}else{
				YAxis.steps = 0;
			}
		}else{
			ERR_LED_SET();
			while(1);
		}
	}
}


ISR(TCE0_OVF_vect){
	static double desiredPositionPrevious = 0, currentPosition = 0;
	static double middlePosition = 0, middleOffset = 0, maxDiff = 0, scalar = 0;
	static double axisDiff = 0;
	static char doneOnce = FALSE;
	
	
	
	if(ZAxis.shouldMove && steppersEnabled){
		currentPosition = getAxisPosition(&ZAxis);
		
		if(doneOnce == FALSE){
			desiredPositionPrevious = currentPosition;
			middlePosition = (((ZAxis.desiredPosition-desiredPositionPrevious)/2) + desiredPositionPrevious);
			maxDiff = abs(middlePosition - desiredPositionPrevious);
			doneOnce = TRUE;
		}
		
		axisDiff = (ZAxis.desiredPosition - currentPosition);
		if(ZAxis.inManualMode == TRUE){
			TCE0.PER = 10752;
		}else{
			middleOffset = abs(middlePosition - currentPosition);
			if(middleOffset >= (((double)7*maxDiff)/10)){
				double ratio = (maxDiff - (((double)7*maxDiff)/10));
				double prescalar = (ratio-((double)maxDiff-middleOffset));
				scalar = ((prescalar*4608)/ratio);
				TCE0.PER = 2304+(int)scalar;
			}
		}
		
		if(abs(axisDiff) <= ZAxis.acceptableError){
			ZAxis.shouldMove = FALSE;
			doneOnce = FALSE;
		}else if(axisDiff > 0){
			Z_DIR_CLR();
			_delay_us(2);
			Z_STEP_SET();
			_delay_us(2);
			Z_STEP_CLR();
			ZAxis.steps++;
		}else if(axisDiff < 0){
			Z_DIR_SET();
			_delay_us(2);
			Z_STEP_SET();
			_delay_us(2);
			Z_STEP_CLR();
			ZAxis.steps--;
		}else{
			ERR_LED_SET();
			while(1);
		}
	}
}


ISR(TCF0_OVF_vect){
	static double desiredPositionPrevious = 0, currentPosition = 0;
	static double middlePosition = 0, middleOffset = 0, maxDiff = 0, scalar = 0;
	static double axisDiff = 0;
	static char doneOnce = FALSE;
	
	
	if(AAxis.shouldMove){
		currentPosition = getAxisPosition(&AAxis);
		
		if(doneOnce == FALSE){
			desiredPositionPrevious = currentPosition;
			middlePosition = (((AAxis.desiredPosition-desiredPositionPrevious)/2) + desiredPositionPrevious);
			maxDiff = abs(middlePosition - desiredPositionPrevious);
			doneOnce = TRUE;
		}
		
		axisDiff = (AAxis.desiredPosition - currentPosition);
		if(AAxis.inManualMode == TRUE){
			TCF0.PER = 10752;
		}else{
			middleOffset = abs(middlePosition - currentPosition);
			scalar = (middleOffset*9216/maxDiff);
			TCF0.PER = 4608+(int)scalar;
		}
		
		if(abs(axisDiff) <= AAxis.acceptableError){
			AAxis.shouldMove = FALSE;
			doneOnce = FALSE;
			}else if(axisDiff > 0){
			A_DIR_SET();
			_delay_us(2);
			A_STEP_SET();
			_delay_us(2);
			A_STEP_CLR();
			AAxis.steps++;
			}else if(axisDiff < 0){
			A_DIR_CLR();
			_delay_us(2);
			A_STEP_SET();
			_delay_us(2);
			A_STEP_CLR();
			AAxis.steps--;
			}else{
			ERR_LED_SET();
			while(1);
		}
	}
}

ISR(USARTC0_RXC_vect){
	USART_RXComplete(&usartData);
}


ISR(USARTC0_DRE_vect){
	USART_DataRegEmpty(&usartData);
}

ISR(PORTB_INT0_vect){
	static long int debounceTimePrevious;
	
	if((CurrentTime - debounceTimePrevious) > 1){		
		if(!SYS_HOME_GET()){
			btnState.homePressed = TRUE;
		}
		
		if(!SYS_CAL_GET()){
			btnState.calibratePressed = TRUE;
		}
		
		if(!SYS_START_GET()){
			btnState.startPressed = TRUE;
		}
		debounceTimePrevious = CurrentTime;
	}
}

ISR(PORTF_INT0_vect){
	static long int debounceTimePrevious;
	
	if((CurrentTime - debounceTimePrevious) > 1){
		
		if(!ESTOP_GET()){
			disableSteppers();
			ERR_LED_SET();
			
			_delay_ms(5000);
			
			CCP = CCP_IOREG_gc;
			RST.CTRL =  RST_SWRST_bm;
		}

		debounceTimePrevious = CurrentTime;
	}	
}



#endif /* TIMERCOUNTERFUNCTIONSANDISR_H_ */