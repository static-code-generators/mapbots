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
    NORTH,
    EAST,
    SOUTH,
    WEST
};

// constant values
const forwardStepDist = 25; // sensor readings will be taken at every forwardStepDist cm
const double sideIncr = 25; // side of rectangular spiral will be incremented by sideIncr every loop
const float obstacleAvoidDist = 0.30; // maximum distance of obstacle at which bot should avoid obstacle
const rightAngleDelay = 800; // delay in milliseconds required for bot to turn 90 degrees

// global variables for usage across functions
double currSide = sideIncr;
direction currDir = NORTH; // currDir is orientation wrt origin, assuming that robot starts in NORTH direction
direction pendingDir = EAST;
int obstacleFound = 0; // flag for checking whether obstacle was found in path of bot

Servo myservo;

// this function is for when the bot is moving along the predefined spiral
void moveSpiralDistance(direction currDir, direction pendingDir, long double sideDist);
// this function is for when the bot has encountered an obstacle
// and is now in 'pending' state; ie. it is looking for the
// next opening to join back into the spiral
void movePendingDistance(direction currDir, direction pendingDir);
// utility function for finding distance to obstacle depending on orientation of bot
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
    if(obstacleFound == 0)
    {
        moveSpiralDistance(currDir, pendingDir, currSide);    
        currSide += sideIncr;
        moveRight();
        delay(rightAngleDelay);
    }
    else if(obstacleFound == 1)
    {
        movePendingDistance(currDir, pendingDir);
        moveRight(); 
        // turn back to direction when bot first encountered obstacle
        // so 270 degrees turn needed
        delay(3 * rightAngleDelay);
    }

    stahp();
    takeMultipleReadings();
    sendReadings();
    delay(1000);
}


void moveSpiralDistance(direction currDir, direction pendingDir, long double sideDist)
{
    moveForward();

    int reqPulses = sideDist / distPerPulse;
    int discrPulses = forwardStepDist / distPerPulse;

    int stepPulses = discrPulses;
    int numPulses = 0;

    Serial.print("target: ");
    Serial.println(reqPulses);

    while(obstacleFound != 1 && numPulses < reqPulses)
    {
        Serial.print("numPulses: ");
        Serial.println(numPulses);

        if(numPulses >= stepPulses)
        {
            stahp();
            takeMultipleReadings();
            sendReadings();
            delay(250);
            float obstacleDist = findObstacleDist(currDir);
            
            if(obstacleDist <= obstacleAvoidDist)
            {
                pendingDir = currDir;
                currDir = (currDir + 1) % 4;
                obstacleFound = 1;
                break;
            }
            stepPulses += discrPulses;
            moveForward();
        }

        int initState = digitalRead(encL);
        int currState;

        // The following 2 while loops make sure that the wheel
        // has turned through a solid slat and an empty slat
        while((currState = digitalRead(encL)) == initState)
        {
            delay(10);
        }
        while((currState = digitalRead(encL)) != initState)
        {
            delay(10);
        }
        numPulses++;
    }
    stahp();
}


void movePendingDistance(direction currDir, direction pendingDir)
{
    if(obstacleFound == 1) 
    {
        moveForward();
        
        int discrPulses = forwardStepDist / distPerPulse;
        int stepPulses = discrPulses;

        int numPulses = 0;

        float obstacleDist;
        while((obstacleDist = findObstacleDist(currDir)) <= findObstacleDist)
        {
            if(numPulses >= stepPulses)
            {
                stahp();
                takeMultipleReadings();
                sendReadings();
                delay(250);
                stepPulses += discrPulses;
                moveForward();
            }

            int initState = digitalRead(encL);
            int currState;

            // The following 2 while loops make sure that the wheel
            // has turned through a solid slat and an empty slat
            while((currState = digitalRead(encL)) == initState)
            {
                delay(10);
            }
            while((currState = digitalRead(encL)) != initState)
            {
                delay(10);
            }
            numPulses++;

            delay(50);
        }

        obstacleFound = 0;
        float distPendingTravelled = numPulses / distPerPulse; // this is for later use for x-y coords
        currDir = pendingDir;
        pendingDir = (currDir + 1) % 4;
        stahp();
    }
    else
        Serial.println("You screwed up m8.") // no provision for asserts unfortunately, so.
}

float findObstacleDist(direction currDir)
{
    float obstacleDist;
    switch(currDir)
    {
        // still need to fill in sensor numbers from matrix
        case NORTH: obstacleDist = distMatrix[1][]; break;
        case EAST: obstacleDist = distMatrix[1][]; break;
        case SOUTH: obstacleDist = distMatrix[1][]; break;
        case WEST: obstacleDist = distMatrix[1][]; break;
    }
    return obstacleDist;
}
