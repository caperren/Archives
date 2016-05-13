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

void FullLcdWrite(unsigned char* TopLcd, unsigned char* BottomLcd){
  TopLcdWrite(TopLcd);
  BottomLcdWrite(BottomLcd);
}

void TopLcdWrite(unsigned char *TopLcd){
  LcdMoveToTop();
  LcdSerial.write((const char*)TopLcd);  //Write string
  
}

void BottomLcdWrite(unsigned char *BottomLcd){
  LcdMoveToBottom();
  LcdSerial.write((const char*)BottomLcd);  //Write string
}

void LcdFullWriteString(unsigned char* TopLcd, String BottomLcd){
  TopLcdWrite(TopLcd);
  LcdMoveToBottom();
  LcdSerial.print(BottomLcd);
}

void LcdWriteSavingScreen(){
  FullLcdWrite((unsigned char*)" Settings Saved ",(unsigned char*)"  Sucessfully   ");
  delay(1500);
  
}
