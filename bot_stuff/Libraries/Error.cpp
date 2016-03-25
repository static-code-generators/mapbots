#include "Error.h"
#include <Arduino.h>

void errorLED(errorType error, int LEDPin) {
    if (error == noModule) {
        int state = HIGH;
        int toggle = HIGH ^ LOW;
        for (int i = 0; i < 7; state ^= toggle, ++i) {
            digitalWrite(LEDPin, state);
            delay(143);
        }
    } else if (error == noWiFi) {
        digitalWrite(LEDPin, HIGH); 
        delay(1000);
    } else if (error == noServer) {
        int state = HIGH;
        int toggle = HIGH ^ LOW;
        for (int i = 0; i < 3; state ^= toggle, ++i) {
            digitalWrite(LEDPin, state);
            delay(333);
        }
    } else if (error == badData) {
        int state = HIGH;
        int toggle = HIGH ^ LOW;
        for (int i = 0; i < 5; state ^= toggle, ++i) {
            digitalWrite(LEDPin, state);
            delay(200);
        }
    }
    digitalWrite(LEDPin, LOW);
}

