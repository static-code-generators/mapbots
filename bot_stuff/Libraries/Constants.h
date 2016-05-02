// Constants.h
#ifndef MOVEMENT_CONSTANTS_H
#define MOVEMENT_CONSTANTS_H
/*
Some params: 
For motors:
At 5V, 
Stall current = 0.522A
RPM = 78
For wheels:
Diameter = 50mm
*/

const int AL = 2;
const int BL = 3;
const int AR = 4;
const int BR = 5;
const int encL = 6;
const int encR = 7;

//For wheel and encoder dimensions
const double circ = 157.07963267948965; //circumferance of wheel in mm
const int numSlits = 30; //number of slits in the wheel encoder
const double distPerPulse = circ / numSlits; //what is the meaning of life? we do not know
const int numSens = 8; // number of sensors used

/* 
Shift register pin connections
Pin 1-7 and Pin 15 - outputs, HC-SR04 trig pins
Pin 11 - clockPin - 12
Pin 12 - latchPin - 11 
Pin 14 - dataPin - 10 
Pin 8 and 13 - GND
Pin 10 and 16 - 5V
Echo pins are connected to Pin 8, each through a transistor
*/
const int clockPin = 12;
const int latchPin = 11;
const int dataPin = 10;
const int echoPin = 8;
const int servoPin = 9;

#endif
