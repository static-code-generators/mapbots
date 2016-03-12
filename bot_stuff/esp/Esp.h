#ifndef ESP_H
#define ESP_H

#include<SoftwareSerial.h>

#define SSID "ECE CLUB ROOM"
#define PASS ""
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

boolean writeString (String buffer, int len);
boolean connectWiFi();
boolean connectServer();

SoftwareSerial espSerial(10,11); // RX, TX

#endif
