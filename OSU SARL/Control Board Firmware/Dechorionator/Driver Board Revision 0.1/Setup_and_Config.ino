void InitializePins(){
  pinMode(StartSwitch, INPUT); //Set Start Switch Pin to Input
  pinMode(StopSwitch, INPUT); //Set Stop Switch Pin to Input
  digitalWrite(StartSwitch, HIGH); //Set the internal pullup resistor for the switch
  digitalWrite(StopSwitch, HIGH);  //Set the internal pullup resistor for the switch
  
  pinMode(EncoderLed, OUTPUT); //Set LED Pin to Output
  pinMode(EncoderButton, INPUT); //Set Encoder Button Pin to Input
  pinMode(EncoderChannelA, INPUT); //Set Encoder Channel A Pin to Input
  pinMode(EncoderChannelB, INPUT); //Set Encoder Channel B Pin to Input
  digitalWrite(EncoderLed, HIGH);
  digitalWrite(EncoderButton, HIGH); //Set the internal pullup resistor for the switch
  digitalWrite(EncoderChannelA, HIGH); //Set the internal pullup resistor for the switch
  digitalWrite(EncoderChannelB, HIGH); //Set the internal pullup resistor for the switch
}
