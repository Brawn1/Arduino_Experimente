/*
  RelaisControl.h - Library to Control Relais Port.
  Created by Günter Bailey, May 13, 2018.
  Released into the MIT License.
*/
#ifndef RelaisControl_h
#define RelaisControl_h

#include "Arduino.h"

class relais
{
  public:
    relais(byte P1, byte P2, byte P3, byte P4, byte P5, byte P6, byte P7, byte P8, bool invert);
    bool ON(byte port);
    bool OFF(byte port);
    bool ACTIVE();
  private:
    byte R1;
    byte R2;
    byte R3;
    byte R4;
    byte R5;
    byte R6;
    byte R7;
    byte R8;
    bool INV=false;
};

#endif
