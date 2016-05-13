void ShowROConfigMainScreen(){
  FullLcdWrite((unsigned char*)pgm_read_word(&RinseOnlyConfig[SelectedConfig][0]), (unsigned char*)pgm_read_word(&RinseOnlyConfig[SelectedConfig][1]));
}

void ShowROConfigIndividualConfigScreen(){
  LcdFullWriteString((unsigned char*)pgm_read_word(&RinseOnlyConfig[SelectedConfig][2]), ConfigVal);
}

int ROConfigMainConstrain(signed int Constrain){
  int Constraint = NumROConfig - 1;
  if(Constrain > Constraint){
    Constrain = (NumROConfig - 1);
  }else if(Constrain < 0){
    Constrain = 0;
  }
  return Constrain;
}

int ROConfigIndividualConstrain(signed int Constrain){
  int Constraint = (int)pgm_read_word(&RinseOnlyConfig[SelectedConfig][3]);
  if(Constrain > Constraint){
    Constrain = (int)pgm_read_word(&RinseOnlyConfig[SelectedConfig][3]);
  }else if(Constrain < 0){
    Constrain = 0;
  }
  return Constrain;
}

void ROConfigLoadFromEEPROM(){
  for(int i = 0 ; i < NumROConfig ; i++){
   EEPROMConfigRO[i] = EEPROM.read(pgm_read_word(&RinseOnlyConfig[i][4]));
  }
}

void ROConfigSaveToEEPROM(){
  for(int i = 0 ; i < NumROConfig ; i++){
    EEPROM.write(pgm_read_word(&RinseOnlyConfig[i][4]),EEPROMConfigRO[i]);
  }
}
