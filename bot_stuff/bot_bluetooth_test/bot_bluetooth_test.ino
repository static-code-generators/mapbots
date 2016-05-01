char BYTE;
int LED = 13;

void setup()
{
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
}

void loop()
{
    Serial.println("Press 1 to turn on, 0 to turn off");
    while(!Serial.available());
    BYTE = Serial.read();
    if(BYTE == '0')
        digitalWrite(LED, LOW);
    if(BYTE == '1')
        digitalWrite(LED, HIGH);
    delay(50);
}

