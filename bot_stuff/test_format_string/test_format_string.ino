#include "Sensor.h"
#include "Direction.h"
#include "Constants.h"

enum direction
{
    NORTH = 0,
    WEST = 1,
    SOUTH = 2,
    EAST = 3
};

int rightAngleDelay = 800;

int xPos = 0;
int yPos = 0;
direction currDir = NORTH;

float distMatrix[3][8];

void setup()
{
    pinMode(AL, OUTPUT);
    pinMode(BL, OUTPUT);
    pinMode(AR, OUTPUT);
    pinMode(BR, OUTPUT);
    pinMode(encL, INPUT);
    pinMode(encR, INPUT);
    for(int i = 0; i < 3; i++)
        pinMode(s[i], OUTPUT);
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    myservo.attach(servoPin);

    Serial.begin(9600);
}

void loop()
{
    takeMultipleReadings();
    sendReadings();
    turnLeft();
    currDir = (currDir + 1) % 4;

}

void takeMultipleReadings()
{
    int i;
    int servoAngle = 0;
    for(i = 0; i < 3; i++)
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

void sendReadings()
{
    // Units for data:
    // x-y coords: in cm
    // sensor theta: in degrees

    // Printing x-y coords
    Serial.print("x: ");
    Serial.print(xPos);
    Serial.print(" ");
    Serial.print("y: ");
    Serial.println(yPos);

    // Now printing sensor theta along with sensor data
    // all angles in radians now
    // be careful, the code above talks in degrees at times
    int i, j;
    float sensorTheta = 0;
    float shiftOfDir[4] = {0.0, M_PI / 2, M_PI, (3 * M_PI) / 2};
    float shiftTheta = shiftOfDir[currDir];

    for(i = 0; i < numSens; i++) {
        for(j = 0; j < 3; j++) {
            Serial.print("theta: ");
            Serial.print(sensorTheta + shiftTheta);
            Serial.print(" ");
            Serial.println(distMatrix[i][j]);
            sensorTheta += 15 * (180.0 / M_PI); 
        }
    }
    Serial.println("readings for this batch complete, madafaka");
}

void turnLeft()
{
    moveLeft();
    delay(rightAngleDelay);    
    stahp();
}
