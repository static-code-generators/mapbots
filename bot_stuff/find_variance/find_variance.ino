#include "Sensor.h"
#include "Direction.h"
#include "Constants.h"

const int numReadings = 100;

void setup()
{
    for(int i = 0; i < 3; i++)
        pinMode(s[i], OUTPUT);
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);

}

void loop()
{
    for(int i = 0; i < 8; i++)
    {
        int actualReadings = numReadings;
        float distArr[numReadings];
        float sumDist = 0;
        for(int j = 0; j < numReadings; j++)
        {
            float x = singleSensorReading(i);
            Serial.println(x);
            distArr[j] = x;
            sumDist += distArr[j];
            actualReadings -= (x == 0);
            delay(10);
        }
        float mean = sumDist / (actualReadings ? actualReadings : 1);
        Serial.print("mean: ");
        Serial.println(mean);
        float varSum = 0;
        for(int j = 0; j < numReadings; j++)
            if(distArr[j] != 0)
                varSum += pow((distArr[j] - mean), 2);
        float variance = varSum / (actualReadings ? actualReadings : 1);
//        Serial.println("NAMKJDNSAKJDHAKJSDHKASJH");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(variance);
        delay(1000);
    }

    delay(10000);
}
