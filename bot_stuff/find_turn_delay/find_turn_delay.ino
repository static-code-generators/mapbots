#include "Constants.h"
#include "Direction.h"

int upper = 1000;
int lower = 200;

char resp;

void setup()
{
    pinMode(AL, OUTPUT);
    pinMode(BL, OUTPUT);
    pinMode(AR, OUTPUT);
    pinMode(BR, OUTPUT);
    Serial.begin(9600);
};

void loop()
{
    int mid = (upper + lower) / 2;
    for(int i = 0; i < 8; i++)
    {
        turnLeft(mid);
        delay(500);
    }
    Serial.println("Too much: 2, Too little: 1, Perfect!: 0");
    while(!Serial.available());
    resp = Serial.read();
    if(resp == '2')
        upper = mid;
    else if(resp == '1')
        lower = mid;
    else if(resp == '0')
        Serial.println(mid); 
}

void turnLeft(int currDelay)
{
    moveLeft();
    delay(currDelay);    
    stahp();
}
