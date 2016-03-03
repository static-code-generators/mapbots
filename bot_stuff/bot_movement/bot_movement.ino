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

void loop() {
    moveForward();
    delay(100);
    Serial.print(digitalRead(encL), BIN);
    Serial.print("\n");
    delay(100);
}
