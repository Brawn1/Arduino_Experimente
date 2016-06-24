/*
LED Steuerung mit IR Fernbedienung

function added:
  runLEDs() // running LEDS from Q0-Q7 and Return
  clsShiftRegister() // set Shift Register to zero
  updateShiftRegister(input_number) // set Shift Register with Number 0 - 8
  updownLEDs() // load LEDS up and down from Q0-Q7
  https://www.arduino.cc/en/Tutorial/ShiftOut
*/
#include <IRremote.h>

int RECV_PIN = 9;
IRrecv irrecv(RECV_PIN);
decode_results results;

#define latchPin A1
#define clockPin A0
#define dataPin A2
#define statled 13

byte delays = 500;
byte data = 0;
byte leds;

void setup()
{
  Serial.begin(9600);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(statled,OUTPUT);
  irrecv.enableIRIn();
  alloff();
}

void loop()
{
  if (irrecv.decode(&results)) {
    translateIR();
    irrecv.resume();
  }
}

void translateIR()
{
  digitalWrite(statled,HIGH);
  switch(results.value)
  {
    case 0xFF30CF:
      Serial.println("updownLEDs - Button1");
      updownLEDs();
      break;
      
    case 0xFF18E7:
      Serial.println("runLEDs - Button2");
      runLEDs();
      break;
      
    case 0xFF7A85:
      Serial.println("runupLEDs - Button3");
      runupLEDs();
      break;
      
    case 0xFF10EF:
      Serial.println("rundownLEDs - Button4");
      rundownLEDs();
      break;
      
    case 0xFF38C7:
      Serial.println("randomLEDs - Button5");
      randomLEDs();
      break;
    
    case 0xFF5AA5:
      Serial.println("allon - Button6");
      allon();
      break;
    
    case 0xFF9867:
      Serial.println("random programm - Button Random");
      singlerandom();
      break;
    
    case 0xFF22DD:
      Serial.println("Random Play 100 Times - Button Play");
      for (int i=0;i<=100;i++)
      {
       // if (irrecv.decode(&results)) {
         // switch(results.value) {
           //case 0xFFA25D:
           //   break;
            singlerandom();
          //irrecv.resume();
          //}
        //}
        //delay(1000);
      }
  }
  digitalWrite(statled,LOW);
}

// function select 1 programm per random
void singlerandom()
{
  int c;
  c = random(6);
  if (c == 0)
  { 
    allon();
  }
  else if (c == 1)
  {  
    randomLEDs();
  }
  else if (c == 2)
  {
    rundownLEDs();
  }
  else if (c == 3)
  {
    runupLEDs();
  }
  else if (c == 4)
  {
    updownLEDs();
  }
  else
  {
    runLEDs();
  }
  
}

// function switch all LEDs off
void alloff()
{
  for (int i=0;i<=8;i++)
  {
    shiftWrite(i,LOW);
  }
}

// function switch all LEDs for 1 Second on
void allon()
{
  for (int i=0;i<=8;i++)
  {
    shiftWrite(i,HIGH);
  }
  
  delay(1000);
  for (int i=0;i<=8;i++)
  {
    shiftWrite(i,LOW);
  }
}

// function random led lights between 0 - 7
void randomLEDs()
{
  for (int j=0;j<=8;j++)
  {
    int i;
    i = random(8);
    shiftWrite(i,HIGH);
    delay(delays);
    shiftWrite(i,LOW);
  }
}

// function run down Q7-Q0
void rundownLEDs()
{
  for (int i=7;i>=0;i--)
  {
    shiftWrite(i,HIGH);
    delay(delays);
    shiftWrite(i,LOW);
  }
}

//function run up Q0-Q7
void runupLEDs()
{
  for (int i=0;i<=7;i++)
  {
    shiftWrite(i,HIGH);
    delay(delays);
    shiftWrite(i,LOW);
  }
}

//function load up down leds
void updownLEDs()
{
  for (int i=0;i<=7;i++)
  {
    shiftWrite(i, HIGH);
    delay(delays);
  }
  for (int j=7;j>=0;j--)
  {
    shiftWrite(j,LOW);
    delay(delays);
  }
}  

// function Running LED from Q0-Q7 and Q7-Q0
void runLEDs()
{
  for (int i=0;i<=7;i++)
  {
    shiftWrite(i,HIGH);
    delay(delays);
    shiftWrite(i,LOW);
  }
  for (int j=7;j>=0;j--)
  {
    shiftWrite(j,HIGH);
    delay(delays);
    shiftWrite(j,LOW);
  }
}

void shiftWrite(int desiredPin, boolean desiredState)
{
  bitWrite(data,desiredPin,desiredState);
  shiftOut(dataPin,clockPin,MSBFIRST,data);
  digitalWrite(latchPin,HIGH);
  digitalWrite(latchPin,LOW);
}

