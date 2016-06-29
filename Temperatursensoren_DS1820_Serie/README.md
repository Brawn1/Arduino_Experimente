# Temperatur Sensoren mit ID Auslesen



## Schaltung



![Arduino Schaltplan](doku/schaltplan.png)

![Bild Schaltung von der Seite](doku/bild_schaltung_1.jpg)

![Bild Schaltung von Oben](doku/bild_schaltung_2.jpg)

![Bild Schaltung im Betrieb](doku/bild_schaltung_3.jpg)


* Schaltung in Aktion

[![Arduino UNO DS18B20 Video](https://i.ytimg.com/vi/lkTJRRQ38B0/default.jpg?v=5774239f&sqp=CJDI0LsF&rs=AOn4CLDbyizEpxnX8qdu7an__HynPlIdBg)](https://youtu.be/lkTJRRQ38B0)

* Test mit einem iframe

<iframe width="560" height="315" src="https://www.youtube.com/embed/lkTJRRQ38B0" frameborder="0" allowfullscreen></iframe>

## Sketch (Code)

Dieser Code ist gut Kommentiert und somit muss man nicht mehr viel dazu Schreiben.

Es kann auch mit mehreren DS18B20 / DS18S20 verwendet werden.


```Arduino
/*
  Temperatur Sensoren DS18B20 bzw. DS18S20 mit SensorID Auslesen.
  Der Wert wird bereits in Grad Celsius umgerechnet.
  
*/

// Bibliotheken laden
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin für DS18B20 definieren Arduino
#define DS18B20_PIN 4

// alle x Millisekunden messen
int mtime = 10000;
// LED anzeige auf folgendem Port (somit sieht man, wann
// die Messung erfolgt
int ledport = 13;

// OneWire Referenz setzen
OneWire oneWire(DS18B20_PIN);

// DS18B20/DS18S20 initialisieren
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
void setup() {
  // Serielle Ausgabe starten 
  Serial.begin(9600);
  // DS18B20/DS18S20 starten
  sensors.begin();
  pinMode(ledport, OUTPUT);
}

void loop() {
  // Starten der Messung LED einschalten
  digitalWrite(ledport, HIGH);
  // Temperatursensor auslesen
  sensors.requestTemperatures();
  for(byte i=0;i<sensors.getDeviceCount();i++){
    sensors.getAddress(tempDeviceAddress, i);
    // funktion Aufrufen und UUID inkl. Temperatur ueber die 
    // Serielle Schnittstelle ausgeben.
    serialout_temperature(getuuid(tempDeviceAddress),sensors.getTempCByIndex(i));  
  }
  // LED am Ende wieder abschalten
  digitalWrite(ledport,LOW);
  delay(mtime);
}

// Temperatur ausgeben
void serialout_temperature(byte devaddr,float temp) {
  Serial.print(devaddr);
  Serial.print(";");
  Serial.print(temp);
  Serial.print(";");
  Serial.print("\n");
}

// Sensoruuid Ausgeben
int getuuid(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
```
