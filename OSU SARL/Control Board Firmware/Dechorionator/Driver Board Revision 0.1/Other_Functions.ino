void ReadFromEEPROM(){
  SDConfigLoadFromEEPROM();
  TFConfigLoadFromEEPROM();
  PnRConfigLoadFromEEPROM();
  ROConfigLoadFromEEPROM();
}

void SaveToEEPROM(){
  SDConfigSaveToEEPROM();
  TFConfigSaveToEEPROM();
  PnRConfigSaveToEEPROM();
  ROConfigSaveToEEPROM();
}

void DisplayProgramList(){
  FullLcdWrite((unsigned char*)pgm_read_word(&ProgramNames[SelectedProgram][0]), (unsigned char*)pgm_read_word(&ProgramNames[SelectedProgram][1]));  
}

void GetInitialEncoderData(){
  EncoderChannelALastValue = EncoderReferenceVariable = digitalRead(EncoderChannelA);  
}

signed char ReturnEncoderDirection(){
  EncoderReferenceVariable = digitalRead(EncoderChannelA);
  if((EncoderChannelALastValue == LOW) && (EncoderReferenceVariable == HIGH)){
    if(digitalRead(EncoderChannelB) == LOW){
      EncoderChannelALastValue = EncoderReferenceVariable;;
      return 1;
    }else{
      EncoderChannelALastValue = EncoderReferenceVariable;
      return -1;
    }  
  }
  EncoderChannelALastValue = EncoderReferenceVariable;
  return 0;  
}

void ClearAndRestartStopWatch(){
  MyStopWatch.reset();
  MyStopWatch.start();
}

void SoftStopMotor(unsigned char Motor){
  if(Motor == 1){
    for( ; DriveLoop != RotationMinDriveSpeed ; DriveLoop--){
      MotorDriver.setM1Speed(DriveLoop);
    }
    MotorDriver.setM1Speed(0);   
  }
  else if(Motor == 2){
    for( ; DriveLoop != PumpMinDriveSpeed ; DriveLoop--){
      MotorDriver.setM2Speed(DriveLoop);
    }
    MotorDriver.setM2Speed(0);
  }  
}
