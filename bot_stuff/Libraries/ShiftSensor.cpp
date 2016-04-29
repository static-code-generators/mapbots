#include "Constants.h"
#include "Sensor.h"

// Code for using multiple sensors (upto 8)

void triggerShiftRegisterAt(int i, byte sensors);


void scan(float distCm[], byte sensors)
{
    sensors = 0;

    // resetting all triggers on 8 sensors to 0
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, sensors);
    digitalWrite(latchPin, HIGH);
    delay(100);

    int i;
    for(i = 0; i < numSens; i++)
    {
        triggerShiftRegisterAt(i, sensors); 
        // converts time taken to receive pulse (in microseconds) to centimeters
        distCm[i] = (pulseIn(echoPin, HIGH, 25000))/2.0/29.1;
        //Serial.print("distCm[");
        //Serial.print(i);
        //Serial.print("]: ");
        //Serial.println(distCm[i]);
        delay(100);
    }
}

void triggerShiftRegisterAt(int i, byte sensors)
{
    bitSet(sensors, i); // sets ith bit to 1 in byte 'sensors'
    Serial.println(sensors, BIN);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, sensors);
    digitalWrite(latchPin, HIGH);

    delayMicroseconds(20);

    bitClear(sensors, i); // clears ith bit to 0 in byte 'sensors'
    Serial.println(sensors, BIN);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, sensors);
    digitalWrite(latchPin, HIGH);
}
