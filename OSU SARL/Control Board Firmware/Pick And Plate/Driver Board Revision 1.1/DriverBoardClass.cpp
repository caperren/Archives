#include "DriverBoardClass.h"
#include <Arduino.h>

DriverBoard::DriverBoard(Axis *XAxisPt,Axis *YAxisPt, Axis *ZAxisPt, unsigned char ServoP, unsigned char StatusLed, unsigned char ErrorLed, unsigned char M_Enable, unsigned char ESTOP, unsigned char LimitESTOP, unsigned char X1_Lim, unsigned char X2_Lim, unsigned char Y1_Lim, 
                         unsigned char Y2_Lim, unsigned char Z1_Lim, unsigned char Z2_Lim, unsigned char A1_Lim, unsigned char A2_Lim, unsigned char XJL, unsigned char XJR, unsigned char YJF, unsigned char YJB, unsigned char ZJU, 
                         unsigned char ZJD, unsigned char AJF, unsigned char AJB, unsigned char JSI, unsigned char JSD){
  //Assign Pins to Class Variables
  XAxis = XAxisPt;
  YAxis = YAxisPt;
  ZAxis = ZAxisPt;
  p_Servo = ServoP;
  p_StatusLed = StatusLed;
  p_ErrorLed = ErrorLed;
  p_M_Enable = M_Enable;
  p_ESTOP = ESTOP;
  p_LimitESTOP = LimitESTOP;
  p_X1_Lim = X1_Lim;
  p_X2_Lim = X2_Lim;
  p_Y1_Lim = Y1_Lim;
  p_Y2_Lim = Y2_Lim;
  p_Z1_Lim = Z1_Lim;
  p_Z2_Lim = Z2_Lim;
  p_A1_Lim = A1_Lim;
  p_A2_Lim = A2_Lim;
  p_XJL = XJL;
  p_XJR = XJR;
  p_YJF = YJF;
  p_YJB = YJB;
  p_ZJU = ZJU;
  p_ZJD = ZJD;
  p_AJF = AJF;
  p_AJB = AJB;
  p_JSI = JSI;
  p_JSD = JSD;
  
  //Set pin modes for output pins
  pinMode(p_Servo, OUTPUT);
  pinMode(p_StatusLed, OUTPUT);
  pinMode(p_ErrorLed, OUTPUT);
  pinMode(p_M_Enable, OUTPUT);
  
  //Set pin modes for input pins
  pinMode(p_ESTOP, INPUT);
  pinMode(p_LimitESTOP, INPUT);
  pinMode(p_X1_Lim, INPUT);
  pinMode(p_X2_Lim, INPUT);
  pinMode(p_Y1_Lim, INPUT);
  pinMode(p_Y2_Lim, INPUT);
  pinMode(p_Z1_Lim, INPUT);
  pinMode(p_Z2_Lim, INPUT);
  pinMode(p_A1_Lim, INPUT);
  pinMode(p_A2_Lim, INPUT);
  pinMode(p_XJL, INPUT);
  pinMode(p_XJR, INPUT);
  pinMode(p_YJF, INPUT);
  pinMode(p_YJB, INPUT);
  pinMode(p_ZJU, INPUT);
  pinMode(p_ZJD, INPUT);
  pinMode(p_AJF, INPUT);
  pinMode(p_AJB, INPUT);
  pinMode(p_JSI, INPUT);
  pinMode(p_JSD, INPUT);  
  
  digitalWrite(p_M_Enable, LOW); //Enable motor drivers
  
};

void DriverBoard::MoveToPosition(long XPos){
  XAxis->StepForward();
  YAxis->StepForward();
  ZAxis->StepForward();
}

