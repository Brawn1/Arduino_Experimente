/* 
 Mit diesem Programm wird die Bar Komplett gesteuert.
 Ziel ist es dies mit den Vorhandenen Schaltern aber auch
 mit einer Fernbedienung oder mit einem Smartphone zu Steuern.
 
 Bonus:
 Falls der Raum Leer ist, sollten alle Lichter abgeschaltet werden.
 
 Zu Steuernde Elemente:
 * 5m RGB LEDs hinter den Gläsern
 * Halogen Beleuchtung bei den Glaesern in der Bar
 * Halogen Beleuchtung Bar Innenbereich
 * Halogen Beleuchtung Bar Aussenbereich
 * Wohnzimmer Licht
 * Wohnzimmer Decken Flooter
*/

// RGB Control
int rP = 11;
int bP = 9;
int gP= 10;
int FADESPEED = 5;
// uncomment if your RGB LEDs works with Common Anode
//#define COMMON_ANODE


void setup() {
  Serial.begin(9600);
  pinMode(rP,OUTPUT);
  pinMode(bP,OUTPUT);
  pinMode(gP,OUTPUT);
}

void loop() {
  fadeRGB();
  delay(1000);
  // RED
  setColor(255,0,0);
  delay(2000);
  // Green
  setColor(0,255,0);
  delay(2000);
  // Blue
  setColor(0,0,255);
  delay(2000);
  setColor(0,0,0);
  delay(10000);
}

// smooth fade RGB LED
void fadeRGB()
{
  int r, g, b;
 
  // fade from blue to violet
  for (r = 0; r < 256; r++) { 
    analogWrite(rP, r);
    delay(FADESPEED);
  } 
  // fade from violet to red
  for (b = 255; b > 0; b--) { 
    analogWrite(bP, b);
    delay(FADESPEED);
  } 
  // fade from red to yellow
  for (g = 0; g < 256; g++) { 
    analogWrite(gP, g);
    delay(FADESPEED);
  } 
  // fade from yellow to green
  for (r = 255; r > 0; r--) { 
    analogWrite(rP, r);
    delay(FADESPEED);
  } 
  // fade from green to teal
  for (b = 0; b < 256; b++) { 
    analogWrite(bP, b);
    delay(FADESPEED);
  } 
  // fade from teal to blue
  for (g = 255; g > 0; g--) { 
    analogWrite(gP, g);
    delay(FADESPEED);
  }
}

void setColor(int red, int green, int blue)
{
  /*
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  */
  analogWrite(rP, red);
  analogWrite(gP, green);
  analogWrite(bP, blue);
}
