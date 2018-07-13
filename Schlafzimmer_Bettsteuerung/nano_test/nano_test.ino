/* 
 Zu Steuernde Elemente:
 * 7m RGB LED
 * Lichtsensor
 * 2x PIR Motion Sensor
 * 
 * attiny84 side 5, 7, 8
 * Voltpin = 9
 * 
 * PIRpin links = 0 // arduino nano side = 2
 * PIRpin rechts = 1 // arduino nano side = 3
 * Lichtsensorpin links = 2 //arduino nano side = A1
 * Activity LED = 3 // arduino nano side = 13
 * 
*/
#include <RGBControl.h>
#include <BedLightControl.h>
#include <avr/wdt.h>

byte led = 4;

bedlightcontrol bed(2, 3, A1, 500, 30); // PIRPin1, PIRPin2, Brightnesspin, Brightnessvalue, lighttime in seconds

// activity led
byte ACTLED = 13;


void act(bool on=false){
  /*
   * Activity LED
   */
  if(on){
    digitalWrite(ACTLED, HIGH);
  } else {
    digitalWrite(ACTLED, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  // activity LED
  pinMode(ACTLED, OUTPUT);
  act(true);
  wdt_enable(WDTO_8S);
  
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  delay(1);
  Serial.println(F("setup end..."));
  act();
  wdt_reset();
}

// only for testing
void light(bool on=false){
  if(on){
    digitalWrite(led, HIGH);
    Serial.println(F("Switch Light ON"));
  } else {
    digitalWrite(led, LOW);
    Serial.println(F("Switch Light OFF"));
  }
}


unsigned long currmillis, task2, task3, task4, pirtime;
void loop() {
  currmillis = millis();

  // switch led off or on after time
  if((unsigned long)(currmillis - task2) >= 1000){
    act(true);
    int bedstate = bed.switchON(); // get state
    if(bedstate == 1){
      light(true);
    } else if(bedstate == 0){
      light(false);
    }
    task2 = millis();
    act();
  }

  if((unsigned long)(currmillis - task3) >= 5000){
    act(true);
    Serial.print(F("Lightsensor value = "));
    Serial.println(bed.get_BRIGHTNESSVALUE());
    task3 = millis();
    act();
  }
  wdt_reset();
}



