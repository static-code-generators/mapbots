const int AL = 2;
const int BL = 3;
const int AR = 4;
const int BR = 5;
const int encL = 6;
const int encR = 7;

void moveLeft();
void moveRight();
void moveForward();
void moveBackward();
void stahp();

void setup() {
  // put your setup code here, to run once:
    pinMode(AL, OUTPUT);
    pinMode(BL, OUTPUT);
    pinMode(AR, OUTPUT);
    pinMode(BR, OUTPUT);

    pinMode(encL, INPUT);
    pinMode(encR, INPUT);

    Serial.begin(9600);
}

void loop() {
    // put your main code here, to run repeatedly:
    moveForward();
    delay(1000);
}


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
