#include<SoftwareSerial.h>
SoftwareSerial espSerial(10,11);
void setup() {
  // put your setup code here, to run once:  
  Serial.begin(115200);
  espSerial.begin(115200);


}

void loop() {
  // put your main code here, to run repeatedly:
  while (espSerial.available() > 0) {
    Serial.write(espSerial.read());
  }
  while (Serial.available() > 0) {
    espSerial.write(Serial.read());
  }


}
