/*
  Dieses kleine Programm laesst die 2 LED's abwechselnd Blinken
 */

//globale variable
int taster=13;
int tstatus=0;

//starte setup
// setze PIN 7,8 als Ausgang
void setup() 
{
  //Serial.begin(9600);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(taster,INPUT);
}

//das Programm startet ab da
void loop() 
{
  tstatus=digitalRead(taster);
  //Serial.println(tstatus);
  if (tstatus == LOW)
  {
    for (int rp=1; rp<=5; rp=rp+1)
    {
      digitalWrite(4,LOW);
      digitalWrite(8,HIGH);
      delay(500);
      digitalWrite(8,LOW);
      digitalWrite(7,HIGH);
      delay(500);
      digitalWrite(7,LOW);
      digitalWrite(12,HIGH);
      delay(500);
      digitalWrite(12,LOW);
    }
  }
  else 
  {
    digitalWrite(4,HIGH);
    delay(50);
  }
}

