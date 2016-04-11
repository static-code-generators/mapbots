#include <Servo.h>

#include "Direction.h"
#include "Constants.h"
#include "Sensor.h"

// Final Arduino code for mapbot

/*
TODO:
* Add x-y displacement of bot wrt origin
* Add angle of 24 sensors wrt origin
* Add final takeMultipleReadings() function to Sensor.h header
* Add sendReadings() function for properly formatting the sent data
*/

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
const rightAngleDelay = 800; // delay in milliseconds required for bot to turn 90 degrees

// global variables for usage across functions
double currSide = sideIncr;
direction currDir = NORTH; // currDir is orientation wrt origin, assuming that robot starts in NORTH direction
direction pendingDir = EAST;
int obstacleFound = 0; // flag for checking whether obstacle was found in path of bot

//Bot position(where we are)
float xPos = 0; //millimeters
float yPos = 0;

float straightLineDist = 0;
float unitDist = 50;

Servo myservo;

// move distance dist forward
void moveDistance(double dist);
void turnLeft();
float findObstacleDist(direction currDir);

void setup()
{
    pinMode(AL, OUTPUT);
    pinMode(BL, OUTPUT);
    pinMode(AR, OUTPUT);
    pinMode(BR, OUTPUT);
    pinMode(encL, INPUT);
    pinMode(encR, INPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(echoPin, INPUT);
    myservo.attach(servoPin);

    Serial.begin(9600);
}

void loop()
{
    while (obstacleFound == 1 || straightLineDist > 6000) {
        turnLeft();
        obstacleFound = (findObstacleDistance() < obstacleAvoidDistance);
        straightLineDist = 0;
    }

    moveDistance(unitDist);
    takeMultipleReadings();
    sendReadings();
    delay(1000);
}

//returns forward distance in mm
float findObstacleDist()
{
    float obstacleDist;
    switch(currDir) {
        // still need to fill in sensor numbers from matrix
        case NORTH: obstacleDist = distMatrix[1][]; break;
        case EAST: obstacleDist = distMatrix[1][]; break;
        case SOUTH: obstacleDist = distMatrix[1][]; break;
        case WEST: obstacleDist = distMatrix[1][]; break;
    }
    return obstacleDist;
}

void updatePos(direction currDir, int numPulses)
{
    float distanceTravelled = numPulses * distPerPulse;
    case(currDir) {
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
    int i, j;
    int sensorTheta = 0;
    int shiftTheta;
    case(currDir) {
        case NORTH: shiftTheta = ; break;
        case EAST: shiftTheta = ; break;
        case SOUTH: shiftTheta = ; break;
        case WEST: shiftTheta = ; break;
    }

    for(i = 0; i < numSens; i++) {
        for(j = 0; j < 3; j++) {
            Serial.print("theta: "); Serial.print(sensorTheta + shiftTheta);
            Serial.print(" ");
            Serial.print
            sensorTheta += 15;
        }
    }
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
