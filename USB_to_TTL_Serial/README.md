# Mit einem Arduino eine USB-to_TTL Serielle Verbindung herstellen

Bei diesem Experiment machen wir aus einem Arduino eine USB zu Seriell Verbindungs Adapter.

## Verdrahtung

Der Arduino Uno hat die Ports 0 und 1 als RX, TX (es müssten alle gehen die einen FTDI Chip haben, auch hat es bei einem CH490G Chip auch schon funktioniert).
Somit kann man über diese Ports ein anderes Serielles Gerät damit Verbinden.
GND wird mit dem Arduino Verbunden und Fertig.

## Sketch (Code)

Wir müssen jetzt noch Arduino ein einfachen Sketch hochladen, wo wir auch gleich
verhindern das er die Pins auf High setzt.

* Es kann sein das man die Serielle Verbindung zuerst Abstecken muss bevor der Sketch hochgeladen wird.

```Arduino
void setup() {
  pinMode(0,INPUT);
  pinMode(1,INPUT);
}
void loop() {

}
```

So einfach ist es, und nicht mehr.

Jetzt lässt Arduino einfach alle Befehle und Kommandos direkt durch.
