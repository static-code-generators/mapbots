//Directions.h

#include "Constants.h"

#if !defined(MOVEMENT_DIRECTIONS_H)
#define MOVEMENT_DIRECTIONS_H 1

void moveLeft();
void moveRight();
void moveForward();
void moveBackward();
void stahp();

void moveLeft() {
    // move left wheel anticlock
    digitalWrite(AL, HIGH);
    digitalWrite(BL, LOW);
    // move right wheel clock
    digitalWrite(AR, LOW);
    digitalWrite(BR, HIGH);
}

void moveRight() {
    // move left wheel clock
    digitalWrite(AL, LOW);
    digitalWrite(BL, HIGH);
    // move right wheel anticlock
    digitalWrite(AR, HIGH);
    digitalWrite(BR, LOW);
}

void moveForward() {
    // move left and right clock
    digitalWrite(AL, LOW);
    digitalWrite(BL, HIGH);
    digitalWrite(AR, LOW);
    digitalWrite(BR, HIGH);
}

void moveBackward() {
    // move left and right anticlock
    digitalWrite(AL, HIGH);
    digitalWrite(BL, LOW);
    digitalWrite(AR, HIGH);
    digitalWrite(BR, LOW);
}

void stahp() {
    // stop left and right
    digitalWrite(AL, LOW);
    digitalWrite(BL, LOW);
    digitalWrite(AR, LOW);
    digitalWrite(BR, LOW);
}

#endif
