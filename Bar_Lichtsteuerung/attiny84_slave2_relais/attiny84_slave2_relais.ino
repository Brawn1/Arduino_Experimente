/*
 * AtTiny84 8-Port Relais controlled over i2C.
 * The controller has the i2C Address 0x2 and works as slave.
 * 
 * 
 * wiring (Arduino Port Number):
 * 6, 4 - I²C Communication
 * 
 * 8 Channel Relais Card with Optokoppler (Relais Switching 230V/AC 10A)
 * 
 * AtTiny84 port - 8 channel relais
 *  0 - IN1
 *  1 - IN2
 *  2 - IN3
 *  3 - IN4
 *  5 - IN5
 * 10 - IN6
 *  9 - IN7
 *  8 - IN8
 * 
 * 
 *  7 - Activity LED
 * version 0.8
*/
#include <TinyWireS.h>
#include <RelaisControl.h>
#include <avr/wdt.h>

#define I2C_SLAVE_ADDRESS 0x2

#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE (16)
#endif

// relaiscontrol
byte R1 = 0;
byte R2 = 1;
byte R3 = 2;
byte R4 = 3;
byte R5 = 5;
byte R6 = 10;
byte R7 = 9;
byte R8 = 8;
// init relais
relais relais(R1, R2, R3, R4, R5, R6, R7, R8, true);

// activity LED
byte ACTLED = 7;

// i2c communication
//Character variable used to echo data back.
char txChrData[16];
int txIndex = 0;

//Variables used in getting and parsing data.
char rxChrData[16]; //Receives the data.
//char rxString[16];  //Varbiable for holding one string of data.
int rxIndex = 0; //Used to index rxString.


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
  // enable watchdog timer and set to 8 seconds
  wdt_enable(WDTO_8S);
  // activity LED
  pinMode(ACTLED, OUTPUT);
  act(true);
  
  TinyWireS.begin(I2C_SLAVE_ADDRESS);
  // Sets up the onRequest function (what we do if asked to send something).
  TinyWireS.onRequest(requestEvent);

  // Sets up the onReceive function (what we do if we get stuff).
  TinyWireS.onReceive(receiveEvent);
  
  act();
  wdt_reset();
}


void loop() {
  TinyWireS_stop_check();
  wdt_reset();
}


void requestEvent(){  
  TinyWireS.send(txChrData);
  clstxChar();
}


void clstxChar(){
  memset(txChrData, 0, sizeof(txChrData));
  txIndex = 0;
}


void clsrxChar(){
  memset(rxChrData, 0, sizeof(rxChrData));
  rxIndex = 0;
}


void receiveEvent(uint8_t  b){
  if(b < 1){
    // Sanity-check
    return;
  }
  if(b > TWI_RX_BUFFER_SIZE){
    // Also insane number
    return;
  }

  if(!b){
    // This write was only to set the buffer for next read
    return;
  }
  
  while(b--){
    rxChrData[rxIndex++] = TinyWireS.receive();
  }
  
  procWireData(rxIndex);
}


void procWireData(int index){
  act(true);
  bool is_processed = false;
  clstxChar();
  char* topic = strtok(rxChrData, ',');
  char* value = strchr(rxChrData, ',');
  *value = 0;
  value++;

  // 8 port relais
  if(strcmp(topic, "relais1")==0){
    if(strcmp(value, "1")==0){
      relais.ON(R1);
    } else {
      relais.OFF(R1);
    }
    is_processed = true;
  }

  if(strcmp(topic, "relais2")==0){
    if(strcmp(value, "1")==0){
      relais.ON(R2);
    } else {
      relais.OFF(R2);
    }
    is_processed = true;
  }

  if(strcmp(topic, "relais3")==0){
    if(strcmp(value, "1")==0){
      relais.ON(R3);
    } else {
      relais.OFF(R3);
    }
    is_processed = true;
  }

  if(strcmp(topic, "relais4")==0){
    if(strcmp(value, "1")==0){
      relais.ON(R4);
    } else {
      relais.OFF(R4);
    }
    is_processed = true;
  }

  if(strcmp(topic, "relais5")==0){
    if(strcmp(value, "1")==0){
      relais.ON(R5);
    } else {
      relais.OFF(R5);
    }
    is_processed = true;
  }

  if(strcmp(topic, "relais6")==0){
    if(strcmp(value, "1")==0){
      relais.ON(R6);
    } else {
      relais.OFF(R6);
    }
    is_processed = true;
  }

  if(strcmp(topic, "relais7")==0){
    if(strcmp(value, "1")==0){
      relais.ON(R7);
    } else {
      relais.OFF(R7);
    }
    is_processed = true;
  }

  if(strcmp(topic, "relais8")==0){
    if(strcmp(value, "1")==0){
      relais.ON(R8);
    } else {
      relais.OFF(R8);
    }
    is_processed = true;
  }

  if(is_processed){
    txChrData[txIndex++] = "1";
  } else {
    txChrData[txIndex++] = "0";
  }
  
  clsrxChar();
  act();
  // check if relais active
  if(relais.ACTIVE()){
    act(true);
  } else {
    act();
  }
  wdt_reset();
}

