#include <Arduino.h>
#include<WiFi.h>
#include<WiFiUdp.h>

#define ain1 26
#define ain2 25
#define pwma 33  

// Motor B pins
#define bin1 14
#define bin2 12
#define PWMB 13  

#define stdby 27
const int trigPin = 2;  
const int echoPin = 4;
float duration, distance;

#define PWM_CHANNEL_A 0
#define PWM_CHANNEL_B 1
const char* ssid = "Robot";
const char* password = "adk05eie";

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiUDP udp;
const int udpPort = 4210;
char incomingPacket[255];

unsigned long previousMillis = 0;
const long interval = 500; 
int packetSize = 0;
void setup(){
  Serial.begin(19200);
  Serial.println("BEGINNING");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, password);
    Serial.println(WiFi.softAPIP());  

    udp.begin(udpPort);
    Serial.println("UDP server started");

    pinMode(ain1, OUTPUT);
    pinMode(ain2, OUTPUT);
    pinMode(pwma, OUTPUT);
    pinMode(stdby, OUTPUT);
    pinMode(bin1, OUTPUT);
    pinMode(bin2, OUTPUT);
    pinMode(PWMB, OUTPUT);
  
    ledcAttach(PWM_CHANNEL_A, 1000, 8);
    ledcAttach(PWM_CHANNEL_B, 1000, 8);
    pinMode(trigPin, OUTPUT);  
	  pinMode(echoPin, INPUT); 

}
int readUS(int trigPin,int echoPin){
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);  
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration*.0343)/2;  
  return distance;

}
void moveForward(int spd){
  digitalWrite(stdby, HIGH); 
  digitalWrite(ain1,HIGH);
  digitalWrite(ain2,LOW);
  ledcWrite(PWM_CHANNEL_A, spd);
  digitalWrite(bin1,HIGH);
  digitalWrite(bin2,LOW);
  ledcWrite(PWM_CHANNEL_B, spd);
}
void moveBackward(int spd){
  digitalWrite(stdby,HIGH);
  digitalWrite(ain1,LOW);
  digitalWrite(ain2,HIGH);
  ledcWrite(PWM_CHANNEL_A, spd);
  digitalWrite(bin1,LOW);
  digitalWrite(bin2,HIGH);
  ledcWrite(PWM_CHANNEL_B, spd);
}
void turnLeft(int spd){
  digitalWrite(stdby,HIGH);
  digitalWrite(ain1,LOW);
  digitalWrite(ain2,HIGH);
  ledcWrite(PWM_CHANNEL_A, spd);
  digitalWrite(bin1,HIGH);
  digitalWrite(bin2,LOW);
  ledcWrite(PWM_CHANNEL_B, spd);
}
void turnRight(int spd){
  digitalWrite(stdby,HIGH);
  digitalWrite(ain1,HIGH);
  digitalWrite(ain2,LOW);
  ledcWrite(PWM_CHANNEL_A, spd);
  digitalWrite(bin1,LOW);
  digitalWrite(bin2,HIGH);
  ledcWrite(PWM_CHANNEL_B, spd);
}
void stop(){
  digitalWrite(stdby,LOW);
}
void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
      int len = udp.read(incomingPacket, 255);
      if (len > 0) {
          incomingPacket[len] = '\0';
      }
      int fingers_extended = atoi(incomingPacket);
      Serial.print("Received fingers: ");
      Serial.println(fingers_extended);
      if (fingers_extended == 0) {
        stop();
        Serial.println("Stop");
      } 
      else if (fingers_extended == 1) {
        int dist = readUS(trigPin,echoPin);
        if (dist>10){
        moveForward(100);
        delay(1000);
        Serial.println("Forward EASY");
       }
       else{
        Serial.println("CANNOT MOVE OBSTRUCTION");
       }
        
      }
      else if (fingers_extended == 2) {
        moveBackward(80);
        delay(1000);
        Serial.println("Back");
      } 
      else if (fingers_extended == 3) {
        turnLeft(70);
        delay(500);
        Serial.println("Left");
      } 
      else if (fingers_extended == 4) {
        turnRight(70);
        delay(500);
        Serial.println("Right");
      } 
      else {
        stop();
        Serial.println("Stop");
      } 
      delay(300);
  }
  else{
    stop();
    Serial.println("Data not recieved");
    delay(300);
  }
}
  
