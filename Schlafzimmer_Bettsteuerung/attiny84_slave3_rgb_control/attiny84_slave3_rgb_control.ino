/*
 * RGB Strip control script with AtTiny84 and MosFET Transistors
 * Bailey 2017
 * v0.3 - send back 1 for OK and 2 for Error
 * added watchdog with 8 seconds timer
 * 
 * setup:
 * red   = 5
 * blue  = 7
 * green = 8
 * 
 * Powerpin = 9 // 12V RGB LED Powersource
 * 
 * PIRPin Left = 0
 * PIRPin Right = 1
 * Lightsensorpin = 2
 * 
 * Activity LED = 3
 * 
 */
#include <EEPROM.h>
#include <TinyWireS.h>
#include <RGBControl.h>
#include <BedLightControl.h>
#include <avr/wdt.h>

#define I2C_SLAVE_ADDRESS 0x3

#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 16 )
#endif

#define VPIN 9 // voltpin
#define ACTLED 3 //activity led pin

bool taskrun;
bedlightcontrol bed(0, 1, 2, 500, 30); // PIRPin1, PIRPin2, Brightnesspin, Brightnessvalue, lighttime
int defaultbright;

int FADESPEED = 10;
rgb led(5, 7, 8); // red, blue, green

// new version
//Character variable used to echo data back.
char txChrData[16];
int txIndex = 0;

//Variables used in getting and parsing data.
char rxChrData[16]; //Receives the data.
//char rxString[16];  //Varbiable for holding one string of data.
int rxIndex = 0; //Used to index rxString.

void statled(bool on=false){
  if(on){
    digitalWrite(VPIN, HIGH);
  } else {
    digitalWrite(VPIN, LOW);
  }
}

void act(bool activ=false){
  if(activ){
    digitalWrite(ACTLED, HIGH);
  } else {
    digitalWrite(ACTLED, LOW);
  }
}

void setup() {
  pinMode(VPIN, OUTPUT);
  pinMode(ACTLED, OUTPUT);
  act(true);
  taskrun = false;

  // load default data from eeprom
  defaultbright = int(EEPROM.read(0));
  if(defaultbright < 0){
    defaultbright = 255;
  }
  
  // enable watchdog timer
  wdt_enable(WDTO_8S);
  
  TinyWireS.begin(I2C_SLAVE_ADDRESS);

  // Sets up the onRequest function (what we do if asked to send something).
  TinyWireS.onRequest(requestEvent);

  // Sets up the onReceive function (what we do if we get stuff).
  TinyWireS.onReceive(receiveEvent);
  wdt_reset();
  act();
}

/*
void reboot_board(){
  // reboot board with watchdog
  wdt_enable(WDTO_1S); // set Watchdog timer to 1 second
  wdt_reset(); // reset watchdog
  for(int i=0;i<100;i++){
    delay(500);
  }
}
*/

unsigned long currmillis, task1;
void loop() {
  currmillis = millis();
  
  // Detects a stop sending command.
  TinyWireS_stop_check();

  // task for automatic switch bed light on
  if((unsigned long)(currmillis - task1) >= 1000){
    act(true);
    if(!(taskrun)){
      int bedstate = bed.switchON();
      if(bedstate == 1){
        led.white(defaultbright);
      } else if(bedstate == 0){
        led.white(0);
      }
    }
    task1 = millis();
    act();
  }
  
  // reset watchdog timer
  wdt_reset();
}


void clstxChar(){
  memset(txChrData, 0, sizeof(txChrData));
  txIndex = 0;
}


void clsrxChar(){
  memset(rxChrData, 0, sizeof(rxChrData));
  rxIndex = 0;
}


void requestEvent(){
  act(true);
  TinyWireS.send(txChrData);
  clstxChar();
  act();
}


void receiveEvent(uint8_t  b){
  wdt_reset();
  if (b < 1){
        // Sanity-check
        return;
    }
    if (b > TWI_RX_BUFFER_SIZE){
        // Also insane number
        return;
    }

    //char c = TinyWireS.receive();
    //b--;
    if (!b){
        // This write was only to set the buffer for next read
        return;
    }
    while(b--){
        rxChrData[rxIndex++] = TinyWireS.receive();
    }
  procWireData(rxIndex);
}


void procWireData(int index) {
  act(true);
  bool task = false;
  char* cmd = strtok(rxChrData, ',');
  char* sep = strchr(rxChrData, ',');
  *sep = 0;
  sep++;

  if(strcmp(cmd, "off") == 0){
    statled(false);
    led.switch_rgb_strip(false);
    task = true;
    taskrun = false;
  }
  else if(strcmp(cmd, "on") == 0){
    statled(true);
    led.switch_rgb_strip(true);
    task = true;
    taskrun = false;
  }
  else if(strcmp(cmd, "white") == 0){
    statled(true);
    led.white(atoi(sep));
    task = true;
  }
  else if(strcmp(cmd, "fade") == 0){
    statled(true);
    led.fade_rgb_up_down(atoi(sep));
    task = true;
    taskrun = true;
  }
  else if(strcmp(cmd, "g") == 0){
    statled(true);
    led.green(atoi(sep));
    task = true;
  }
  else if(strcmp(cmd, "b") == 0){
    statled(true);
    led.blue(atoi(sep));
    task = true;
  }
  else if(strcmp(cmd, "r") == 0){
    statled(true);
    led.red(atoi(sep));
    task = true;
  }
  else if(strcmp(cmd, "rl") == 0){
    statled(true);
    led.red_light(atoi(sep));
    task = true;
  }
  else if(strcmp(cmd, "gl") == 0){
    statled(true);
    led.green_light(atoi(sep));
    task = true;
  }
  else if(strcmp(cmd, "defbright") ==0){
    EEPROM.write(0, atoi(sep));
    //reboot_board();
    defaultbright = atoi(sep);
  }
  
  if(atoi(sep) > 0){
    taskrun = true;
  } else {
    taskrun = false;
  }
  
  //wdt_reset(); //reset watchdog timer
  // if task was startet, clear txChrData buffer to protect overspeech.
  if(task){
    clstxChar();
  } else {
    taskrun = false;
  }
  
  // Puts the data we got into a variable to send back for error checking.
  for(int i=0;i<=rxIndex;i++){
    txChrData[txIndex++] = rxChrData[i];
  }
  // clear receive buffer
  clsrxChar();
  wdt_reset();
  act();
}

