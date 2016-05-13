void InitializePins(){
  pinMode(LedGreen, OUTPUT); //Set Green LED Pin to Output
  pinMode(LedRed, OUTPUT); //Set Red LED Pin to Output
  
  pinMode(StartSwitch, INPUT); //Set Start Switch Pin to Input
  pinMode(StopSwitch, INPUT); //Set Stop Switch Pin to Input
  digitalWrite(StartSwitch, HIGH); //Set the internal pullup resistor for the switch
  digitalWrite(StopSwitch, HIGH);  //Same for this one
  
  pinMode(EncoderChannelA, INPUT); //Set Encoder Channel A Pin to Input
  pinMode(EncoderChannelB, INPUT); //Set Encoder Channel B Pin to Input
}
