#include<SoftwareSerial.h>

#define SSID "ECE CLUB ROOM"
#define PASS "ece@iiitd"
#define IP "10.0.0.5" // Server IP
#define PORT "5555" // Server port

int echoPin = 7; // Echo Pin
int trigPin = 8; // Trigger Pin
int LEDPin = 13;// connected LED
int maximumRange = 4; // Maximum range of sensor in metres
int minimumRange = 0; // Minimum range needed
float pingTime = 0;
float targetDistance = 0; // Duration used to calculate distance
int speedofSound = 336; //constant in m/s

int id = 0;

boolean writeString (const char *buffer, int len);
boolean connectWiFi();
boolean connectServer();

SoftwareSerial espSerial(10,11); // RX, TX

void setup() {
  Serial.begin(9600);
  espSerial.begin(115200);

  pinMode(trigPin, OUTPUT); //sonic pulse from trigPin
  pinMode(echoPin, INPUT);   //detection at Echo Pin
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)

  connectWiFi();
  connectServer();

}

void loop() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2000); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20); 
  digitalWrite(trigPin, LOW);
  pingTime = pulseIn(echoPin, HIGH);
  /* Calculate the distance (in metres) based on the speed of sound. */
  pingTime=pingTime/1000000.; //calculate time in seconds
  targetDistance=(speedofSound*pingTime); //twice distance in metres
  targetDistance=targetDistance/2;
  if (targetDistance >= maximumRange || targetDistance <= minimumRange)
  {
    /* Send a negative number to computer and Turn LED ON 
       to indicate "out of range" */
    writeString("-1", 2);
    digitalWrite(LEDPin, HIGH); 
  }
  else {
    /* Send the distance to the computer using Serial protocol, and
       turn LED OFF to indicate successful reading. */
    String targetDistanceString = String(targetDistance);
    const char * targetDistanceCString = targetDistanceString.c_str();
    writeString(targetDistanceCString, strlen(targetDistanceCString));  
    digitalWrite(LEDPin, LOW); 
  }
  //Delay 1s before next reading.
  delay(1000);
}


boolean connectWiFi() {
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK")) {
//    monitor.println("RECEIVED: OK");
    return true;
  } else {
//    monitor.println("RECEIVED: Error");
    return false;
  }
}

boolean connectServer() {
  Serial.println("AT+CIPMUX=1");
  String cmd = "AT+CIPSTART=0,\"TCP\"";
  cmd += ",\"";
  cmd += IP;
  cmd += "\",";
  cmd += PORT; 
  Serial.println(cmd);
  delay(2000);

  if (Serial.find("OK")) {
    return true;
  }
  else {
    return false;
  }
}

boolean writeString (const char *buffer, int len) {
  Serial.print("AT+CIPSEND=");
  Serial.print(id);
  Serial.print(",");
  Serial.println(len + 1);
  Serial.println(buffer);
}
