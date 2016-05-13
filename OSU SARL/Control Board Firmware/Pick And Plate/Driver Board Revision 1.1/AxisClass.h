#ifndef AXIS_CLASS_H
#define AXIS_CLASS_H
const unsigned char Microstep[6][3] = {
  {
    0, 0, 0
  }
  ,
  {
    1, 0, 0
  }
  ,
  {
    0, 1, 0
  }
  ,
  {
    1, 1, 0
  }
  ,
  {
    0, 0, 1
  }
  ,
  {
    1, 0, 1
  }
};

class Axis {
private:
  unsigned char p_Step;
  unsigned char p_Dir;
  unsigned char p_M0;
  unsigned char p_M1;
  unsigned char p_M2;
  unsigned char p_Fault;

  unsigned int DelVal;

public:
  Axis (unsigned char AxisStep, unsigned char AxisDir, unsigned char AxisM0, unsigned char AxisM1, unsigned char AxisM2, unsigned char AxisFault);
  void SetMicrostep(unsigned char Level);
  void StepForward();
  void StepBackward();
  int SetRPM(int RPM);
  void SetDelay(unsigned int delLen);
};
#endif

