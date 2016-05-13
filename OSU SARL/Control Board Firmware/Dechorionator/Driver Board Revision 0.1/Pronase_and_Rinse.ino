void ShowPnRConfigMainScreen(){
  FullLcdWrite((unsigned char*)pgm_read_word(&PronaseAndRinseConfig[SelectedConfig][0]), (unsigned char*)pgm_read_word(&PronaseAndRinseConfig[SelectedConfig][1]));
}

void ShowPnRConfigIndividualConfigScreen(){
  LcdFullWriteString((unsigned char*)pgm_read_word(&PronaseAndRinseConfig[SelectedConfig][2]), ConfigVal);
}

int PnRConfigMainConstrain(signed int Constrain){
  int Constraint = NumPnRConfig - 1;
  if(Constrain > Constraint){
    Constrain = (NumPnRConfig - 1);
  }else if(Constrain < 0){
    Constrain = 0;
  }
  return Constrain;
}

int PnRConfigIndividualConstrain(signed int Constrain){
  int Constraint = (int)pgm_read_word(&PronaseAndRinseConfig[SelectedConfig][3]);
  if(Constrain > Constraint){
    Constrain = (int)pgm_read_word(&PronaseAndRinseConfig[SelectedConfig][3]);
  }else if(Constrain < 0){
    Constrain = 0;
  }
  return Constrain;
}

void PnRConfigLoadFromEEPROM(){
  for(int i = 0 ; i < NumPnRConfig ; i++){
   EEPROMConfigPnR[i] = EEPROM.read(pgm_read_word(&PronaseAndRinseConfig[i][4]));
  }
}

void PnRConfigSaveToEEPROM(){
  for(int i = 0 ; i < NumPnRConfig ; i++){
    EEPROM.write(pgm_read_word(&PronaseAndRinseConfig[i][4]),EEPROMConfigPnR[i]);
  }
}
