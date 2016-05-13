#include "AxisClass.h"
#include <Arduino.h>
extern const unsigned char Microstep[6][3];

Axis::Axis (unsigned char AxisStep, unsigned char AxisDir, unsigned char AxisM0, unsigned char AxisM1, unsigned char AxisM2, unsigned char AxisFault){
  p_Step = AxisStep;
  p_Dir = AxisDir;
  p_M0 = AxisM0;
  p_M1 = AxisM1;
  p_M2 = AxisM2;
  p_Fault = AxisFault;
  DelVal = 8;

  pinMode(p_Step, OUTPUT);
  pinMode(p_Dir, OUTPUT);
  pinMode(p_M0, OUTPUT);
  pinMode(p_M1, OUTPUT);
  pinMode(p_M2, OUTPUT);
  pinMode(p_Fault, INPUT);

  SetMicrostep(8); 
}


void Axis::SetMicrostep(unsigned char Level){
  unsigned char row = -1;
  switch (Level){
  case 1:
    row = 0;
    break;
  case 2:
    row = 1;
    break;
  case 4:
    row = 2;
    break;
  case 8:
    row = 3;
    break;
  case 16:
    row = 4;
    break;
  case 32:
    row = 5;
    break;
  }

  digitalWrite(p_M0, Microstep[row][0]);
  digitalWrite(p_M1, Microstep[row][1]);
  digitalWrite(p_M2, Microstep[row][2]);


}

void Axis::StepForward(){
  digitalWrite(p_Dir, HIGH);
  digitalWrite(p_Step, HIGH);
  digitalWrite(p_Step, LOW);
}

void Axis::StepBackward(){
  digitalWrite(p_Dir, LOW);
  digitalWrite(p_Step, HIGH);
  digitalWrite(p_Step, LOW);  
}

int Axis::SetRPM(int RPM){
  int CountVal = ((16000000L/8)/((200L*8*RPM)/60));
  cli();
  OCR1A = CountVal;
  sei();
  return CountVal;

}

void Axis::SetDelay(unsigned int delLen){
  DelVal = delLen;
}













