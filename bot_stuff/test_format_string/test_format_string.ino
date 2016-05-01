#include "Sensor.h"
#include "Direction.h"
#include "Constants.h"
#include <Servo.h>
#include <math.h>

enum direction
{
    NORTH = 0,
    WEST = 1,
    SOUTH = 2,
    EAST = 3
};

int rightAngleDelay = 800;
Servo myservo;

int xPos = 0;
int yPos = 0;
direction currDir = NORTH;

float distMatrix[3][8];

void turnServoAngle(int startAngle, int stopAngle);
void takeMultipleReadings();
void takeReading(int row);
void sendReadings();
void turnLeft();

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
    myservo.write(0);

    Serial.begin(9600);
}

void loop()
{
    takeMultipleReadings();
    sendReadings();
    turnLeft();
    currDir = (direction)(((int)currDir + 1) % 4);
}

void takeMultipleReadings()
{
    int i;
    int servoStartAngle = 0;
    int servoStopAngle = 15;
    for(i = 0; i < 3; i++)
    {
        turnServoAngle(servoStartAngle, servoStopAngle);
        delay(500);
        takeReading(i);
        delay(500);
        servoStartAngle += 15;
        servoStopAngle += 15;
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

void sendReadings()
{
    // Units for data:
    // x-y coords: in cm
    // sensor theta: in degrees

    // String format:
    // id - 42
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
         0
         |
    6 --   -- 2
         |
         4
    This is wrt 0 position of servo. 

    The 'theta' being sent is:
              0
              |
    M_PI/2 --   -- 3 * M_PI/2
              |
             M_PI
    This is wrt the NWSE from the bot orientation at origin.
    */

    int i, j;
    float sensorTheta = 0;
    float shiftOfDir[4] = {0.0, M_PI / 2, M_PI, (3 * M_PI) / 2};
    float shiftTheta = shiftOfDir[currDir];
    
    // outer for-loop iterates as 0, 7, 6, 5... 1 due to orientation of sensors; we want to go counter-clockwise
    for(i = numSens; i > 0; i--) {
        for(j = 0; j < 3; j++) {
            Serial.print(42);
            Serial.print(" ");
            Serial.print(distMatrix[i % numSens][j]);
            Serial.print(" ");
            Serial.print(xPos);
            Serial.print(" ");
            Serial.print(yPos);
            Serial.print(" ");
            Serial.println(fmod(sensorTheta + shiftTheta, 2 * M_PI));
            sensorTheta += 15 * (180.0 / M_PI); 
        }
    }
//    Serial.println("readings for this batch complete, madafaka");
}

void turnLeft()
{
    moveLeft();
    delay(rightAngleDelay);    
    stahp();
}

void turnServoAngle(int startAngle, int stopAngle)
{
    for(int pos = startAngle; pos <= stopAngle; pos++)
    {
        myservo.write(pos);
        delay(15);
    }
}
