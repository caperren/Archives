/*
 * Steppers.c
 *
 * Created: 8/9/2014 1:10:47 PM
 *  Author: corwin
 */ 

#include "Steppers.h"

void enableSteppers()
{
	STEP_EN_CLR();
	steppersEnabled = TRUE;
	_delay_ms(100);
}

void disableSteppers()
{
	STEP_EN_SET();
	steppersEnabled = FALSE;
	_delay_ms(100);
}

void calibrateAllSteppers()
{
	ZAxis.desiredPosition = 40;														//Set position for the pipette to be up initially so it doesn't get broken on anything
	ZAxis.shouldMove = TRUE;														//Allow axis to move
	while(ZAxis.shouldMove);														//Wait for the pipette to come up before continuing
	
	homeYStepper();																	//Home the y axis against the limit switch
	homeXStepper();																	//Home the x axis against the limit switch
	_delay_ms(100);
	
	XAxis.desiredPosition = 47.5;														//Set x position to z calibration point
	YAxis.desiredPosition = 36.5;													//Move y position to z calibration point
	XAxis.shouldMove = TRUE;														//Allow axis to move
	YAxis.shouldMove = TRUE;														//Allow axis to move
	while(YAxis.shouldMove || XAxis.shouldMove);									//Don't continue until calibration point has been reached
	
	homeZStepper();																	//Home the z axis against the limit switch
	setStepsOffset(&ZAxis);															//Re-align z steps to account for switch offset
}

void homeAllSteppers()
{
	
	ZAxis.desiredPosition = 40;														//Set position for the pipette to be up initially so it doesn't get broken on anything
	ZAxis.shouldMove = TRUE;														//Allow axis to move
	while(ZAxis.shouldMove);														//Wait for the pipette to come up before continuing
	
	homeYStepper();																	//Home the y axis against the limit switch
	homeXStepper();																	//Home the x axis against the limit switch
	
}

void homeXStepper()
{
	X_DIR_CLR();
	while(X_LIM_LEFT_GET()){
		X_STEP_SET();
		_delay_us(2);
		X_STEP_CLR();
		_delay_us(400);
	}
	XAxis.steps = 0;
}

void homeYStepper()
{
	Y_DIR_SET();
	while(Y_LIM_FORW_GET()){
		Y_STEP_SET();
		_delay_us(2);
		Y_STEP_CLR();
		_delay_us(400);
	}
	YAxis.steps = 0;
}

void homeZStepper()
{
	Z_DIR_SET();
	while(Z_LIM_DOWN_GET()){
		Z_STEP_SET();
		_delay_us(2);
		Z_STEP_CLR();
		_delay_us(400);
	}
	ZAxis.steps = 0;
	ZAxis.desiredPosition = 30;
	ZAxis.shouldMove = TRUE;
	while(ZAxis.shouldMove);
}

void homeAStepper()
{
	
}

double getAxisPosition(Axes_t *axis)
{
	return (axis->steps/axis->stepsPerMM);
}

void setStepsOffset(Axes_t *axis)
{
	axis->steps = (axis->steps + (axis->mmFromZero * axis->stepsPerMM));
}

