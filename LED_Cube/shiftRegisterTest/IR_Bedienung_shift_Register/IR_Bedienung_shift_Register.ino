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

int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);
decode_results results;

#define latchPin A1
#define clockPin A0
#define dataPin A2
#define statled 13

int redPin = 11;
int bluePin = 9;
int greenPin = 10;
int FADESPEED = 5;
// uncomment if your RGB LEDs works with Common Anode
//#define COMMON_ANODE

byte delays = 500;
byte data = 0;
byte leds;

void setup()
{
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(statled,OUTPUT);
  irrecv.enableIRIn();
  alloff();
  setColor(0,0,0);
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
            singlerandom();
      }
      break;
    
    case 0xFF42BD:
      Serial.println("blink 6 times - Button7");
      blinkall();
      break;
      
    case 0xFF4AB5:
      Serial.println("fade RGB - Button8");
      fadeRGB();
      setColor(0,0,0);
      break;
      
  }
  digitalWrite(statled,LOW);
}


// smooth fade RGB LED
void fadeRGB()
{
  int r, g, b;
 
  // fade from blue to violet
  for (r = 0; r < 256; r++) { 
    analogWrite(redPin, r);
    delay(FADESPEED);
  } 
  // fade from violet to red
  for (b = 255; b > 0; b--) { 
    analogWrite(bluePin, b);
    delay(FADESPEED);
  } 
  // fade from red to yellow
  for (g = 0; g < 256; g++) { 
    analogWrite(greenPin, g);
    delay(FADESPEED);
  } 
  // fade from yellow to green
  for (r = 255; r > 0; r--) { 
    analogWrite(redPin, r);
    delay(FADESPEED);
  } 
  // fade from green to teal
  for (b = 0; b < 256; b++) { 
    analogWrite(bluePin, b);
    delay(FADESPEED);
  } 
  // fade from teal to blue
  for (g = 255; g > 0; g--) { 
    analogWrite(greenPin, g);
    delay(FADESPEED);
  }
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
  else if (c == 5)
  {
    blinkall();
  }
  else if (c == 6)
  {
    fadeRGB();
  }
  else
  {
    runLEDs();
  }
  
}

// function blink all LEDs 6 times
void blinkall()
{
  for (int i=0;i<=5;i++)
  {
    allon();
    delay(1000);
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

void setColor(int red, int green, int blue)
{
  /*
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  */
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

