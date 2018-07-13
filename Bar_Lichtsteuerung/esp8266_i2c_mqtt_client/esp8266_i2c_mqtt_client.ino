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
 
 * verdrahtung:
 * A4, A5 - I²C Kommunikation auf die Bar RGB Lichtschlange
 * 
 * D2, D3 - Kommunikation ESP8266
 * 
 * 8 channel Relais with Optokoppler and 5V (Switching 230V/AC 10A)
 * D4 - IN1 - Stehlampe
 * D5 - IN2 - 
 * D6 - IN3 - 
 * D7 - IN4 - 
 * D8 - IN5 -
 * D9 - IN6 - 
 * D10 - IN7 -
 * D11 - IN8 -
 * 
 * D13 - Information PIN
 * 
 * Flash Option:
 * =============
 * Flash Mode = DIO
 * Flash Frequency = 40 Mhz ( Can be overclocked on some modules  :D )
 * CPU Frequency = 80 Mhz  ( See note above )
 * Flash Size = 1M (512k SPIFFS) <<< The important one
 * Debug port = disabled
 * Debug level = none
 * Reset Method = ck
 * Upload speed = 115200
 * 
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>

// esp8266 control
const char* WIFI_SSID = "AT6156WLAN1";
const char* WIFI_PASSWORD =  "61atwlan0156";
int reqCount = 0;

// mqtt setup
char mqttserver[] = "192.168.2.228";
char mqtt_user[] = "mosquitto";
char mqtt_password[] = "platinum-2018";
char mqtt_clientid[] = "bsn-arduinowz";

unsigned long lastsend = 0;

// Initialize the Ethernet client object
WiFiClient espClient;
PubSubClient client;

// i2c communication
byte rx = 0;
byte tx = 2;
int txsize = 16;
char txchar[16];
int txIndex = 0;

// activity led
byte ACTLED = 1;

void act(bool on=false){
  /*
   * Activity LED
   */
  if(on){
    digitalWrite(ACTLED, HIGH);
  } else {
    digitalWrite(ACTLED, LOW);
  }
}

void setup() {
  // Serial.begin(115200);
  // activity LED
  pinMode(ACTLED, OUTPUT);
  act(true);
  setup_wifi();
  //setup mqtt client
  client.setClient(espClient);
  client.setServer(mqttserver, 1883);
  client.setCallback(callback);
  lastsend = 0;
  // start i2c
  Wire.begin(rx,tx);
  delay(1);
  act();
}


unsigned long currmillis, task1, task2, task3, task4;
void loop() {
  currmillis = millis();
  
  // check every 2 seconds the connection to mqtt server
  if((unsigned long)(currmillis - task2) >= 2000){
    act(true);
    if(!client.connected()){
      mqtt_reconnect();
    }
    task2 = millis();
    act();
  }

  // look on mqtt board, if task is published
  if((unsigned long)(currmillis - task3) >= 1000){
    act(true);
    client.loop();
    task3 = millis();
    act();
  }

  // send heardbeat to mqtt service
  if((unsigned long)(currmillis - task4) >= 10000){
    act(true);
    client.publish("/bsn/wz/bar/heardbeat", String(random(8)).c_str());
    task4 = millis();
    act();
  }
}


void clstxChar(){
  memset(txchar, 0, sizeof(txchar));
  txIndex = 0;
}

// mqtt part
void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonBuffer<100> jsonBuffer;
  char msgbuffer[100] = "";
  String msg;
  
  // convert payload to char
  for (int i = 0; i < length; i++) {
    msgbuffer[i] = (char)payload[i];
  }

  // communicate over i2c to external modul, to control RGB light
  if((strcmp(topic, "/bsn/wz/bar/rgb/control")==0) || (strcmp(topic, "/bsn/wz/bar/rgb/white")==0)){
    JsonObject& json = jsonBuffer.parseObject(msgbuffer);
    
    if(!json.success()){
      return;
    }
    
    if(strcmp(topic, "/bsn/wz/bar/rgb/white")==0){
      String level = json["level"];
      msg = "white,";
      msg += level;
      txIndex = int(msg.length()+1);
      msg.toCharArray(txchar, txIndex);
      i2cSendData(1);
      clstxChar();
    }
    
    if(strcmp(topic, "/bsn/wz/bar/rgb/control")==0){
      String r = json["r"];
      String g = json["g"];
      String b = json["b"];
      msg;
      for(int i=0; i < 3;i++){
        if(i==0){
          msg = "r,"+r;
        }else if(i==1){
          msg = "g,"+g;
        } else if(i==2){
          msg = "b,"+b;
        }
        txIndex = int(msg.length()+1);
        msg.toCharArray(txchar, txIndex);
        i2cSendData(1);
        clstxChar();
      }
    }
    clstxChar();
    return;
  }
  
  // switch RGB ON
  else if(strcmp(topic, "/bsn/wz/bar/rgb/on")==0){
    if(strcmp(msgbuffer, "true")==0){
      msg = "on,0";  
    } else {
      msg = "off,0"; 
    }
    txIndex = int(msg.length()+1);
    msg.toCharArray(txchar, txIndex);
    i2cSendData(1);
    clstxChar();
    return;
  }

  // 8 port relais
  else if(strcmp(topic, "/bsn/wz/bar/relais1")==0){
    if(strcmp(msgbuffer, "true")==0){
      msg = "relais1,1";
    } else {
      msg = "relais1,0";
    }
  }
  
  else if(strcmp(topic, "/bsn/wz/bar/relais2")==0){
    if(strcmp(msgbuffer, "true")==0){
      msg = "relais2,1";
    } else {
      msg = "relais2,0";
    }
  }

  else if(strcmp(topic, "/bsn/wz/bar/relais3")==0){
    if(strcmp(msgbuffer, "true")==0){
      msg = "relais3,1";
    } else {
      msg = "relais3,0";
    }
  }

  else if(strcmp(topic, "/bsn/wz/bar/relais4")==0){
    if(strcmp(msgbuffer, "true")==0){
      msg = "relais4,1";
    } else {
      msg = "relais4,0";
    }
  }

  else if(strcmp(topic, "/bsn/wz/bar/relais5")==0){
    if(strcmp(msgbuffer, "true")==0){
      msg = "relais5,1";
    } else {
      msg = "relais5,0";
    }
  }

  else if(strcmp(topic, "/bsn/wz/bar/relais6")==0){
    if(strcmp(msgbuffer, "true")==0){
      msg = "relais6,1";
    } else {
      msg = "relais6,0";
    }
  }

  else if(strcmp(topic, "/bsn/wz/bar/relais7")==0){
    if(strcmp(msgbuffer, "true")==0){
      msg = "relais7,1";
    } else {
      msg = "relais7,0";
    }
  }

  else if(strcmp(topic, "/bsn/wz/bar/relais8")==0){
    if(strcmp(msgbuffer, "true")==0){
      msg = "relais8,1";
    } else {
      msg = "relais8,0";
    }
  }
  txIndex = int(msg.length()+1);
  msg.toCharArray(txchar, txIndex);
  i2cSendData(2);
  clstxChar();
}


void i2cSendData(byte address){
  if(address == 1){
    Wire.beginTransmission(0x1);
  } else {
    Wire.beginTransmission(0x2);
  }
  Wire.write(txchar, txIndex);
  Wire.endTransmission();
  delay(1);
  
}


void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect (clientId, username, password)
    if ( client.connect(mqtt_clientid, mqtt_user, mqtt_password) ) {
      // json values
      client.subscribe("/bsn/wz/bar/rgb/control");
      client.subscribe("/bsn/wz/bar/rgb/white");
      // bool values
      client.subscribe("/bsn/wz/bar/rgb/on");
      client.subscribe("/bsn/wz/bar/relais1");
      client.subscribe("/bsn/wz/bar/relais2");
      client.subscribe("/bsn/wz/bar/relais3");
      client.subscribe("/bsn/wz/bar/relais4");
      client.subscribe("/bsn/wz/bar/relais5");
      client.subscribe("/bsn/wz/bar/relais6");
      client.subscribe("/bsn/wz/bar/relais7");
      client.subscribe("/bsn/wz/bar/relais8");
    } else {
      delay(5000);
    }
  }
}


void setup_wifi(){
  // setup WiFi mode
  WiFi.mode(WIFI_STA);
  // attempt to connect to WIFI network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while( WiFi.status() != WL_CONNECTED){
    delay(500);
  }
}

