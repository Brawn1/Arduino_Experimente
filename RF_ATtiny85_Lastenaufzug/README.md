# RF AtTiny85 gesteuerter Lastenaufzug

## Kurze Einleitung

Da wir gerne Grillen aber die Sachen vom 1.OG jedesmal hinunter und 
wieder rauftragen müssen, haben wir uns entschlossen 
eine Seilwinde die noch in der Garage lag unter dem Balkon im 2.OG zu 
montieren.

Die Montage war einfach, aber leider stellte sich heraus das die 
Seilwinde sehr Langsam ist, und man die nur mit einem 2m Kabel vom 1.OG 
bedienen konnte.

Also habe ich mir überlegt dies mit einem Funksystem zu lösen, aber 
damit nicht extra eine Fernbedinung entworfen werden muss, verwende ich 
einen vorhandenen 433Mhz Sender für Funksteckdosen.

Dabei sind noch die 2 Kanäle C & D bei dem Sender Frei.

Damit es auch nur auf die 4 Taster am Sender Funktioniert und nicht 
durch irgendein anderes Signal aktiviert wird, habe ich den Code mit 
dem Arduino UNO ausgelesen.


## Benötigte Teile

* 1x 433Mhz Empfänger (E-Bay um ca. 2,50 € (Sender & Empfänger)

* 1x AtTiny85 (für die Steuerung)

* 1x Transistor PNP

* 1x 2-Fach Relais (Bei E-Bay um ca. 5 €) *Achtung: ich habe aus 
versehen die Relais mit Optokopler gekauft, somit muss ich zum 
Abschalten der Relais die Ports auf HIGH setzen.*


## Verdrahtung

Die Verdrahtung ist dank dem AtTiny85 sehr einfach.

![Verdrahtung AtTiny85](Doku/Schaltplan.jpg)

* Die Pins vom AtTiny85 werden wie dargestellt verwendet:

*PB1* -> aktivierung Spannungsversorgung vom Relais über PNP Transistor

*PB2* -> *DATA* RF-Empfänger

*PB3* -> *IN1* Relais 1

*PB4* -> *IN2* Relais 2

*VIN* -> max. 5,5 VDC

*GND* -> GND


## AtTinyX5 Pins und Datenblatt

Eine kurze Darstellung der Pins vom AtTinyX5 Serie

![Darstellung der Pins](Doku/attiny85_pinout.jpg)

Das Offizielle Datenblatt von Atmel

[PDF Datasheet AtTiny25 - 85](Doku/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf)


## Quellen

* [RC-Switch](https://github.com/sui77/rc-switch)
* [Atmel AtTiny85](http://www.atmel.com/devices/attiny85.aspx)
