
#include <SoftwareSerial.h>
#include <Sabertooth.h>

int SabertoothPin = 8;

int LeftRoverPin = 2;
int RightRoverPin = 3;
int KillSwitchPin = 4;

SoftwareSerial SWSerial(NOT_A_PIN, SabertoothPin);
Sabertooth ST(128, SWSerial);

int Left1;
int Right1;

int Left;
int Right;

int LeftPrev = 1500;
int RightPrev = 1500;

int LeftSmoothVal = 65;
int RightSmoothVal = 65;

void setup() {
  SWSerial.begin(9600);
  ST.autobaud();
  Serial.begin(9600);

  pinMode(LeftRoverPin, INPUT);
  pinMode(RightRoverPin, INPUT);
  pinMode(KillSwitchPin, INPUT);
}

void loop() {

  Left1 = pulseIn(LeftRoverPin, HIGH);
  Right1 = pulseIn(RightRoverPin, HIGH);
  //Left
  if((LeftPrev > Left1) && ((LeftPrev - Left1) > LeftSmoothVal)){
    LeftPrev -= LeftSmoothVal;
    Left = map(LeftPrev, 1000, 2000, -127, 127);

  }
  else if((LeftPrev < Left1) && ((Left1 - LeftPrev) > LeftSmoothVal)){
    LeftPrev += LeftSmoothVal;
    Left = map(LeftPrev, 1000, 2000, -127, 127);
  }  
  else{
    Left = map(Left1, 1000, 2000, -127, 127);
    LeftPrev = Left1;
  }
  //Right
  if((RightPrev > Right1) && ((RightPrev - Right1) > RightSmoothVal)){
    RightPrev -= RightSmoothVal;
    Right = map(RightPrev , 2000, 1000, -127, 127);

  }
  else if((RightPrev < Right1) && ((Right1 - RightPrev) > RightSmoothVal)){
    RightPrev += RightSmoothVal;
    Right = map(RightPrev, 2000, 1000, -127, 127);

  }  
  else{
    Right = map(Right1, 2000, 1000, -127, 127);
    RightPrev = Right1;
  }
  //Drive
  if(((Left1 & Right1) != 0) & (pulseIn(KillSwitchPin, HIGH) > 1500)){
    ST.motor(1, Left);
    ST.motor(2, Right);
  }
  else{
    ST.motor(1, 0);
    ST.motor(2, 0); 
  }
}



















