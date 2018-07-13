/*
   Relais Control demo
   use Arduino Uno Digitalpin 2-9
*/

#include <RelaisControl.h> //import library

// relaiscontrol asign port to variable
byte R1 = 2;
byte R2 = 3;
byte R3 = 4;
byte R4 = 5;
byte R5 = 6;
byte R6 = 7;
byte R7 = 8;
byte R8 = 9;

// init relais
relais relais(R1, R2, R3, R4, R5, R6, R7, R8, true); // Relais port 1-8, invert (if relais is off when Output is HIGH = invert = true)


void setup(){
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void serialtext(int i, bool state){
  Serial.print("Switch Relais ");
  Serial.print(i);
  if(state){
    Serial.println("on");
  } else {
    Serial.println("off");
  }
  
}

void is_relais_on(){
  Serial.println("get sum state if 1 or more relais is on");
  //with function relais.ACTIVE() we become boolean value back (true = on, false = off)
  if(relais.ACTIVE()){
    Serial.println("1 or more relais is on");
  } else {
    Serial.println("all relais off");
  }
}

void loop(){
  Serial.println("start demo program...");
  //we use byte array to shrink code
  byte vararray[8] = {R1, R2, R3, R4, R5, R6, R7, R8};
  
  // part1: switch relais 1-8 on
  Serial.println("Switch Relais 1-8 ON");
  for(int i=0;i<8;i++){
    relais.ON(vararray[i]);
    serialtext(i, true);
    delay(1000);
  }
  Serial.println("all Relais Switched on");
  delay(1000);
  
  // part2: check if one or more relais is on
  is_relais_on();
  delay(1000);

  // part3: switch all relais off
  Serial.println("Switch Relais 1-8 OFF");
  for(int i=0;i<8;i++){
    relais.OFF(vararray[i]);
    serialtext(i, false);
    delay(1000);
  }
  Serial.println("all Relais Switched off");
  delay(1000);

  //part4: check if one or more relais is on
  is_relais_on();
  delay(1000);

  //part5: switch only relais port 3 on
  Serial.println("Switch Relais 3 ON");
  int r = 2;
  relais.ON(vararray[r]);
  serialtext(r, true);
  delay(1000);

  //part6: check if one or more relais is on
  is_relais_on();
  delay(1000);
  
  //part7: switch relais port 3 off
  Serial.println("Switch Relais 3 OFF");
  relais.OFF(vararray[r]);
  serialtext(r, true);
  delay(1000);
  
  //part7: switch relais 1-8 on and then off (just for fun :-) )
  Serial.println("Switch Relais 1-8 ON");
  for(int i=0;i<8;i++){
    relais.ON(vararray[i]);
    serialtext(i, true);
    delay(500);
  }
  Serial.println("all Relais Switched on");
  delay(500);

  is_relais_on();
  delay(500);

  Serial.println("Switch Relais 1-8 OFF");
  for(int i=0;i<8;i++){
    relais.OFF(vararray[i]);
    serialtext(i, false);
    delay(500);
  }
  Serial.println("all Relais Switched off");
  delay(1000);

  // demo finish
  Serial.println("demo program end...");
  Serial.println("restarting demo in 10 seconds");
  delay(10000);
}
