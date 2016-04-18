const int trig = 8;
const int echo = 10;
const int s[3] = {11, 12, 13};

void setup() {
    // put your setup code here, to run once:
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(s[0], OUTPUT);
    pinMode(s[1], OUTPUT);
    pinMode(s[2], OUTPUT);
    Serial.begin(9600);
}

float takereading(int sensor)
{
    for (int i = 0; i < 3; ++i) {
        digitalWrite(s[i], (sensor >> i) & 1);
        Serial.print((sensor >> i) & 1);
        //delay(10);
    }
    Serial.print(", ");
    delay(10);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    return pulseIn(echo, HIGH, 25000) / 2.0 / 29.1;
    
}
void loop() {
    int i;
    for (i=0; i<8; i++) {
        Serial.print(i);
        Serial.print(": ");
        Serial.println(takereading(i));
        delay(100);
    }
    delay(2000);
}
