///////////////////////////////////////////////////////////////////////////////////////////////
//Includes, Defines, Instantiations, and Global Variables//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Includes
#include "DualVNH5019MotorShield.h"
#include <SoftwareSerial.h>
#include <StopWatch.h>

//Pin Definitions (Arduino Suggested Method)
const unsigned char LcdTX = 5;  //Pin connected to LCD RX Line
const unsigned char LedGreen = A2;  //Pin connected to rotary encoder led
const unsigned char LedRed = A3;  //Pin connected to rotary encoder led


const unsigned char StartSwitch = 11;  //Pin connected to start switch
const unsigned char StopSwitch = 13;  //Pin connected to stop switch

const unsigned char EncoderChannelA = A4;  //Pin connected to channel A of the rotary encoder
const unsigned char EncoderChannelB = A5;  //Pin connected to channel B of the rotary encoder

//Instantiations
SoftwareSerial LcdSerial(3, LcdTX);  //Software Serial Instantiation with Pin 3 as dummy for RX
DualVNH5019MotorShield MotorDriver;  //Motor Driver Library Instantiation
StopWatch MyStopWatch(StopWatch::SECONDS);  //StopWatch Instantiation for measuring time

enum ArduinoStates{  //Enumeration for defining the arduino program states
  WaitToStart,
  ChangeConfig,
  MainProg
} ArduinoState = WaitToStart;

//Global Variables and Constants
unsigned char DoneOnce = 0;  //Variable to store whether something has happened or not.

const unsigned int LcdPageDelay = 2;  //Seconds to wait before the lcd changes pages.

///////////////////////////////////////////////////////////////////////////////////////////////
//Arduino Setup////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void setup(){
  MotorDriver.init();  //Initialize the VNH5019 Motor Driving Shield
  InitializePins();  //Initialize pins for switches, rotary encoder, and leds
  
  Serial.begin(115200);  //Start communicating to PC at 115200 baud
  
  LcdSerial.begin(9600);  //Start communicating with LCD dispaly at 9600 buad
  FullLcdClear();
  FullLcdWrite("Tanguay Labs", "Dechlorinator");  //Show the device name
  Serial.print("Dechlorinator Initialized");
  delay(5000);  //Wait on name screen for five seconds
  FullLcdClear();  //Clear the LCD
}

///////////////////////////////////////////////////////////////////////////////////////////////
//Main Arduino Program Loop////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void loop(){
  switch(ArduinoState){
    case WaitToStart:
      if(DoneOnce != 1){  //Check to see if the stopwatch needs to be started
        MyStopWatch.reset();  //Reset the stopwatch
        MyStopWatch.start();  //Start the stopwatch
        DoneOnce  = 1;  //Set DoneOnce to 1 so we know it's been started for the first time
      }
      
      if(MyStopWatch.elapsed() < LcdPageDelay){  //Check if the time on the stopwatch is longer than our page delay
        FullLcdWrite(" Press Start To ","     Begin      ");  //Show the start screen
      }else{  //If the time is over the page delay, switch to the config info screen
        FullLcdWrite(" Press Knob To  ","Change Settings ");  //Show the config info screen
        if(MyStopWatch.elapsed() > (2 * LcdPageDelay)){  //Check to see if the time is longer than two page delays
          MyStopWatch.reset();  //Reset the stopwatch to 0
          MyStopWatch.start();  //Start the stopwatch again
        }  
      }
      
      if(digitalRead(StartSwitch) == 0){  //Check to see if start button has been pressed
        ArduinoState = MainProg;  //Switch to the main dechlorinator program
      }
      
      if(digitalRead(StartSwitch) == 0){  //Check to see if start button has been pressed
        ArduinoState = MainProg;  //Switch to the main dechlorinator program
      }
      break;
    default:
       break;
  };
  /*
  lcdSerial.write(254);
  lcdSerial.write(128);
  lcdSerial.write("Press Start");
  lcdSerial.write(254);
  lcdSerial.write(192);
  lcdSerial.print("To Begin");
  if(digitalRead(11) == 0){
    for (int i = 0; i <= 400; i++)
    {
      md.setM1Speed(i);
      if (i%200 == 100)
      {
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
        lcdClear();
        lcdSerial.write(254);
        lcdSerial.write(128);
        lcdSerial.write("Motor 1 Current:");
        lcdSerial.write(254);
        lcdSerial.write(192);
        lcdSerial.print(md.getM1CurrentMilliamps());
      }
      delay(5);
      if(i == 400){
        while(1){
          Serial.print("M1 current: ");
          Serial.println(md.getM1CurrentMilliamps());
          lcdClear();
          lcdSerial.write(254);
          lcdSerial.write(128);
          lcdSerial.write("Motor 1 Current:");
          lcdSerial.write(254);
          lcdSerial.write(192);
          lcdSerial.print(md.getM1CurrentMilliamps());
          delay(100);
          if(digitalRead(3) == 0){
            i = 0;
            break;
          }
        }
      }  
    }
  }
  */
}
