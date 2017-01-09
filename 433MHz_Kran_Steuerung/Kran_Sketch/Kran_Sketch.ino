/*
 * Kransteuerung mit einem ATtiny85 und einem 433MHz Modul Empfangsmodul.
 * 
 * Beschreibung:
 * Die Steuerung wird für einen kleinen Kran am Balkon verwendet und muss mit einem 433MHz Sender gesteuert werden.
 * Den Sender selber haben wir zum schalten von 3 Terrassenlampen und somit haben wir noch 2 Kanäle (C,D) für unseren Kran.
 * 
 * Da der Microcontroller keine hohe Lasten schalten kann, Steuere ich damit 2 Wechselrelais an.
 * Bei den Wechselrelais wird auf der 230VAC so geschalten, dass auch beim Stecken von einem Relais immer nur eines aktiv sein kann.
 * Ob das jetzt das Relais für Rauf oder Runter ist kommt von der Sicherheit darauf an. (bei mir ist es Rauf da sonst der Motor sonst
 * nicht abschaltet).
 * 
 * Tipp:
 * Wenn man mehr Geld Investieren möchte, wäre ein Sicherheitsrelais (Schütz) dafür besser vorgesehen. Da es aber Privat ist und der Kran
 * gerade mal ca. 10x im Jahr genutzt wird. ist das nicht so schlimm. (über Winter wird es vom Netz Abgesteckt).
 * 
 * Damit auch der Kran vor Ort Manuell bedient werden kann, sind auch noch 2 Taster (Rauf, Runter) am Kasten montiert.
 * 
 * Da es sehr viel mit 433Mhz Fernbedinung gibt, habe ich zuerst den Code von der FB Ausgelesen und es in die Konditionen im Loop
 * eingebaut.
 * Zum Empfangen hat sich das Projekt RCSwitch am besten bewährt.
 * 
 * Da die Relais eher Träge sind, wird zur Sicherheit zwischen dem Umschalten 150ms gewartet.
 * Es würde zwar durch die Verdrahtung nichts machen wenn beide Relais aktiv sind, aber man muss es ja nicht fordern.
 * 
 * 
 * ATtiny85:
 * ---------
 * VCC = 5V
 * GND = GND
 * DATA (433Mhz Empfang) = Pin2
 * Relais 1 = Pin3
 * Relais 2 = Pin4
 * SW_Up = Pin1
 * SW_Down = Pin0
 * 
 * Info:
 * Falls ein anderer Arduino Kontroller verwendet wird, muss man die Pins anpassen.
 * 
 * MIT License:
 * Copyright (c) 2016 Günter Bailey
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
 * documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * MIT Lizenz in Deutsch:
 * Copyright (c) 2016 Günter Bailey
 * 
 * Hiermit wird unentgeltlich jeder Person, die eine Kopie der Software und der zugehörigen Dokumentationen (die "Software") erhält, 
 * die Erlaubnis erteilt, sie uneingeschränkt zu nutzen, inklusive und ohne Ausnahme mit dem Recht, sie zu verwenden, 
 * zu kopieren, zu verändern, zusammenzufügen, zu veröffentlichen, zu verbreiten, zu unterlizenzieren und/oder zu verkaufen, 
 * und Personen, denen diese Software überlassen wird, diese Rechte zu verschaffen, unter den folgenden Bedingungen:
 * 
 * Der obige Urheberrechtsvermerk und dieser Erlaubnisvermerk sind in allen Kopien oder Teilkopien der Software beizulegen.
 * DIE SOFTWARE WIRD OHNE JEDE AUSDRÜCKLICHE ODER IMPLIZIERTE GARANTIE BEREITGESTELLT, 
 * EINSCHLIESSLICH DER GARANTIE ZUR BENUTZUNG FÜR DEN VORGESEHENEN ODER EINEM BESTIMMTEN ZWECK SOWIE JEGLICHER RECHTSVERLETZUNG, 
 * JEDOCH NICHT DARAUF BESCHRÄNKT. IN KEINEM FALL SIND DIE AUTOREN ODER COPYRIGHTINHABER FÜR JEGLICHEN SCHADEN ODER 
 * SONSTIGE ANSPRÜCHE HAFTBAR ZU MACHEN, OB INFOLGE DER ERFÜLLUNG EINES VERTRAGES, 
 * EINES DELIKTES ODER ANDERS IM ZUSAMMENHANG MIT DER SOFTWARE ODER SONSTIGER VERWENDUNG DER SOFTWARE ENTSTANDEN.
 * 
 */
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();
int Rec_Pin = 0; // 0 => Pin 2 (wird bei rc-switch so gezaehlt)
int SW_Up = 1; // Switch Up
int SW_Down = 0; // Switch Down
int S1 = 4; // Switch Relais 1
int S2 = 5; // Switch Relais 2
// da bei ATtiny85 bereits alle Pins gebraucht werden, muessen wir uns mit einem Boolean Feld zufrieden geben.
// die Boolean Felder sind fuer uns sozusagen der Ersatz von 2 zusaetzlichen Input Pins
boolean S1_State = false;
boolean S2_State = false;
char mode[4]; // ein Reservierter Speicher fuer HIGH, LOW Schaltwert
int waittime = 1000; // Wartezeit zwischen dem Druecken der Taster (1 Sekunde)

void setup() {
  //Serial.begin(115200);
  pinMode(SW_Down, INPUT);
  pinMode(SW_Up, INPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  // zur Sicherheit die Pins sofort auf LOW setzen und somit die Relais abschalten
  pinMode(S1, LOW);
  pinMode(S2, LOW);
  // Setze Status auf false
  S1_State = false;
  S2_State = false;
  mySwitch.enableReceive(Rec_Pin); // 0 => Pin 2
}

void SecRelais1(char mode){
  /*
   * Secure Schalt Logic fuer Relais 1
   */
  if (mode == HIGH) {
    if (S1_State == false && S2_State == false) {
      S1_State = true;
      digitalWrite(S1, HIGH);
    } else if (S1_State == false && S2_State == true) {
      digitalWrite(S2, LOW);
      S2_State = false;
      delay(150);
      S1_State = true;
      digitalWrite(S1, HIGH);
    } else if (S1_State == true && S2_State == false) {
      // do nothing
    }
  } else {
    digitalWrite(S1, LOW);
    S1_State = false;
  }
}

void SecRelais2(char mode){
  /*
   * Secure Schalt Logic fuer Relais 2
   */
  if (mode == HIGH) {
    if (S2_State == false && S1_State == false) {
      S2_State = true;
      digitalWrite(S2, HIGH);
    } else if (S2_State == false && S1_State == true) {
      digitalWrite(S1, LOW);
      S1_State = false;
      delay(150);
      S2_State = true;
      digitalWrite(S2, HIGH);
    } else if (S2_State == true && S1_State == false) {
      // do nothing
    }
  } else {
    digitalWrite(S2, LOW);
    S2_State = false;
  }
}


unsigned long millitask1 = 0;
unsigned long millitask2 = 0;
void loop() {
  unsigned long currmillis = millis();
  if ((unsigned long)(currmillis - millitask1) == waittime || currmillis == 1000) {
     // 
     while (digitalRead(SW_Down) == HIGH && digitalRead(SW_Up) == LOW) {
      SecRelais2(HIGH);
     }
     
     while (digitalRead(SW_Up) == HIGH && digitalRead(SW_Down) == LOW) {
      SecRelais1(HIGH);
     }

     while (digitalRead(SW_Up) == HIGH && digitalRead(SW_Down) == HIGH) {
      // if SW1 and SW2 High switch Secure Relais 1 and 2 OFF
      SecRelais1(LOW);
      SecRelais2(LOW);
     }

     SecRelais1(LOW);
     SecRelais2(LOW);
  }

  
  if (mySwitch.available()) {
    long unsigned int value = mySwitch.getReceivedValue();

    if (value == 5592337) {
      SecRelais1(HIGH);
      //Serial.println(F("Switch Relais 1 ON"));
    }
    else if (value == 5592340) {
      SecRelais1(LOW);
      SecRelais2(LOW);
      delay(150);
      //Serial.println(F("Switch Relais 1 OFF"));
    } 
    else if (value == 5592145) {
      SecRelais2(HIGH);
      //Serial.println(F("Switch Relais 2 ON"));
    } 
    else if (value == 5592148) {
      SecRelais2(LOW);
      SecRelais1(LOW);
      delay(150);
      //Serial.println(F("Switch Relais 2 OFF"));
    }  
    else {
      // do nothing
    }
    mySwitch.resetAvailable();
  }
}

