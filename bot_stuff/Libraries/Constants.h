// Constants.h
#ifndef MOVEMENT_CONSTANTS_H
#define MOVEMENT_CONSTANTS_H
// Some params: 
// For motors:
// At 5V, 
// Stall current = 0.522A
// RPM = 78
// For wheels:
// Diameter = 50mm

const int AL = 2;
const int BL = 3;
const int AR = 4;
const int BR = 5;
const int encL = 6;
const int encR = 7;

//For wheel and encoder dimensions
const double circ = 157.07963267948965;
const int numSlits = 30;
const double distPerPulse = circ / numSlits;

const int numSens = 8; // number of sensors used
const int clockPin = 6;
const int latchPin = 5;
const int dataPin = 4;
const int echoPin = 3;

#endif
