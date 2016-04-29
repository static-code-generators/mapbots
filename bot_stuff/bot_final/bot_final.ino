#include <Servo.h>

#include "Direction.h"
#include "Constants.h"
#include "Sensor.h"

#define M_PI 3.14159265358979323846
// Final Arduino code for mapbot


enum direction
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

// all distances are in millimeters, unless otherwise specified
// so are your mother's boobs
// constant values
const float obstacleAvoidDist = 300; // maximum distance of obstacle at which bot should avoid obstacle
const int rightAngleDelay = 800; // delay in milliseconds required for bot to turn 90 degrees

// global variables for usage across functions
direction currDir = NORTH; // currDir is orientation wrt origin, assuming that robot starts in NORTH direction
direction pendingDir = EAST;
int obstacleFound = 0; // flag for checking whether obstacle was found in path of bot

/*
* contains readings from sensors, the angles wrt. present orientation look like
* A = {{0, 45, 90, 135, 180, 225, 270, 315},
* {15, 60, 105, 150, 195, 240, 285, 330},
* {30, 75, 120, 165, 210, 255, 300, 345}}
* so distMatrix[i][j] = distance at angle(A[i][j])
*/
float distMatrix[3][8];

//Bot position(where we are)
float xPos = 0;
float yPos = 0;

float straightLineDist = 0;
float unitDist = 50;

Servo myservo;

// move distance dist forward
void moveDistance(double dist);
// turn 90-degrees left
void turnLeft();
float findObstacleDist();
void updatePos(float distanceTravelled);

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
    while (obstacleFound == 1 || straightLineDist > 2048.0) {
        turnLeft();
        takeMultipleReadings();
        obstacleFound = (distMatrix[0][0] < obstacleAvoidDist);
        straightLineDist = 0;
    }

    moveDistance(unitDist);
    straightLineDist += unitDist;
    takeMultipleReadings();
    sendReadings();
    delay(1000);
}

void updatePos(float distanceTravelled)
{
    switch (currDir) {
        case NORTH: yPos += distanceTravelled; break;
        case EAST: xPos += distanceTravelled; break;
        case SOUTH: yPos -= distanceTravelled; break;
        case WEST: xPos -= distanceTravelled; break;
    }
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

void moveDistance(double dist) {
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

void turnLeft()
{
    moveLeft();
    delay(rightAngleDelay);    
    stahp();
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
