#include<WiFi.h>
#include<WiFiUdp.h>
const char* ssid = "ESP32_2A53N";
const char* password = "ambada_kalla";
#define pwma 13
#define ain2 12
#define ain1 14  
#define stdby 27
// Motor B pins
#define bin1 26
#define bin2 25
#define pwmb 33 
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiUDP udp;
const int udpPort = 4210;
char incomingPacket[255];
unsigned long previousMillis = 0;
const long interval = 500; 
int packetSize = 0;

void setup() {
  Serial.begin(115200); // Set baud rate to 115200
  Serial.println("BEGINNING");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  udp.begin(udpPort);
  Serial.print("Listening for commands on port: ");
  Serial.println(udpPort);
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(pwma, OUTPUT);
  pinMode(stdby, OUTPUT);
  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);
  pinMode(pwmb, OUTPUT);

}
void moveForward(int spd){
  digitalWrite(stdby, HIGH); 
  digitalWrite(ain1,HIGH);
  digitalWrite(ain2,LOW);
  analogWrite(pwma, spd);
  digitalWrite(bin1,HIGH);
  digitalWrite(bin2,LOW);
  analogWrite(pwmb, spd);
}
void moveBackward(int spd){
  digitalWrite(stdby,HIGH);
  digitalWrite(ain1,LOW);
  digitalWrite(ain2,HIGH);
  analogWrite(pwma, spd);
  digitalWrite(bin1,LOW);
  digitalWrite(bin2,HIGH);
  analogWrite(pwmb, spd);
}
void turnLeft(int spd){
  digitalWrite(stdby,HIGH);
  digitalWrite(ain1,LOW);
  digitalWrite(ain2,HIGH);
  analogWrite(pwma,spd);
  digitalWrite(bin1,HIGH);
  digitalWrite(bin2,LOW);
  analogWrite(pwmb, spd);
}
void turnRight(int spd){
  digitalWrite(stdby,HIGH);
  digitalWrite(ain1,HIGH);
  digitalWrite(ain2,LOW);
  analogWrite(pwma, spd);
  digitalWrite(bin1,LOW);
  digitalWrite(bin2,HIGH);
  analogWrite(pwmb, spd);
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
        Serial.println("Received Command: 0 | Stop");
        stop();
      } 
      else if (fingers_extended == 1) {
       Serial.println("Received Command: 1 | Move Forward");   
       moveForward(140);    
      }
      else if (fingers_extended == 2) {
        Serial.println("Received Command: 2 | Move Backward"); 
        moveBackward(140);
      } 
      else if (fingers_extended == 3) {
        Serial.println("Received Command: 3 | Turn Left"); 
        turnLeft(140);
      } 
      else if (fingers_extended == 4) {
        Serial.println("Received Command: 4 | Turn Right"); 
        turnRight(140);

      } 
      else {
        Serial.println("Error 404/exec: UNKNOWN COMMAND | STOP");
        stop();
      } 
  }
  else{
    Serial.println("Data not recieved");
    delay(100);
  }
}