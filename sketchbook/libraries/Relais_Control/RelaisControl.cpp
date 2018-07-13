/*
  RelaisControl.h - Library to Control Relais Port.
  Created by Günter Bailey, May 13, 2018.
  Released into the MIT License.
*/
#include "RelaisControl.h"
#include "Arduino.h"


relais::relais(byte P1=-1, byte P2=-1, byte P3=-1, byte P4=-1, byte P5=-1, byte P6=-1, byte P7=-1, byte P8=-1, bool invert=false){
	/*
	 * init ports for relais
	*/
	INV = invert;
	byte vararray[8] = {R1, R2, R3, R4, R5, R6, R7, R8};
	byte portarray[8] = {P1, P2, P3, P4, P5, P6, P7, P8};
	for(byte i=0;i<8;i++){
		//asign ports P(x) to variable R(x)
		vararray[i] = portarray[i];
		//if port is used greater or equal zero, set OUTPUT
		if(vararray[i] >= 0){
			pinMode(vararray[i], OUTPUT);
			if(INV){
				digitalWrite(vararray[i], HIGH);
			} else {
				digitalWrite(vararray[i], LOW);
			}
		}
	}
}

bool relais::ON(byte port){
	/*
	 * switch relais on
	 */
	if(INV){
		if(digitalRead(port)){
			digitalWrite(port, LOW);
			return (!digitalRead(port));
		}
	} else {
		if(!(digitalRead(port))){
			digitalWrite(port, HIGH);
			return digitalRead(port);
		}
	}
}

bool relais::OFF(byte port){
	/*
	 * switch relais off
	 */
	if(INV){
		if(!(digitalRead(port))){
			digitalWrite(port, HIGH);
			return (!digitalRead(port));
		}
	} else {
		if(digitalRead(port)){
			digitalWrite(port, LOW);
			return digitalRead(port);
		}
	}
}

bool relais::ACTIVE(){
	/*
	 * get boolean state if relaiscard is active (switched on or when
	 * INV switched off)
	 */
	bool chk = false;
	byte vararray[8] = {R1, R2, R3, R4, R5, R6, R7, R8};
	for(byte i=0;i<8;i++){
		if(INV){
			if(!digitalRead(vararray[i])){
				chk = true; 
			};
		} else {
			if(digitalRead(vararray[i])){
				chk = true;
			}
		}
	}
	return chk;
}
