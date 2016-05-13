void ShowProgramListIntro(){
  FullLcdWrite((unsigned char*)"Select Program, ",(unsigned char*)"Press Start     ");  //Show the start screen
  delay(TitleTimeout* 1000);  //Wait for three seconds
  FullLcdClear();
  DoneOnce  = 1;  //Set DoneOnce to 1 so we know it's been started for the first time
  EStop = 0;  
}

signed char ConstrainProgramList(signed char ProgramList){
  if(ProgramList == -1){
    return 3;
  }else if(ProgramList == 4){
    return 0;
  }
}

unsigned char CheckConfigButton(){
  if(digitalRead(EncoderButton) == LOW){
    delay(DebounceTimeout);
    SelectedConfig = 0;
    ArduinoProgram = ArduinoPrograms((char)SelectedProgram + 4);
    return 1;
  }
 return 0; 
}

unsigned char CheckProgramStart(){
  if(digitalRead(StartSwitch) == LOW){
    ArduinoProgram = (ArduinoPrograms)SelectedProgram;  //Change arduino program selector to the new one
    return 1;
  } 
  return 0;
}

void GoToProgramSelection(){
  delay(DebounceTimeout);
  ArduinoProgram = ProgramSelection;
}

