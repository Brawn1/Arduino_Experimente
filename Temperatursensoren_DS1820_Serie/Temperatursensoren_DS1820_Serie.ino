/*
  Dieses Projekt soll den Raspberry Pi bei einfachen Messaufgaben
  Abloesen.
  Somit ist es moeglich, die Sensoren guenstiger und Robuster zu entwickeln.
*/
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin für DS18B20 definieren Arduino D2
#define DS18B20_PIN 4

// alle x Sekunden messen
int mtime = 10000;
// LED anzeige auf folgendem Port
int ledport = 13;

// OneWire Referenz setzen
OneWire oneWire(DS18B20_PIN);

// DS18B20 initialisieren
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
void setup() {
  // Serielle Ausgabe starten 
  Serial.begin(9600);
  // DS18B20 starten
  sensors.begin();
  pinMode(ledport,OUTPUT);
}

void loop() {
  digitalWrite(ledport,HIGH);
  // Temperatursensor auslesen
  sensors.requestTemperatures(); 
  for(byte i=0;i<sensors.getDeviceCount();i++){
    sensors.getAddress(tempDeviceAddress, i);
    serialout_temperature(getuuid(tempDeviceAddress),sensors.getTempCByIndex(i));  
  }
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

int getuuid(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

