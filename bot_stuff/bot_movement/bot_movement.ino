#include "Constants.h"
#include "Direction.h"

void setup() {
    pinMode(AL, OUTPUT);
    pinMode(BL, OUTPUT);
    pinMode(AR, OUTPUT);
    pinMode(BR, OUTPUT);

    pinMode(encL, INPUT);
    pinMode(encR, INPUT);

    Serial.begin(9600);
}

const int timeIncrement = 1000;
int timeInterval = timeIncrement;

void loop() {
    moveLeft();
    delay(1000);
    stahp();

    moveForward();
    delay(timeInterval);
    timeInterval += timeIncrement;
    stahp();
    delay(1000);
}
