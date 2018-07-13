/*
  RGBControl.h - Library RGB LED Functions.
  Created by Günter Bailey, December 26, 2017.
  Released into the MIT License.
*/
#ifndef RGBControl_h
#define RGBControl_h

#include "Arduino.h"

class rgb
{
  public:
    rgb(int red, int blue, int green);
    void green(int intense);
    void blue(int intense);
    void red(int intense);
    void fade_blue(bool up=true, int fadespeed=5);
    void fade_red(bool up=true, int fadespeed=5);
    void fade_green(bool up=true, int fadespeed=5);
    void switch_rgb_strip(bool on=false);
    void fade_rgb_up_down(int fadespeed=5);
    void white(int intense);
    void red_light(int intense);
    void green_light(int intense);
    void setcolor(int red, int green, int blue);
  private:
    int REDPIN;
    int BLUEPIN;
    int GREENPIN;
};

#endif
