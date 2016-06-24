/*
IR DEMO
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
    Serial.print("VALUE-1 - ");
    Serial.print(results.value, HEX);
    Serial.println("");
    Serial.println("VALUE-2 - ");
    irrecv.resume(); // Receive the next value
  }
}

*/
Obi Fernbedienungscodes RGB Lichtschlange:

Knoepfe von rechts nach links
VALUE-1 - FFC03F
VALUE-1 - FF40BF
VALUE-1 - FF807F
VALUE-1 - FF00FF
VALUE-1 - FFE01F
VALUE-1 - FF609F
VALUE-1 - FFA05F
VALUE-1 - FF20DF
VALUE-1 - FFD02F
VALUE-1 - FF50AF
VALUE-1 - FF906F
VALUE-1 - FF10EF
VALUE-1 - FFF00F
VALUE-1 - FF708F
VALUE-1 - FFB04F
VALUE-1 - FF30CF
VALUE-1 - FFC837
VALUE-1 - FF48B7
VALUE-1 - FFFFFFFF
VALUE-1 - FF8877
VALUE-1 - FF08F7
VALUE-1 - FFE817
VALUE-1 - FF6897
VALUE-1 - FFA857
VALUE-1 - FF28D7
*/
