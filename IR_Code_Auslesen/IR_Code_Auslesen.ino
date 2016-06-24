/*
IR Code Analyse

https://arduino-info.wikispaces.com/IR-RemoteControl

*/
#include <IRremote.h>

int RECV_PIN = 9;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Button - ");
    Serial.print(results.value, HEX); // print results as HEX
    //Serial.print(" - ");
    //Serial.print(results.value); // print results as decimal
    Serial.println("");
    irrecv.resume(); // Receive the next value
  }
}

/*
Obi Fernbedienungscodes RGB Lichtschlange:

Knoepfe von rechts nach links
ON - FFC03F
OFF - FF40BF
Dimmen - FF807F
Heller - FF00FF
Weiss - FFE01F
Blau - FF609F
Gruen - FFA05F
Rot - FF20DF
FLASH - FFD02F
Blau 1 - FF50AF
Gruen 1 - FF906F
Rot 1 - FF10EF
STROBE - FFF00F
Blau 2 - FF708F
Gruen 2 - FFB04F
Orange 2 - FF30CF
FADE - FFC837
Blau 3 - FF48B7
Dunkelgruen - FF8877
Orange/Gelb - FF08F7
SMOOTH - FFE817
Violett - FF6897
Hellblau - FFA857
Gelb - FF28D7

=====================
Arduino FB codes

AUS - FFA25D
Mode - FF629D
Mute - FFE21D
Play/Pause - FF22DD
Rew - FF02FD
Fwd - FFC23D
EQ - FFE01F
Leise - FFA857
Lauter - FF906F
Num 0 - FF6897
Random - FF9867
U/SD - FFB04F
Num 1 - FF30CF
Num 2 - FF18E7
Num 3 - FF7A85
Num 4 - FF10EF
Num 5 - FF38C7
Num 6 - FF5AA5
Num 7 - FF42BD
Num 8 - FF4AB5
Num 9 - FF52AD

*/
