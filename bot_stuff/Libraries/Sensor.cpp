#include "Constants.h"
#include "Sensor.h"

void scanAll(float distCm[])
{
    int i;
    for(i = 0; i < numSens; i++)
    {
        distCm[i] = singleSensorReading(i);
        delay(100);
    }
}

float singleSensorReading(int sensor)
{
    for (int i = 0; i < 3; ++i) {
        digitalWrite(s[i], (sensor >> i) & 1);
        //Serial.print((sensor >> i) & 1);
        //delay(10);
    }
    
    //Serial.print(", ");
    delay(10);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    return pulseIn(echoPin, HIGH, 25000) / 2.0 / 29.1;
}
