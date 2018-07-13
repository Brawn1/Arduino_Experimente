/*
  RGBControl.cpp - Library RGB LED Functions.
  Created by Günter Bailey, December 26, 2017.
  Released into the MIT License.
*/
#include "RGBControl.h"
#include "Arduino.h"
#include "avr/wdt.h"


rgb::rgb(int red, int blue, int green){
    REDPIN = red;
    BLUEPIN = blue;
    GREENPIN = green;
    pinMode(REDPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);
}

void rgb::fade_green(bool up=true, int fadespeed){
  if(up){
    for(int i=0;i < 256; i++){
      rgb::green(i);
      wdt_reset();
      delay(fadespeed);
    }
  } else {
    for(int i=255;i > 0; i--){
      rgb::green(i);
      wdt_reset();
      delay(fadespeed);
    }
  }
}

void rgb::fade_red(bool up=true, int fadespeed){
  if(up){
    for(int i=0;i < 256; i++){
      rgb::red(i);
      wdt_reset();
      delay(fadespeed);
    }
  } else {
    for(int i=255;i > 0; i--){
      rgb::red(i);
      wdt_reset();
      delay(fadespeed);
    }
  }
}

void rgb::fade_blue(bool up=true, int fadespeed){
  if(up){
    for(int i=0;i < 256; i++){
      rgb::blue(i);
      wdt_reset();
      delay(fadespeed);
    }
  } else {
    for(int i=255;i > 0; i--){
      rgb::blue(i);
      wdt_reset();
      delay(fadespeed);
    }
  }
}

void rgb::red(int intense){
   analogWrite(REDPIN, intense);
}

void rgb::green(int intense){
   analogWrite(GREENPIN, intense);
}

void rgb::blue(int intense){
   analogWrite(BLUEPIN, intense);
}

void rgb::switch_rgb_strip(bool on=false){
  /*
   * switch RGB LED Strip off
   */
   if(on){
    analogWrite(GREENPIN, 0);
    analogWrite(REDPIN, 0);
    analogWrite(BLUEPIN, 0);
   } else {
    analogWrite(GREENPIN, 0);
    analogWrite(REDPIN, 0);
    analogWrite(BLUEPIN, 0);
   }
   wdt_reset();
}

void rgb::white(int intense){
    rgb::red(intense);
    rgb::green(intense);
    rgb::blue(intense);
}

void rgb::green_light(int intense){
    if(intense < 255){
        float r1 = (200.0 / 100.0);
        float g1 = (150.0 / 100.0);
        float r2 = r1 * intense;
        float g2 = g1 * intense;
        rgb::blue(0);
        rgb::red((int)r2);
        rgb::green((int)g2);
    } else {
        rgb::red(200);
        rgb::green(150);
        rgb::blue(0);
    }
    wdt_reset();
}

void rgb::red_light(int intense){
    if(intense < 255){
        float r1 = (255.0 / 100.0);
        float g1 = (66.0 / 100.0);
        float r2 = r1 * intense;
        float g2 = g1 * intense;
        rgb::blue(0);
        rgb::red((int)r2);
        rgb::green((int)g2);
    } else {
        rgb::red(255);
        rgb::green(66);
        rgb::blue(0);
    }
    wdt_reset();
}

void rgb::fade_rgb_up_down(int fadespeed){
    rgb::fade_red(true, fadespeed);
    wdt_reset();
    rgb::fade_blue(false, fadespeed);
    wdt_reset();
    rgb::fade_green(true, fadespeed);
    wdt_reset();
    rgb::fade_red(false, fadespeed);
    wdt_reset();
    rgb::fade_blue(true, fadespeed);
    wdt_reset();
    rgb::fade_green(false, fadespeed);
}

void rgb::setcolor(int red, int green, int blue){
    analogWrite(REDPIN, red);
    analogWrite(BLUEPIN, blue);
    analogWrite(GREENPIN, green);
    wdt_reset();
}
