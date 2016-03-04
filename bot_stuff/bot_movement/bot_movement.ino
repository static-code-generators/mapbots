#include "Direction.h"
#include "Constants.h"

const double distIncr = 25;
double currDist = distIncr;

void moveDistance(long double dist);

void setup() {
    pinMode(AL, OUTPUT);
    pinMode(BL, OUTPUT);
    pinMode(AR, OUTPUT);
    pinMode(BR, OUTPUT);
    pinMode(encL, INPUT);
    pinMode(encR, INPUT);

    Serial.begin(9600);
}

void loop() {
    moveDistance(currDist);
    currDist += distIncr;
    moveLeft();
    delay(900);
    stahp();
}

void moveDistance(long double dist) {
    moveForward();
    int reqPulses = dist / distPerPulse;
    int numPulses = 0;
    Serial.print("target: ");
    Serial.println(reqPulses);
    while (numPulses < reqPulses) {
        int initValue = digitalRead(encL);
        int currValue;

        Serial.print("HM: ");
        Serial.println(numPulses);

        while((currValue = digitalRead(encL)) == initValue) {
            delay(10);
        }
        while((currValue = digitalRead(encL)) != initValue) {
            delay(10);
        }
        ++numPulses;
    }
    stahp();
}
