#include <Servo.h>

#include "Direction.h"
#include "Constants.h"
#include "Sensor.h"

#define M_PI 3.14159265358979323846
// Final Arduino code for mapbot

enum direction
{
    NORTH = 0,
    WEST = 1,
    SOUTH = 2,
    EAST = 3
};

// all distances are in millimeters, unless otherwise specified
// so are your mother's boobs
// constant values
const float obstacleAvoidDist = 200; // maximum distance of obstacle at which bot should avoid obstacle
const int rightAngleDelay = 650; // delay in milliseconds required for bot to turn 90 degrees

// global variables for usage across functions
direction currDir = NORTH; // currDir is orientation wrt origin, assuming that robot starts in NORTH direction
int obstacleFound = 0; // flag for checking whether obstacle was found in path of bot

/*
* contains readings from sensors, the angles wrt. present orientation look like
* A = {
* {45, 0, 315, 270, 225, 180, 135, 90},
* {60, 15, 330, 285, 240, 195, 150, 105},
* {75, 30, 345, 300, 255, 210, 165, 120}
* }
* so distMatrix[i][j] = distance at angle(A[i][j])
*/
float distMatrix[3][8];

//Bot position(where we are)
float xPos = 0;
float yPos = 0;

float straightLineDist = 0;
float unitDist = 100;

Servo myservo;

// move distance dist forward
void moveDistance(double dist);
// turn 90-degrees left
void turnLeft();
float findObstacleDist();
void updatePos(float distanceTravelled);
void takeMultipleReadings();
void takeReading(int row);
void sendReadings();
int obstacleHaiKya();

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
    //Serial.print("Current front sensor reading: ");
    //Serial.println(distMatrix[0][1]);
    while (obstacleFound == 1 || straightLineDist > 512.0) {
        turnLeft();
        currDir = (direction)(((int)currDir + 1) % 4);
        takeMultipleReadings();
        myservo.write(0);
        sendReadings();
        obstacleFound = obstacleHaiKya();
        straightLineDist = 0;
    }

    moveDistance(unitDist);
    updatePos(unitDist);
    straightLineDist += unitDist;
    takeMultipleReadings();
    myservo.write(0);
    obstacleFound = (distMatrix[0][1] != 0) && (distMatrix[0][1] < obstacleAvoidDist);
    sendReadings();
    delay(500);
}

int obstacleHaiKya()
{
    const int size = 3;
    float readings[] = {distMatrix[0][1], distMatrix[1][1], distMatrix[2][2]};
    float minimum = 10000;
    for (int i=0; i<size; i++) {
        if (readings[i] != 0 && readings[i] < minimum) {
            minimum = readings[i];
        }
    }
    return (minimum != 0 && minimum < obstacleAvoidDist);
}

void updatePos(float distanceTravelled)
{
    switch (currDir) {
        case NORTH: yPos += distanceTravelled; break;
        case WEST: xPos -= distanceTravelled; break;
        case SOUTH: yPos -= distanceTravelled; break;
        case EAST: xPos += distanceTravelled; break;
    }
}

void sendReadings()
{
    // Units for data:
    // x-y coords: in cm
    // sensor theta: in degrees

    // String format:
    // distance - float
    // x - float
    // y - float
    // theta - float

    // Now printing sensor theta along with sensor data
    // all angles in radians now
    // be careful, the code above talks in degrees at times
    
    /* 
    Convention for direction system is as follows:
         N
         |
    W --   -- E
         |
         S
    The directions are *absolute*. North is defined as the direction
    where the 0th sensor points to at 0 position of the servo at bot origin. 

    Sensors are arranged as:
         1 
         |
    7 --   -- 3 
         |
         5 
    This is wrt 0 position of servo. 

    The 'theta' being sent is:
              0
              |
    M_PI/2 --   -- 3 * M_PI/2
              |
             M_PI
    This is wrt the NWSE from the bot orientation at origin.
    */

    // distMatrix[3][8]
    
    int i, j;
    float sensorTheta = 0;
    float shiftOfDir[4] = {0.0, M_PI / 2, M_PI, (3 * M_PI) / 2};
    float shiftTheta = shiftOfDir[currDir];

    // Debugging again
//    Serial.println("Entire matrix of readings:");
//    for(i = 0; i < 3; i++)
//    {
//        for(j = 0; j < numSens; j++)
//        {
//            Serial.print(distMatrix[i][j]);
//            Serial.print(" ");    
//        }
//        Serial.print("\n");
//    }
//    Serial.println("Matrix done");
    // debugging done
    
    // outer for-loop iterates as 1, 0, 7, 6, 5... 2 due to orientation of sensors; we want to go counter-clockwise
    for(i = numSens+1; i > 1; i--) {
        for(j = 0; j < 3; j++) {
//            Serial.print(i % numSens);
//            Serial.print(",");
//            Serial.print(j);
//            Serial.print(":");
            Serial.print(distMatrix[j][i % numSens] * 10);
            Serial.print(",");
            Serial.print(xPos);
            Serial.print(",");
            Serial.print(yPos);
            Serial.print(",");
            Serial.print(fmod(sensorTheta + shiftTheta, 2 * M_PI));
            Serial.print(",");
            Serial.print(i);
            Serial.print("\n");
            sensorTheta += 15 * (180.0 / M_PI); 
        }
    }
//    Serial.println("readings for this batch complete, madafaka");
}

void moveDistance(double dist) {
    moveForward();
    int reqPulses = dist / distPerPulse;
    int numPulses = 0;
    //Serial.print("target: ");
    //Serial.println(reqPulses);
    while (numPulses < reqPulses) {
        int initValue = digitalRead(encL);
        int currValue;

        //Serial.print("HM: ");
        //Serial.println(numPulses);

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

    scanAll(distMatrix[row]); // scanAll(float distCm[])

    delay(1000);
}
