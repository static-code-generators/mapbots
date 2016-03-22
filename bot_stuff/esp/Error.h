//Error.h

#include <Arduino.h>

#ifndef ERROR_H
#define ERROR_H

enum errorType
{
    noModule,
    noWiFi,
    noServer,
    badData
};

void errorLED(errorType errorCode, int LEDPin);

#endif
