int echoPin = 7; // Echo Pin
int trigPin = 8; // Trigger Pin
int LEDPin = 13;// connected LED
int maximumRange = 4; // Maximum range of sensor in metres
int minimumRange = 0; // Minimum range needed
float pingTime=0;
float targetDistance=0; // Duration used to calculate distance
int speedofSound=336; //constant in m/s
void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT); //sonic pulse from trigPin
  pinMode(echoPin, INPUT);   //detection at Echo Pin
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
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
    Serial.println("-1");
    digitalWrite(LEDPin, HIGH); 
  }
  else {
    /* Send the distance to the computer using Serial protocol, and
    turn LED OFF to indicate successful reading. */
    Serial.println(targetDistance);
    digitalWrite(LEDPin, LOW); 
  }
  //Delay 1s before next reading.
  delay(1000);
}

