#include "Constants.h"
#include "Sensor.h"

float singleSensorReading(int i);

void scan(float distCm[])
{
    int i;
    for(i = 0; i < numSens; i++)
    {
        distCm[i] = singleSensorReading(i);
        delay(500);
    }
}

float singleSensorReading(int i)
{
    digitalWrite(triggerArr[i], LOW);
    delay(100);
    digitalWrite(triggerArr[i], HIGH);
    delayMicroseconds(50);
    digitalWrite(triggerArr[i], LOW);
    float val = pulseIn(echoPin, HIGH, 25000)/29.1/2.0;
    
    return val;
}
