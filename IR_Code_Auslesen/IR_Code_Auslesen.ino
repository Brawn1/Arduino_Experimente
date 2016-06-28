/*
IR Code Auslesen
*/
#include <IRremote.h>

int RECV_PIN = 9;
int STATLED = 13;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  pinMode(STATLED, OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
  digitalWrite(STATLED, LOW);
}

void loop() {
  if (irrecv.decode(&results)) {
    digitalWrite(STATLED, HIGH);
    Serial.print("Hex - ");
    Serial.print(results.value, HEX); // print results as HEX
    Serial.print(" - Dec - ");
    Serial.print(results.value); // print results as decimal
    Serial.println("");
    irrecv.resume(); // Receive the next value
    digitalWrite(STATLED, LOW);
  }
}

