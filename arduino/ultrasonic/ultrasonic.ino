#include<SoftwareSerial.h>

#define SSID "ECE CLUB ROOM"
#define PASS "ece@iiitd"
#define IP "10.0.0.4" // Server IP
#define PORT "5555" // Server port

const int echoPin = 7; // Echo Pin
const int trigPin = 8; // Trigger Pin
const int LEDPin = 13;// connected LED
const int maximumRange = 4; // Maximum range of sensor in metres
const int minimumRange = 0; // Minimum range needed
float pingTime = 0;
float targetDistance = 0; // Duration used to calculate distance
const int speedofSound = 336; //constant in m/s

int id = 0;

int writeString (String buffer, int len);
int connectWiFi();
int connectServer();
void errorLED(int errorCode);

enum errorType
{
  noModule,
  noWiFi,
  noServer,
  badData
};

SoftwareSerial espSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);

  while (!moduleWorking()) {
    errorLED(noModule);
  }

  pinMode(trigPin, OUTPUT); //sonic pulse from trigPin
  pinMode(echoPin, INPUT);   //detection at Echo Pin
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)

  while (!connectWiFi()) {
    errorLED(noWiFi);
  }
  int temp;
  while ((temp = connectWiFi()) == 0) {
    errorLED(noWiFi);
    Serial.println("connectWiFi: " + String(temp));
    temp = connectServer();
  }
  while ((temp = connectServer()) == 0) {
    errorLED(noServer);
    Serial.println("connectServer: " + String(temp));
    temp = connectServer();
  }
}

void loop() {
  delay(2000);
  /*while (espSerial.available() > 0) {
    Serial.write(espSerial.read());
    }*/
  espSerial.flush();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2000);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);
  pingTime = pulseIn(echoPin, HIGH);
  /* Calculate the distance (in metres) based on the speed of sound. */
  pingTime = pingTime / 1000000.; //calculate time in seconds
  targetDistance = (speedofSound * pingTime); //twice distance in metres
  targetDistance = targetDistance / 2;
  if (targetDistance >= maximumRange || targetDistance <= minimumRange)
  {
    /* Send a negative number to computer and Turn LED ON
       to indicate "out of range" */
    writeString("1 -1", 4);
    errorLED(badData);
  }
  else {
    /* Send the distance to the computer using Serial protocol, and
       turn LED OFF to indicate successful reading. */
    String stringOne = "1 ";
    String targetDistanceString = stringOne + String(targetDistance);
    targetDistanceString += "\r\n";
    Serial.println(targetDistanceString);
    if (!writeString(targetDistanceString, targetDistanceString.length()))
      errorLED(noModule);
    //    digitalWrite(LEDPin, LOW);
  }
  //Delay 1s before next reading.
  delay(1000);
}


int connectWiFi() {
  espSerial.print("AT+CWMODE=1\r\n");
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  espSerial.print(cmd);
  espSerial.print("\r\n");
  Serial.print(cmd);
  espSerial.print("\r\n");
  delay(5000);
  if (int res = findOK()) {
    Serial.println("FIND OUT ->" + String(res));
    //    monitor.println("RECEIVED: OK");
    return 1;
  } else {
    return 0;
  }
}

int moduleWorking() {
  Serial.println("AT");
  espSerial.print("AT");
  espSerial.print("\r\n");
  return findOK();
}

int connectServer() {
  Serial.println("AT+CIPMUX=1");
  espSerial.print("AT+CIPMUX=1\r\n");
  String cmd = "AT+CIPSTART=0,\"TCP\"";
  cmd += ",\"";
  cmd += IP;
  cmd += "\",";
  cmd += PORT;
  cmd += "\r\n";
  Serial.println(cmd);
  espSerial.print(cmd);
  delay(2000);

  if (int res = findOK()) {
    //Serial.println("FIND OUT ->" + String(res));
    return 1;
  }
  else {
    return 0;
  }
}

int writeString (String buffer, int len) {
  Serial.print("AT+CIPSEND=");
  Serial.print(id);
  Serial.print(",");
  Serial.print(len);
  Serial.print("\r\n");
  Serial.print(buffer);
  Serial.print("\r\n");

  String cmd = "AT+CIPSEND=";
  cmd += String(id);
  cmd += ",";
  cmd += String(len);
  cmd += "\r\n";

  espSerial.print(cmd);
  delay(5);

  espSerial.print(buffer);

  return 1;
}

void errorLED(errorType error) {
  if (error == noModule) {
    int state = HIGH;
    int toggle = HIGH ^ LOW;
    for (int i = 0; i < 7; state ^= toggle, ++i)
    {
      digitalWrite(LEDPin, state);
      delay(143);
    }
  } else if (error == noWiFi) {
    digitalWrite(LEDPin, HIGH);
    delay(1000);
  } else if (error == noServer) {
    int state = HIGH;
    int toggle = HIGH ^ LOW;
    for (int i = 0; i < 3; state ^= toggle, ++i)
    {
      digitalWrite(LEDPin, state);
      delay(333);
    }
  } else if (error == badData) {
    int state = HIGH;
    int toggle = HIGH ^ LOW;
    for (int i = 0; i < 5; state ^= toggle, ++i)
    {
      digitalWrite(LEDPin, state);
      delay(200);
    }
  }
  digitalWrite(LEDPin, LOW);
}

int findOK() {
  char inData[100];
  char inChar;
  int index = 0;
  delay(5);
  while (espSerial.available() > 0) // Don't read unless
    // there you know there is data
  {
    //Serial.println(espSerial.available());
    if (index < 99) // One less than the size of the array
    {
      inChar = espSerial.read(); // Read a character
      inData[index] = inChar; // Store it
      index++; // Increment where to write next
    } else {
      break;
    }
  }

  inData[index] = '\0'; // Null terminate the string
  Serial.println(inData);
  int foundOK = 0;
  for (index = 0; inData[index + 1]; index++) {
    if (inData[index] == 'O' && inData[index + 1] == 'K') {
      foundOK = 1;
      break;
    }
  }
  return foundOK;
}

