/* 
Emulating 24 sensors using 4 sensors by rotating
them about a servo motor.
*/

#include<Servo.h>

#include "Constants.h"
#include "Sensor.h"

Servo myservo;

float distMatrix[24/numSens][numSens];

void setup()
{
    Serial.begin(9600);
    int i;
    for(i = 0; i < numSens; i++)
        pinMode(triggerArr[i], OUTPUT);
    pinMode(echoPin, INPUT);
    myservo.attach(servoPin);
}

void loop()
{
    takeMultipleReadings();
}

void takeMultipleReadings()
{
    int i;
    int servoAngle = 0;
    for(i = 0; i < 6; i++)
    {
        myservo.write(servoAngle);
        delay(500);
        takeReading(i);
        delay(500);
        servoAngle += 15;
    }
}

void takeReading(int row)
{
    int i;
    for(i = 0; i < numSens; i++)
        distMatrix[row][i] = 0.0;

    scan(distMatrix[row]); // scan(float distCm[])

    delay(1000);
}
