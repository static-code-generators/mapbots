/* 
Emulating 24 sensors using 8 sensors by rotating
them about a Servo.
*/

#include<Servo.h>

#include "Constants.h"
#include "Sensor.h"

Servo myservo;

int pos = 0;

float distMatrix[3][8];
byte sensorMatrix[3];

void setup()
{
    Serial.begin(9600);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(echoPin, INPUT);
    myservo.attach(9);
}

void loop()
{
    myservo.write(0);
    delay(500);
    takeReading(1);
    delay(500);
    myservo.write(15);
    delay(500);
    takeReading(2);
    delay(500);
    myservo.write(135);
    delay(500);
    takeReading(0);
    delay(500);
}

void takeReading(int row)
{
    int i;
    for(i = 0; i < numSens; i++)
        distMatrix[row][i] = 0.0;

    sensorMatrix[row] = 0;
    scan(distMatrix[row], sensorMatrix[row]); // scan(float distCm[], byte sensors)

    for(i = 0; i < numSens; i++)
    {
        Serial.print(row);
        Serial.print(":\n");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(distMatrix[row][i]);
        Serial.print("cm  ");
    }
    Serial.print("\n");
    delay(1000);
}
