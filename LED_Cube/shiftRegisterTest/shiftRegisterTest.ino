/*
Arduino Lesson 4. LEDs and Shift Register

function added:
  runLEDs() // running LEDS from Q0-Q7 and Return
  clsShiftRegister() // set Shift Register to zero
  updateShiftRegister(input_number) // set Shift Register with Number 0 - 8
  updownLEDs() // load LEDS up and down from Q0-Q7
  https://www.arduino.cc/en/Tutorial/ShiftOut
*/

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
}

void loop()
{
  digitalWrite(statled,HIGH);
  Serial.println("start with updownLEDs");
  updownLEDs();
  digitalWrite(statled,LOW);
  delay(800);
  digitalWrite(statled,HIGH);
  Serial.println("start with runLEDs");
  runLEDs();
  digitalWrite(statled,LOW);
  delay(800);
  digitalWrite(statled,HIGH);
  Serial.println("start with runupLEDs");
  runupLEDs();
  digitalWrite(statled,LOW);
  delay(800);
  digitalWrite(statled,HIGH);
  Serial.println("start with runupLEDs");
  rundownLEDs();
  digitalWrite(statled,LOW);
  delay(800);
  digitalWrite(statled,HIGH);
  Serial.println("start with randomLEDs");
  randomLEDs();
  digitalWrite(statled,LOW);
  delay(800);
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
  for (int i;i<=7;i++)
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
  
  
// Update Shift Register with value leds
void updateShiftRegister(byte lednum)
{
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin,clockPin,MSBFIRST,lednum);
  digitalWrite(latchPin,HIGH);
}

// clear Shift Register and set leds to 0
void clsShiftRegister(byte lednum)
{
  leds = lednum;
  updateShiftRegister(leds);
}
