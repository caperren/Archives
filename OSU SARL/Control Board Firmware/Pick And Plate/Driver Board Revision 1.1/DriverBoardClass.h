#ifndef DRIVERBOARD_CLASS_H
#define DRIVERBOARD_CLASS_H

#include "AxisClass.h"

//extern Axis XAxis;
///extern Axis YAxis;
//extern Axis ZAxis;
  
class DriverBoard{
private:
  float CurXPos;
  float CurYPos;
  float CurZPos;
  
  unsigned char p_Servo;
  unsigned char p_StatusLed;
  unsigned char p_ErrorLed;
  unsigned char p_M_Enable;
  unsigned char p_ESTOP;
  unsigned char p_LimitESTOP;
  unsigned char p_X1_Lim;
  unsigned char p_X2_Lim;
  unsigned char p_Y1_Lim;
  unsigned char p_Y2_Lim;
  unsigned char p_Z1_Lim;
  unsigned char p_Z2_Lim;
  unsigned char p_A1_Lim;
  unsigned char p_A2_Lim;
  unsigned char p_XJL;
  unsigned char p_XJR;
  unsigned char p_YJF;
  unsigned char p_YJB;
  unsigned char p_ZJU;
  unsigned char p_ZJD;
  unsigned char p_AJF;
  unsigned char p_AJB;
  unsigned char p_JSI;
  unsigned char p_JSD;
  
  Axis *XAxis;
  Axis *YAxis;
  Axis *ZAxis;
  
public:
  DriverBoard(Axis *XAxisPt, Axis *YAxisPt, Axis *ZAxisPt, unsigned char ServoP, unsigned char StatusLed, unsigned char ErrorLed, unsigned char M_Enable, unsigned char ESTOP, unsigned char LimitESTOP, unsigned char X1_Lim, unsigned char X2_Lim, unsigned char Y1_Lim, 
              unsigned char Y2_Lim, unsigned char Z1_Lim, unsigned char Z2_Lim, unsigned char A1_Lim, unsigned char A2_Lim, unsigned char XJL, unsigned char XJR, unsigned char YJF, unsigned char YJB, unsigned char ZJU, 
              unsigned char ZJD, unsigned char AJF, unsigned char AJB, unsigned char JSI, unsigned char JSD);
              
  void MoveToPosition(long XPos);
};

#endif
