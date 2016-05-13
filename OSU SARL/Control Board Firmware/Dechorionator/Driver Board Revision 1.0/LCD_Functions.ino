void LcdMoveToTop(){
  LcdSerial.write(254);  //Put LCD into move mode
  LcdSerial.write(128);  //Move to the first character of the first row
}

void LcdMoveToBottom(){
  LcdSerial.write(254);  //Put LCD into move mode
  LcdSerial.write(192);  //Move to the first character of the second row
}

void FullLcdClear(){
  LcdSerial.write(254); //Send special command character
  LcdSerial.write(1);   //Send Clear Screen Command
}

void TopLcdClear(){
  LcdMoveToTop();
  LcdSerial.write("                ");  //Write a blank 16 character string
}

void BottomLcdClear(){
  LcdMoveToBottom();
  LcdSerial.write("                ");  //Write a blank 16 character string
}

void FullLcdWrite(const char* TopLcd, const char* BottomLcd){
  TopLcdWrite(TopLcd);
  BottomLcdWrite(BottomLcd);
}

void TopLcdWrite(const char *TopLcd){
  /*
  unsigned char string[16];
  Serial.write("Size of string: ");
  Serial.write(sizeof(TopLcd));
  delay(5000);
  for(int i = 0 ; i < sizeof(TopLcd) ; i++){
      string[i] = TopLcd[i];
  }
    
  if(sizeof(TopLcd) < 16){    
    for(int i = sizeof(TopLcd) ; i < 16 ; i++){
      string[i] = 32;
    }
  }
  */
  LcdMoveToTop();
  LcdSerial.write(TopLcd);  //Write string
  
}

void BottomLcdWrite(const char *BottomLcd){
  LcdMoveToBottom();
  LcdSerial.write(BottomLcd);  //Write string
}

