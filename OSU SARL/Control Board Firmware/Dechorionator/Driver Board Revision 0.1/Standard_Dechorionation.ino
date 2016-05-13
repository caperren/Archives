///////////////////////////////////////////////////////////////////////////////////////////////
//Program Functions////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////
//Config Functions/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void ShowSDConfigMainScreen(){
  FullLcdWrite((unsigned char*)pgm_read_word(&StandardDechorionationConfig[SelectedConfig][0]), (unsigned char*)pgm_read_word(&StandardDechorionationConfig[SelectedConfig][1]));
}

void ShowSDConfigIndividualConfigScreen(){
  LcdFullWriteString((unsigned char*)pgm_read_word(&StandardDechorionationConfig[SelectedConfig][2]), ConfigVal);
}

int SDConfigMainConstrain(signed int Constrain){
  int Constraint = NumSDConfig - 1;
  if(Constrain > Constraint){
    Constrain = (NumSDConfig - 1);
  }else if(Constrain < 0){
    Constrain = 0;
  }
  return Constrain;
}

int SDConfigIndividualConstrain(signed int Constrain){
  int Constraint = (int)pgm_read_word(&StandardDechorionationConfig[SelectedConfig][3]);
  if(Constrain > Constraint){
    Constrain = (int)pgm_read_word(&StandardDechorionationConfig[SelectedConfig][3]);
  }else if(Constrain < 0){
    Constrain = 0;
  }
  return Constrain;
}

void SDConfigLoadFromEEPROM(){
  for(int i = 0 ; i < NumSDConfig ; i++){
   EEPROMConfigSD[i] = EEPROM.read(pgm_read_word(&StandardDechorionationConfig[i][4]));
  }
}

void SDConfigSaveToEEPROM(){
  for(int i = 0 ; i < NumSDConfig ; i++){
    EEPROM.write(pgm_read_word(&StandardDechorionationConfig[i][4]),EEPROMConfigSD[i]);
  }
}
