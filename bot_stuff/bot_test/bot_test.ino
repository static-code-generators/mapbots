#include "Direction.h"
#include "Constants.h"

void setup()
{
    pinMode(AL, OUTPUT);
    pinMode(BL, OUTPUT);
    pinMode(AR, OUTPUT);
    pinMode(BR, OUTPUT);
    pinMode(encL, INPUT);
    pinMode(encR, INPUT);

    Serial.begin(9600);
}

void loop()
{
}

void testWheelEnc()
{
    moveForward();
    delay(10);
    Serial.print("encL: ");
    Serial.println(digitalRead(encL));
    delay(100);
    Serial.print("encR: ");
    Serial.println(digitalRead(encR));
    delay(100);
}

void testMotor()
{
    delay(100);
    moveForward();
    delay(3000);
    moveBackward();
    delay(3000);
    moveLeft();
    delay(3000);
    moveRight();
    delay(3000);
}

