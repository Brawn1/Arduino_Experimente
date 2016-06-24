/*
  Dieses kleine Programm laesst die 2 LED's abwechselnd Blinken
 */

//globale variable
// lk1 = led1 pin kurz
// lm1 = led1 pin mittel
// ll1 = led1 pin lang
int statled=13;
int lk1=2;
int lm1=4;
int ll1=7;
// led2
int lk2=12;
int lm2=8;
int chtime=800;

//starte setup
void setup() 
{
  Serial.begin(9600);
  pinMode(statled,OUTPUT);
  pinMode(lk1,OUTPUT);
  pinMode(lm1,OUTPUT);
  pinMode(ll1,OUTPUT);
  pinMode(lm2,OUTPUT);
  pinMode(lk2,OUTPUT);
}

//das Programm startet ab da
void loop() 
{
  digitalWrite(statled,HIGH);
  Serial.println("test-1 Rot");
  digitalWrite(lk1,LOW);
  digitalWrite(ll1,LOW);
  digitalWrite(lm1,HIGH);
  delay(chtime);

  Serial.println("Test-3 Blau");
  digitalWrite(lk1,HIGH);
  digitalWrite(ll1,LOW);
  digitalWrite(lm1,LOW);
  delay(chtime);
  
  Serial.println("Test-4 Weiss");
  digitalWrite(lk1,HIGH);
  digitalWrite(ll1,LOW);
  digitalWrite(lm1,HIGH);
  delay(chtime);
  
  Serial.println("Test-5 LED2 ROT");
  led1setlow();
  digitalWrite(lk2,LOW);
  digitalWrite(ll1,LOW);
  digitalWrite(lm2,HIGH);
  delay(chtime);
  
  Serial.println("Test-6 LED2 Blau");
  led1setlow();
  digitalWrite(lk2,HIGH);
  digitalWrite(ll1,LOW);
  digitalWrite(lm2,LOW);
  delay(chtime);
  
  Serial.println("Test-7 LED2 Weiss");
  led1setlow();
  digitalWrite(lk2,HIGH);
  digitalWrite(ll1,LOW);
  digitalWrite(lm2,HIGH);
  delay(chtime);
  
  Serial.println("Test-Ende Reset");
  led1setlow();
  led2setlow();
  digitalWrite(statled,LOW);
  delay(chtime);
}

void blueled1() {
  digitalWrite(lk1,HIGH);
  digitalWrite(ll1,LOW);
  digitalWrite(lm1,LOW);
}

void redled1() {
  digitalWrite(lk1,LOW);
  digitalWrite(ll1,LOW);
  digitalWrite(lm1,HIGH);
}

void whiteled1() {
  digitalWrite(lk1,HIGH);
  digitalWrite(ll1,LOW);
  digitalWrite(lm1,HIGH);
}

// set led ports to LOW
void led1setlow() {
  digitalWrite(lk1,LOW);
  digitalWrite(ll1,LOW);
  digitalWrite(lm1,LOW);
}

void led2setlow() {
  digitalWrite(lk2,LOW);
  //digitalWrite(ll2,LOW);
  digitalWrite(lm2,LOW);
}
  
