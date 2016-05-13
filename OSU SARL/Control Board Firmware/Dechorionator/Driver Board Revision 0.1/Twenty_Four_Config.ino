void ShowTFConfigMainScreen(){
  FullLcdWrite((unsigned char*)pgm_read_word(&TwoFourDechorionationConfig[SelectedConfig][0]), (unsigned char*)pgm_read_word(&TwoFourDechorionationConfig[SelectedConfig][1]));
}

void ShowTFConfigIndividualConfigScreen(){
  LcdFullWriteString((unsigned char*)pgm_read_word(&TwoFourDechorionationConfig[SelectedConfig][2]), ConfigVal);
}

int TFConfigMainConstrain(signed int Constrain){
  int Constraint = NumTFConfig - 1;
  if(Constrain > Constraint){
    Constrain = (NumTFConfig - 1);
  }else if(Constrain < 0){
    Constrain = 0;
  }
  return Constrain;
}

int TFConfigIndividualConstrain(signed int Constrain){
  int Constraint = (int)pgm_read_word(&TwoFourDechorionationConfig[SelectedConfig][3]);
  if(Constrain > Constraint){
    Constrain = (int)pgm_read_word(&TwoFourDechorionationConfig[SelectedConfig][3]);
  }else if(Constrain < 0){
    Constrain = 0;
  }
  return Constrain;
}

void TFConfigLoadFromEEPROM(){
  for(int i = 0 ; i < NumTFConfig ; i++){
   EEPROMConfigTF[i] = EEPROM.read(pgm_read_word(&TwoFourDechorionationConfig[i][4]));
  }
}

void TFConfigSaveToEEPROM(){
  for(int i = 0 ; i < NumTFConfig ; i++){
    EEPROM.write(pgm_read_word(&TwoFourDechorionationConfig[i][4]),EEPROMConfigTF[i]);
  }
}
