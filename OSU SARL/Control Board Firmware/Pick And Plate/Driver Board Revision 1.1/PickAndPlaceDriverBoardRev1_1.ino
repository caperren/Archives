#include <SoftwareSerial.h>
#include "DriverBoardClass.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Pin Definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Inputs/////////////////////////////////////////
////////////////////////////////////////////////
const unsigned char p_ESTOP = 2;
const unsigned char p_LimitESTOP = 3;

const unsigned char p_XFault = 27;
const unsigned char p_YFault = 33;
const unsigned char p_AFault = 41;
const unsigned char p_SFault = A13;

const unsigned char p_X1_Lim = A0;
const unsigned char p_X2_Lim = A1;
const unsigned char p_Y1_Lim = A2;
const unsigned char p_Y2_Lim = A3;
const unsigned char p_Z1_Lim = A4;
const unsigned char p_Z2_Lim = A5;
const unsigned char p_A1_Lim = A6;
const unsigned char p_A2_Lim = A7;

const unsigned char p_XJL = 42;
const unsigned char p_XJR = 43;
const unsigned char p_YJF = 44;
const unsigned char p_YJB = 45;
const unsigned char p_ZJU = 46;
const unsigned char p_ZJD = 47;
const unsigned char p_AJF = 48;
const unsigned char p_AJB = 49;
const unsigned char p_JSI = A14;
const unsigned char p_JSD = A15;

/////////////////////////////////////////////////
//Outputs////////////////////////////////////////
////////////////////////////////////////////////
const unsigned char p_M_Enable = 13;

const unsigned char p_StatusLed = 8;
const unsigned char p_ErrorLed = 9;

const unsigned char p_XStep = 22;
const unsigned char p_XDir = 23;
const unsigned char p_XM0 = 24;
const unsigned char p_XM1 = 25;
const unsigned char p_XM2 = 26;

const unsigned char p_YStep = 28;
const unsigned char p_YDir = 29;
const unsigned char p_YM0 = 30;
const unsigned char p_YM1 = 31;
const unsigned char p_YM2 = 32;

const unsigned char p_AStep = 34;
const unsigned char p_ADir = 35;
const unsigned char p_AM0 = 36;
const unsigned char p_AM1 = 37;
const unsigned char p_AM2 = 40;

const unsigned char p_SStep = A8;
const unsigned char p_SDir = A9;
const unsigned char p_SM0 = A10;
const unsigned char p_SM1 = A11;
const unsigned char p_SM2 = A12;

const unsigned char p_Servo = 5;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Global Variables and Instantiations
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Axis XAxis(p_XStep, p_XDir, p_XM0, p_XM1, p_XM2, p_XFault);
Axis YAxis(p_YStep, p_YDir, p_YM0, p_YM1, p_YM2, p_YFault);
Axis ZAxis(p_AStep, p_ADir, p_AM0, p_AM1, p_AM2, p_AFault);
DriverBoard myDriver(&XAxis, &YAxis, &ZAxis, p_Servo, p_StatusLed, p_ErrorLed, p_M_Enable, p_ESTOP, p_LimitESTOP, p_X1_Lim, p_X2_Lim, p_Y1_Lim, p_Y2_Lim,
p_Z1_Lim, p_Z2_Lim, p_A1_Lim, p_A2_Lim, p_XJL, p_XJR, p_YJF, p_YJB, p_ZJU, p_ZJD, p_AJF, p_AJB, p_JSI, p_JSD);


SoftwareSerial MySerial(10, 12);

String ReceiveBuffer = "";


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Interrupt Service Routines
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER1_COMPA_vect){
 myDriver.MoveToPosition(1);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Arduino Setup Function
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){
 // cli();
  Serial.begin(4800);
  MySerial.begin(4800);

TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  PRR1 &= ~(1 << PRTIM1);    //Disable power reduction mode on timer 1 ((Might need to be changed...))
  TCCR1B |= ((1 << WGM12) | (1 << CS11));// | (1 << CS10)); //Set prescalaer to 64
  OCR1A = 2358;              //Set count value
  TIMSK1 = (1 << OCIE1A);

  sei();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Arduino Main Program Loop
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){
  if(MySerial.available()){
    ReceiveBuffer = "";
    while(MySerial.available()){
      int temp = MySerial.read();
      if(isDigit(temp)){
        ReceiveBuffer += char(temp);
      }
    }
    //XAxis.SetDelay(ReceiveBuffer.toInt());
    //YAxis.SetDelay(ReceiveBuffer.toInt());
    //ZAxis.SetDelay(ReceiveBuffer.toInt());

    MySerial.println(XAxis.SetRPM(ReceiveBuffer.toInt()));

  }


  //ZAxis.StepForward();


  if(digitalRead(p_XJL)){
    myDriver.MoveToPosition(1);
  }
  else if(digitalRead(p_XJR)){
    //myDriver.MoveToPosition(1);
  }

  if(digitalRead(p_YJF)){
    //YAxis.StepForward();
  }
  else if(digitalRead(p_YJB)){
    //YAxis.StepBackward();
  }

}







