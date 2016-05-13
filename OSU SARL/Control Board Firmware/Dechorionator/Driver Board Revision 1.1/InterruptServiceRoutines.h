/*
 * InterruptServiceRoutines.h
 *
 * Created: 8/4/2014 4:55:26 PM
 *  Author: corwin
 */ 


#ifndef INTERRUPTSERVICEROUTINES_H_
#define INTERRUPTSERVICEROUTINES_H_

extern SoftLCD LCD;
extern StepperClass Stepper;
extern int i;
extern int TimeCount;

ISR(TIMER2_OVF_vect){
	static int CountDelay = 0;
	CountDelay++;
	if(CountDelay >= 61){
		TimeCount++;
		CountDelay = 0;
	}
}

ISR(TIMER1_COMPA_vect){
	Stepper.OneStep();
	i++;
}


////////Pin change interrupt 2 is for pins pcint 23 to 16
////////
//Pin change interrupt 18 is on pin PORTD2
//ISR(){
	
//}

//Pin change interrupt 19 is on pin PORTD3
//ISR(){
	
//}



#endif /* INTERRUPTSERVICEROUTINES_H_ */