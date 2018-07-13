/*
 * RGB Strip control script with AtTiny84 and MosFET Transistors
 * Bailey 2017
 * v0.3 - send back 1 for OK and 2 for Error
 * added watchdog with 8 seconds timer
 * 
 */
#include <TinyWireS.h>
#include <RGBControl.h>
//#include <avr/wdt.h>

#define I2C_SLAVE_ADDRESS 0x1

#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 16 )
#endif

#define VPIN 9 // voltpin

int FADESPEED = 10;
rgb led(5, 7, 8); // red, blue, green

//read serial buffer
//int serbuffcount;
//char serbuff[10];


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

void setup() {
  pinMode(VPIN, OUTPUT);
  
  // enable watchdog timer
  //wdt_enable(WDTO_8S);
  
  TinyWireS.begin(I2C_SLAVE_ADDRESS);

  // Sets up the onRequest function (what we do if asked to send something).
  TinyWireS.onRequest(requestEvent);

  // Sets up the onReceive function (what we do if we get stuff).
  TinyWireS.onReceive(receiveEvent);
}


void loop() {
  // Detects a stop sending command.
  TinyWireS_stop_check();

  // reset watchdog timer
  //wdt_reset();
}


void requestEvent(){  
  TinyWireS.send(txChrData);
  txIndex = 0;
}


void receiveEvent(uint8_t  b){
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
        //if (rxIndex >= reg_size){
        //    reg_position = 0;
        //}
    }


/*
  while(TinyWireS.available() > 1){
    char c = TinyWireS.receive();
    rxChrData[rxIndex++] = char(c);
  }
*/  
  procWireData(rxIndex);
  /*
  //clear array and buffer
  for (int x = 0; x <= serbuffcount; x++) {
    serbuff[x] = '\0';
  }
  serbuffcount = 0;
  */
}


void procWireData(int index) {
  char* cmd = strtok(rxChrData, ',');
  char* sep = strchr(rxChrData, ',');
  *sep = 0;
  sep++;

  if(strcmp(cmd, "off") == 0){
    statled(false);
    led.switch_rgb_strip(false);
  }
  else if(strcmp(cmd, "on") == 0){
    statled(true);
    led.switch_rgb_strip(true);
  }
  else if(strcmp(cmd, "white") == 0){
    statled(true);
    led.white(atoi(sep));
  }
  else if(strcmp(cmd, "fade") == 0){
    statled(true);
    led.fade_rgb_up_down(atoi(sep));
  }
  else if(strcmp(cmd, "g") == 0){
    statled(true);
    led.green(atoi(sep));
  }
  else if(strcmp(cmd, "b") == 0){
    statled(true);
    led.blue(atoi(sep));
  }
  else if(strcmp(cmd, "r") == 0){
    statled(true);
    led.red(atoi(sep));
  }
  else if(strcmp(cmd, "rl") == 0){
    statled(true);
    led.red_light(atoi(sep));
  }
  else if(strcmp(cmd, "gl") == 0){
    statled(true);
    led.green_light(atoi(sep));
  }
  
  //wdt_reset(); //reset watchdog timer
  // Puts the data we got into a variable to send back for error checking.
  txIndex = 0;
  for(int i=0;i<=rxIndex;i++){
    txChrData[txIndex++] = rxChrData[i];
  }
  rxIndex = 0;
}


