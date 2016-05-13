///////////////////////////////////////////////////////////////////////////////////////////////
//Includes, Defines, Instantiations, and Global Variables//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Includes
#include "DualVNH5019MotorShield.h"                  //Library for the motor driver shield
#include <EEPROM.h>                                  //EEPROM Storage Library for storing config changes
#include <SoftwareSerial.h>                          //Software Serial Library needed for lcd communications
#include <StopWatch.h>                               //Stopwatch library for timing purposes
#include <avr/pgmspace.h>                            //PROGMEM library for storage of config
#include <String.h>

//Pin Definitions (Arduino Suggested Method)
const unsigned char LcdTX = 5;                       //Pin connected to LCD RX Line

const unsigned char StartSwitch = 11;                //Pin connected to start switch
const unsigned char StopSwitch = 13;                 //Pin connected to stop switch

const unsigned char EncoderLed = A2;                 //Pin connected to rotary encoder led
const unsigned char EncoderButton = A3;              //Pin connected to rotary encoder led
const unsigned char EncoderChannelB = A4;            //Pin connected to channel A of the rotary encoder
const unsigned char EncoderChannelA = A5;            //Pin connected to channel B of the rotary encoder

//Instantiations
SoftwareSerial LcdSerial(3, LcdTX);                  //Software Serial Instantiation with Pin 3 as dummy for RX
DualVNH5019MotorShield MotorDriver;                  //Motor Driver Library Instantiation
StopWatch MyStopWatch(StopWatch::SECONDS);           //StopWatch Instantiation for measuring time

enum ArduinoPrograms{                                //Enumeration for defining the arduino program states
  StandardDechorionation = 0,                        //This is the standard Dechorianation Program
  TwoFourDechorionation = 1,                         //Used for embryo's 24 hours old
  PronaseAndRinse = 2,                               //This is an alternating pronase and rinse cycle
  RinseOnly = 3,                                     //This only does the rinse cycle
  SDConfig = 4,                                      //Config page for the standard dechorionation
  TFConfig = 5,                                      //Config page for the 24 hour 
  PnRConfig = 6,                                     //Config page for Pronase and Rinse
  ROConfig = 7,                                      //Config page for Rinse Only
  ProgramSelection = 8                               //Presents a program selection screen
} ArduinoProgram = ProgramSelection;                 //This selects the arduino program to run

//Global Variables and Constants and Configs
const unsigned char TitleTimeout = 2;                //Time in seconds to display the main device title
const unsigned char LcdPageDelay = 3;                //Seconds to wait before the lcd changes pages.
const unsigned char ConfigPageTimeout = 5;           //Time in seconds to wait before dropping the user back to the program selection screen
const unsigned int DebounceTimeout = 300;            //Timeout in milliseconds of the button debounce delay

unsigned char DoneOnce = 0;                          //Variable to store whether something has happened or not.
unsigned char EStop = 0;                             //Variable to store the status of the system's estop button
unsigned char CycleSkip = 0;                         //Variable to store whether the user wants to skip the current cycle of a program.

signed char SelectedProgram = 0;                     //Holds the current program selection. This is not the currently active program.
signed char SelectedConfig = 0;                      //Hold the current config selection.
unsigned char PreviousConfig = 0;                    //A value to hold the previous config value. Used for smoothing the rotary encoder reading.

unsigned char EncoderChannelALastValue = LOW;        //This variable stores the previous state of channel a on the rotary encoder. This is needed to determine direction.
unsigned char EncoderReferenceVariable = LOW;        //This is a dummy varaible for direction determination on for the rotary encoder.

String ConfigVal;                                    //This is used to store the value returned from PROGMEM and converting it to a char array.
int NewVal;                                          //This holds the new value
int PrevVal;                                         //This holds the previous value when changing configs

const unsigned int MaxDriveSpeed = 400;              //Max drive speed value given by the motor driver library
const unsigned int RotationMinDriveSpeed = 75;       //Minimum value to get the motor to turn
const unsigned int PumpMinDriveSpeed = 75;           //Minimum value to get the motor to turn
const unsigned int PumpMaxDriveSpeed = 180;
unsigned int MaxRampValue = 300;                     //The maximum reasonable value used when ramping the motors
unsigned int MaxPumpRampValue = 150;
unsigned int MinRampValue = 1;                       //The minimum reasonable value used when ramping the motors
unsigned int CycleSeconds;                           //Stores the seconds that each cycle should run for.
unsigned int DriveLoop;                              //Looping variable for driving motors


//| Program List Top | Program List Bottom |
PROGMEM prog_char* ProgramNames[][2] = {            //Strings for program names shown on the LCD
  {"1:   Standard   ", "  Dechorionation"},
  {"2: 24 Hour Old  ", "  Dechorionation"},
  {"3:  6 Hour HPF  ", "  Dechorionation"},         //Note that "Pronase and Rinse" has been changed to this.
  {"4: Rinse Cycle  ", "      Only      "}
};

// |  Main Page Top   |  Main Page Bottom  | Adjust Page Top |        Max Value      |   EEPROM Address  |
PROGMEM const prog_char* StandardDechorionationConfig[][5] = {
  {"Pronase Cycle 1 ", "    Minutes     ", "    Minutes     ", (const prog_char*)256, (const prog_char*)0},
  {"Pronase Cycle 1 ", "    Seconds     ", "    Seconds     ", (const prog_char*)60, (const prog_char*)1},
  {"  Rinse Cycle   ", "    Minutes     ", "    Minutes     ", (const prog_char*)256, (const prog_char*)2},
  {"  Rinse Cycle   ", "    Seconds     ", "    Seconds     ", (const prog_char*)60, (const prog_char*)3},
  {"Pronase Cycle 2 ", "    Minutes     ", "    Minutes     ", (const prog_char*)256, (const prog_char*)4},
  {"Pronase Cycle 2 ", "    Seconds     ", "    Seconds     ", (const prog_char*)60, (const prog_char*)5},
  {" Rotation Speed ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)7},
  {"Rotation Ramping", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)6},
  {"   Pump Speed   ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)8},
  {"   Pump Ramp    ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)9}
};

const unsigned char NumSDConfig = 10;               //Stores the maximum number of available configs

unsigned char EEPROMConfigSD[NumSDConfig] = {       //User Modifiable Standard Dechorionation Config Array
  6,                                                //Values correspond to each cycle above, in same array order                                 
  30,                                               //Default values here are for initial EEPROM Programming
  1,
  0,
  10,
  0,
  50,
  75,
  100,
  100
};

// |  Main Page Top   |  Main Page Bottom  | Adjust Page Top |        Max Value      |   EEPROM Address  |
PROGMEM const prog_char* TwoFourDechorionationConfig[][5] = {
  {"Pronase Cycle 1 ", "    Minutes     ", "    Minutes     ", (const prog_char*)256, (const prog_char*)10},
  {"Pronase Cycle 1 ", "    Seconds     ", "    Seconds     ", (const prog_char*)60, (const prog_char*)11},
  {"  Rinse Cycle   ", "    Minutes     ", "    Minutes     ", (const prog_char*)256, (const prog_char*)12},
  {"  Rinse Cycle   ", "    Seconds     ", "    Seconds     ", (const prog_char*)60, (const prog_char*)13},
  {"Pronase Cycle 2 ", "    Minutes     ", "    Minutes     ", (const prog_char*)256, (const prog_char*)14},
  {"Pronase Cycle 2 ", "    Seconds     ", "    Seconds     ", (const prog_char*)60, (const prog_char*)15},
  {" Rotation Speed ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)16},
  {"Rotation Ramping", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)17},
  {"   Pump Speed   ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)18},
  {"   Pump Ramp    ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)19}
};

const unsigned char NumTFConfig = 10;               //Stores the maximum number of available configs

unsigned char EEPROMConfigTF[NumTFConfig] = {       //User Modifiable 24 Hour Dechorionation Config Array
  3,                                                //Values correspond to each cycle above, in same array order                                 
  0,                                                //Default values here are for initial EEPROM Programming
  1,
  0,
  10,
  0,
  85,
  85,
  85,
  85
};

// |  Main Page Top   |  Main Page Bottom  | Adjust Page Top |        Max Value      |   EEPROM Address  |
PROGMEM const prog_char* PronaseAndRinseConfig[][5] = {
  {"Pronase Cycle 1 ", "    Minutes     ", "    Minutes     ", (const prog_char*)256, (const prog_char*)10},
  {"Pronase Cycle 1 ", "    Seconds     ", "    Seconds     ", (const prog_char*)60, (const prog_char*)11},
  {"  Rinse Cycle   ", "    Minutes     ", "    Minutes     ", (const prog_char*)256, (const prog_char*)12},
  {"  Rinse Cycle   ", "    Seconds     ", "    Seconds     ", (const prog_char*)60, (const prog_char*)13},
  {"Pronase Cycle 2 ", "    Minutes     ", "    Minutes     ", (const prog_char*)256, (const prog_char*)14},
  {"Pronase Cycle 2 ", "    Seconds     ", "    Seconds     ", (const prog_char*)60, (const prog_char*)15},
  {" Rotation Speed ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)16},
  {"Rotation Ramping", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)17},
  {"   Pump Speed   ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)18},
  {"   Pump Ramp    ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)19} 
};

const unsigned char NumPnRConfig = 10;

unsigned char EEPROMConfigPnR[NumPnRConfig] = {       //User Modifiable Pronase and Rinse Config Array
  6,                                                  //Values correspond to each cycle above, in same array order                                 
  30,                                                 //Default values here are for initial EEPROM Programming
  1,
  0,
  10,
  0,
  85,
  85,
  85,
  85
};

// |  Main Page Top   |  Main Page Bottom  | Adjust Page Top |        Max Value      |   EEPROM Address  |
PROGMEM const prog_char* RinseOnlyConfig[][5] = {
  {"  Rinse Cycle   ", "    Minutes     ", "    Minutes     ", (const prog_char*)256, (const prog_char*)30},
  {"  Rinse Cycle   ", "    Seconds     ", "    Seconds     ", (const prog_char*)60, (const prog_char*)31},
  {"   Pump Speed   ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)32},
  {"   Pump Ramp    ", "   Percentage   ", "    Percent     ", (const prog_char*)100, (const prog_char*)33}
};

const unsigned char NumROConfig = 4;

unsigned char EEPROMConfigRO[NumROConfig] = {         //User Modifiable Standard Dechorionation Config Array
  1,                                                  //Values correspond to each cycle above, in same array order
  0,                                                  //Default values here are for initial EEPROM Programming
  85,
  85
};
///////////////////////////////////////////////////////////////////////////////////////////////
//Arduino Setup////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void setup(){
  MotorDriver.init();                                                                    //Initialize the VNH5019 Motor Driving Shield
  InitializePins();                                                                      //Initialize pins for switches, rotary encoder, and leds
  Serial.begin(115200);                                                                  //Start communicating to PC at 115200 baud
  LcdSerial.begin(9600);                                                                 //Start communicating with LCD dispaly at 9600 buad
  delay(TitleTimeout* 500);                                                              //Wait for Lcd to initialize
  FullLcdClear();                                                                        //Clear the LCD to remove artifacts
  FullLcdWrite((unsigned char*)"  Tanguay Labs  ", (unsigned char*)"Dechorionator #2");  //Show the device name
  Serial.println("Dechorionator Initialized");                                           //Debug on Serial
  delay(TitleTimeout* 1000);                                                             //Wait on name screen
  FullLcdClear();                                                                        //Clear the LCD
  //SaveToEEPROM();                                                                      //Used to write initial EEPROM configuration. It should no longer be needed.
  ReadFromEEPROM();                                                                      //Reads user configured settings from EEPROM into config arrays
  GetInitialEncoderData();
}

///////////////////////////////////////////////////////////////////////////////////////////////
//Main Arduino Program Loop////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void loop(){
  switch(ArduinoProgram){
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
    case ProgramSelection:
      if(DoneOnce != 1){ ShowProgramListIntro();}                                        //Show an intro screen if the device has just turned on
      DisplayProgramList();                                                              //Show the currently selected program title
      if(CheckProgramStart() | CheckConfigButton()){                                     //If the start button or configuration button are pressed, immediately switch to the new program
        SelectedConfig = PreviousConfig = 0;                                             //Set config variables to 0 in preperation
        break;                                                                           //Immediately break if a button has been pressed
      }                              
      SelectedProgram = ConstrainProgramList(SelectedProgram + ReturnEncoderDirection());//Determine if the encoder has moved, and change the program display if it has
      break;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////      
    case StandardDechorionation:
      EStop = 0;
     
//Pronase Cycle 1 
      FullLcdWrite((unsigned char*)"    Starting    ",(unsigned char*)"Pronase Cycle 1 ");
      delay(LcdPageDelay*500);
      CycleSkip = 0;
      ClearAndRestartStopWatch();      
      CycleSeconds = ((60*EEPROMConfigSD[0]) + EEPROMConfigSD[1]);
      while(MyStopWatch.elapsed() < CycleSeconds & !EStop & !CycleSkip){
        for(DriveLoop = RotationMinDriveSpeed ; DriveLoop < MaxDriveSpeed ; DriveLoop = DriveLoop + ((EEPROMConfigSD[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigSD[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          } 
        }
        for( ; DriveLoop >  RotationMinDriveSpeed ; DriveLoop = DriveLoop - ((EEPROMConfigSD[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigSD[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          }
        }
         
      }
      SoftStopMotor(1);  //Stops Motor 1, rotation motor

//Rinse Cycle   
      if(!EStop){
        FullLcdWrite((unsigned char*)"    Starting    ",(unsigned char*)"  Rinse Cycle   ");
        delay(LcdPageDelay*500);
        CycleSkip = 0;
        ClearAndRestartStopWatch();
        CycleSeconds = ((60*EEPROMConfigSD[2]) + EEPROMConfigSD[3]);
        
        for(DriveLoop = PumpMinDriveSpeed ; DriveLoop < PumpMaxDriveSpeed ; DriveLoop = DriveLoop + (((EEPROMConfigSD[9]/4)*MaxPumpRampValue)/100)){
          MotorDriver.setM2Speed((EEPROMConfigSD[8]*DriveLoop)/100);
          delay(250);
        }
        MotorDriver.setM2Speed((EEPROMConfigSD[8]*PumpMaxDriveSpeed)/100);
      }
      while(MyStopWatch.elapsed() < CycleSeconds & !EStop & !CycleSkip){
        LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
        if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
        }  
      }
      SoftStopMotor(2);
      
//Pronase Cycle 2    
      if(!EStop){
        FullLcdWrite((unsigned char*)"    Starting    ",(unsigned char*)"Pronase Cycle 2 ");
        delay(LcdPageDelay*500);
        CycleSkip = 0;
        ClearAndRestartStopWatch();      
        CycleSeconds = ((60*EEPROMConfigSD[4]) + EEPROMConfigSD[5]);
      }
      while(MyStopWatch.elapsed() < CycleSeconds & !EStop & !CycleSkip){
        for(DriveLoop = RotationMinDriveSpeed ; DriveLoop < MaxDriveSpeed ; DriveLoop = DriveLoop + ((EEPROMConfigSD[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigSD[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          } 
        }
        for( ; DriveLoop >  RotationMinDriveSpeed ; DriveLoop = DriveLoop - ((EEPROMConfigSD[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigSD[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          }
        }
         
      }
      SoftStopMotor(1);  //Stops Motor 1, rotation motor
      
      
      EStop = 0;
      GoToProgramSelection();
      break;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
    case TwoFourDechorionation:
      
      EStop = 0;
     
//Pronase Cycle 1 
      FullLcdWrite((unsigned char*)"    Starting    ",(unsigned char*)"Pronase Cycle 1 ");
      delay(LcdPageDelay*500);
      CycleSkip = 0;
      ClearAndRestartStopWatch();      
      CycleSeconds = ((60*EEPROMConfigTF[0]) + EEPROMConfigTF[1]);
      while(MyStopWatch.elapsed() < CycleSeconds & !EStop & !CycleSkip){
        for(DriveLoop = RotationMinDriveSpeed ; DriveLoop < MaxDriveSpeed ; DriveLoop = DriveLoop + ((EEPROMConfigTF[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigTF[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          } 
        }
        for( ; DriveLoop >  RotationMinDriveSpeed ; DriveLoop = DriveLoop - ((EEPROMConfigTF[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigTF[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          }
        }
         
      }
      SoftStopMotor(1);  //Stops Motor 1, rotation motor

//Rinse Cycle   
      if(!EStop){
        FullLcdWrite((unsigned char*)"    Starting    ",(unsigned char*)"  Rinse Cycle   ");
        delay(LcdPageDelay*500);
        CycleSkip = 0;
        ClearAndRestartStopWatch();
        CycleSeconds = ((60*EEPROMConfigTF[2]) + EEPROMConfigTF[3]);
        
        for(DriveLoop = PumpMinDriveSpeed ; DriveLoop < PumpMaxDriveSpeed ; DriveLoop = DriveLoop + ((EEPROMConfigTF[9]*MaxPumpRampValue)/100)){
          MotorDriver.setM2Speed((EEPROMConfigTF[8]*DriveLoop)/100);
        }
        MotorDriver.setM2Speed((EEPROMConfigTF[8]*PumpMaxDriveSpeed)/100);
      }
      while(MyStopWatch.elapsed() < CycleSeconds & !EStop & !CycleSkip){
        LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
        if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
        }  
      }
      SoftStopMotor(2);
      
//Pronase Cycle 2    
      if(!EStop){
        FullLcdWrite((unsigned char*)"    Starting    ",(unsigned char*)"Pronase Cycle 2 ");
        delay(LcdPageDelay*500);
        CycleSkip = 0;
        ClearAndRestartStopWatch();      
        CycleSeconds = ((60*EEPROMConfigTF[4]) + EEPROMConfigTF[5]);
      }
      while(MyStopWatch.elapsed() < CycleSeconds & !EStop & !CycleSkip){
        for(DriveLoop = RotationMinDriveSpeed ; DriveLoop < MaxDriveSpeed ; DriveLoop = DriveLoop + ((EEPROMConfigTF[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigTF[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          } 
        }
        for( ; DriveLoop >  RotationMinDriveSpeed ; DriveLoop = DriveLoop - ((EEPROMConfigTF[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigTF[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          }
        }
         
      }
      SoftStopMotor(1);  //Stops Motor 1, rotation motor
      
      
      EStop = 0;
      GoToProgramSelection();
      break;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////    
    case PronaseAndRinse:
      EStop = 0;
     
//Pronase Cycle 1
      FullLcdWrite((unsigned char*)"    Starting    ",(unsigned char*)"Pronase Cycle 1 ");
      delay(LcdPageDelay*500);
      CycleSkip = 0;
      ClearAndRestartStopWatch();      
      CycleSeconds = ((60*EEPROMConfigPnR[0]) + EEPROMConfigPnR[1]);
      while(MyStopWatch.elapsed() < CycleSeconds & !EStop & !CycleSkip){
        for(DriveLoop = RotationMinDriveSpeed ; DriveLoop < MaxDriveSpeed ; DriveLoop = DriveLoop + ((EEPROMConfigPnR[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigPnR[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          } 
        }
        for( ; DriveLoop >  RotationMinDriveSpeed ; DriveLoop = DriveLoop - ((EEPROMConfigPnR[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigPnR[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          }
        }
         
      }
      SoftStopMotor(1);  //Stops Motor 1, rotation motor

//Rinse Cycle   
      if(!EStop){
        FullLcdWrite((unsigned char*)"    Starting    ",(unsigned char*)"  Rinse Cycle   ");
        delay(LcdPageDelay*500);
        CycleSkip = 0;
        ClearAndRestartStopWatch();
        CycleSeconds = ((60*EEPROMConfigPnR[2]) + EEPROMConfigPnR[3]);
        
        for(DriveLoop = PumpMinDriveSpeed ; DriveLoop < PumpMaxDriveSpeed ; DriveLoop = DriveLoop + ((EEPROMConfigPnR[9]*MaxPumpRampValue)/100)){
          MotorDriver.setM2Speed((EEPROMConfigPnR[8]*DriveLoop)/100);
        }
        MotorDriver.setM2Speed((EEPROMConfigPnR[8]*PumpMaxDriveSpeed)/100);
      }
      while(MyStopWatch.elapsed() < CycleSeconds & !EStop & !CycleSkip){
        LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
        if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
        }  
      }
      SoftStopMotor(2);
      
//Pronase Cycle 2    
      if(!EStop){
        FullLcdWrite((unsigned char*)"    Starting    ",(unsigned char*)"Pronase Cycle 2 ");
        delay(LcdPageDelay*500);
        CycleSkip = 0;
        ClearAndRestartStopWatch();      
        CycleSeconds = ((60*EEPROMConfigPnR[4]) + EEPROMConfigPnR[5]);
      }
      while(MyStopWatch.elapsed() < CycleSeconds & !EStop & !CycleSkip){
        for(DriveLoop = RotationMinDriveSpeed ; DriveLoop < MaxDriveSpeed ; DriveLoop = DriveLoop + ((EEPROMConfigPnR[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigPnR[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          } 
        }
        for( ; DriveLoop >  RotationMinDriveSpeed ; DriveLoop = DriveLoop - ((EEPROMConfigPnR[7]*MaxRampValue)/100)){
          MotorDriver.setM1Speed((DriveLoop * EEPROMConfigPnR[6])/100);
          LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
          if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
          }
        }
         
      }
      SoftStopMotor(1);  //Stops Motor 1, rotation motor
      
      
      EStop = 0;
      GoToProgramSelection();
      break;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////    
    case RinseOnly:
      EStop = 0;
      
      if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
      FullLcdWrite((unsigned char*)"    Starting    ",(unsigned char*)"  Rinse Cycle   ");
      
      if(!EStop){
        delay(LcdPageDelay*500);
        CycleSkip = 0;
        ClearAndRestartStopWatch();
        CycleSeconds = ((60*EEPROMConfigRO[0]) + EEPROMConfigRO[1]);
        
        for(DriveLoop = PumpMinDriveSpeed ; DriveLoop < PumpMaxDriveSpeed ; DriveLoop = DriveLoop + ((EEPROMConfigRO[3]*MaxPumpRampValue)/100)){
          MotorDriver.setM2Speed((EEPROMConfigRO[2]*DriveLoop)/100);
        }
        MotorDriver.setM2Speed((EEPROMConfigRO[8]*PumpMaxDriveSpeed)/100);
      }
      while(MyStopWatch.elapsed() < CycleSeconds & !EStop & !CycleSkip){
        LcdFullWriteString((unsigned char*)" Time Remaining ",("      " + String((CycleSeconds - MyStopWatch.elapsed()), DEC) + "         "));
        if(digitalRead(StopSwitch) == LOW){
            SoftStopMotor(1);
            GoToProgramSelection();
            EStop = 1;
            break; 
          }
          if(digitalRead(StartSwitch) == LOW){
            delay(DebounceTimeout);
            CycleSkip = 1;
            break;
        }  
      }
      SoftStopMotor(2);
      
      EStop = 0;
      GoToProgramSelection();
      break;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////    
    case SDConfig:
      ClearAndRestartStopWatch();;
      
      while(MyStopWatch.elapsed() < (ConfigPageTimeout)){
        if(PreviousConfig != SelectedConfig ){
          ClearAndRestartStopWatch();
          
        }
        
        ShowSDConfigMainScreen();
        if(digitalRead(EncoderButton) == LOW){
          MyStopWatch.stop();
          delay(DebounceTimeout);
          PrevVal = NewVal = (int)EEPROMConfigSD[SelectedConfig];
          ConfigVal = "      " + String(NewVal, DEC) + "         ";
          ShowSDConfigIndividualConfigScreen();
          while(digitalRead(EncoderButton) == HIGH){
            NewVal = SDConfigIndividualConstrain(NewVal + ReturnEncoderDirection());
            if(NewVal != PrevVal){
              PrevVal = NewVal;
              ConfigVal = "      " + String(NewVal, DEC) + "         ";
              ShowSDConfigIndividualConfigScreen(); 
            }        
          }
          EEPROMConfigSD[SelectedConfig] = NewVal;
          delay(DebounceTimeout);
          ClearAndRestartStopWatch();
          
        }    
        PreviousConfig = SelectedConfig;
        SelectedConfig = SDConfigMainConstrain(SelectedConfig += ReturnEncoderDirection());   

        if(digitalRead(StartSwitch) == LOW){
          GoToProgramSelection();
          SDConfigSaveToEEPROM();
          LcdWriteSavingScreen();
          break; 
        }
        if(digitalRead(StopSwitch) == LOW){
          GoToProgramSelection();
          break; 
        }
      }
      GoToProgramSelection();
      
      break;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////    
    case TFConfig:
      ClearAndRestartStopWatch();
      
      while(MyStopWatch.elapsed() < (ConfigPageTimeout)){
        if(PreviousConfig != SelectedConfig ){
          ClearAndRestartStopWatch();
          
        }
        
        ShowTFConfigMainScreen();
        if(digitalRead(EncoderButton) == LOW){
          MyStopWatch.stop();
          delay(DebounceTimeout);
          PrevVal = NewVal = (int)EEPROMConfigTF[SelectedConfig];
          ConfigVal = "      " + String(NewVal, DEC) + "         ";
          ShowTFConfigIndividualConfigScreen();
          while(digitalRead(EncoderButton) == HIGH){
            NewVal = TFConfigIndividualConstrain(NewVal + ReturnEncoderDirection());
            if(NewVal != PrevVal){
              PrevVal = NewVal;
              ConfigVal = "      " + String(NewVal, DEC) + "         ";
              ShowTFConfigIndividualConfigScreen(); 
            }        
          }
          EEPROMConfigTF[SelectedConfig] = NewVal;
          delay(DebounceTimeout);
          ClearAndRestartStopWatch();
          
        }    
        PreviousConfig = SelectedConfig;
        SelectedConfig = TFConfigMainConstrain(SelectedConfig += ReturnEncoderDirection());   

        if(digitalRead(StartSwitch) == LOW){
          GoToProgramSelection();
          TFConfigSaveToEEPROM();
          LcdWriteSavingScreen();
          break; 
        }
        if(digitalRead(StopSwitch) == LOW){
          GoToProgramSelection();
          break; 
        }
      }
      GoToProgramSelection();
      
      break;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////    
    case PnRConfig:
      ClearAndRestartStopWatch();
      
      while(MyStopWatch.elapsed() < (ConfigPageTimeout)){
        if(PreviousConfig != SelectedConfig ){
          ClearAndRestartStopWatch();
          
        }
        
        ShowPnRConfigMainScreen();
        if(digitalRead(EncoderButton) == LOW){
          MyStopWatch.stop();
          delay(DebounceTimeout);
          PrevVal = NewVal = (int)EEPROMConfigPnR[SelectedConfig];
          ConfigVal = "      " + String(NewVal, DEC) + "         ";
          ShowPnRConfigIndividualConfigScreen();
          while(digitalRead(EncoderButton) == HIGH){
            NewVal = PnRConfigIndividualConstrain(NewVal + ReturnEncoderDirection());
            if(NewVal != PrevVal){
              PrevVal = NewVal;
              ConfigVal = "      " + String(NewVal, DEC) + "         ";
              ShowPnRConfigIndividualConfigScreen(); 
            }        
          }
          EEPROMConfigPnR[SelectedConfig] = NewVal;
          delay(DebounceTimeout);
          ClearAndRestartStopWatch();
          
        }    
        PreviousConfig = SelectedConfig;
        SelectedConfig = PnRConfigMainConstrain(SelectedConfig += ReturnEncoderDirection());   

        if(digitalRead(StartSwitch) == LOW){
          GoToProgramSelection();
          PnRConfigSaveToEEPROM();
          LcdWriteSavingScreen();
          break; 
        }
        if(digitalRead(StopSwitch) == LOW){
          GoToProgramSelection();
          break; 
        }
      }
      GoToProgramSelection();
      
      break;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////    
    case ROConfig:
      ClearAndRestartStopWatch();
      
      while(MyStopWatch.elapsed() < (ConfigPageTimeout)){
        if(PreviousConfig != SelectedConfig ){
          ClearAndRestartStopWatch();
          
        }
        
        ShowROConfigMainScreen();
        if(digitalRead(EncoderButton) == LOW){
          MyStopWatch.stop();
          delay(DebounceTimeout);
          PrevVal = NewVal = (int)EEPROMConfigRO[SelectedConfig];
          ConfigVal = "      " + String(NewVal, DEC) + "         ";
          ShowROConfigIndividualConfigScreen();
          while(digitalRead(EncoderButton) == HIGH){
            NewVal = ROConfigIndividualConstrain(NewVal + ReturnEncoderDirection());
            if(NewVal != PrevVal){
              PrevVal = NewVal;
              ConfigVal = "      " + String(NewVal, DEC) + "         ";
              ShowROConfigIndividualConfigScreen(); 
            }        
          }
          EEPROMConfigRO[SelectedConfig] = NewVal;
          delay(DebounceTimeout);
          ClearAndRestartStopWatch();
          
        }    
        PreviousConfig = SelectedConfig;
        SelectedConfig = ROConfigMainConstrain(SelectedConfig += ReturnEncoderDirection());   

        if(digitalRead(StartSwitch) == LOW){
          GoToProgramSelection();
          ROConfigSaveToEEPROM();
          LcdWriteSavingScreen();
          break; 
        }
        if(digitalRead(StopSwitch) == LOW){
          GoToProgramSelection();
          break; 
        }
      }
      GoToProgramSelection();
      break;        
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////    
    default:
      FullLcdWrite((unsigned char*)"Fatal Error   ", (unsigned char*)"Contact Support ");
      while(1);
      break;
  };
}
