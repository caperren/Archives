
int mot = 0;
int pot = A1;


void setup() {                
  pinMode(mot, OUTPUT);  
  pinMode(pot, INPUT);  
}


void loop() {
  analogWrite(mot, analogRead(pot)/4);
}

