#include <WiFi.h>
#include <ESP32Servo.h>

Servo servo1;
const int servoPin1 = 16;

#include "credentials.h"

WiFiServer server(80);
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

String header;

void setup() {
  Serial.begin(115200);

  initServo();
  initWifi();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  if(client) {
      currentTime = millis();
      previousTime = currentTime;
//      Serial.println("New Client connected.");
      String currentLine = "";
      while(client.connected() && currentTime - previousTime <= timeoutTime) {
        currentTime = millis();
        if(client.available()) {
          char c = client.read();
          //Serial.write(c);
          header += c;
          if(c == '\n') {
            if(currentLine.length() == 0) { // end of request
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              if(header.indexOf("GET /dispense") >= 0) {
                dispense();
              }
              // put the LED code here
              // /LEDprogress
              

              client.println("<button><a href='/dispense'>Dispense</a></button>");
              client.println();
              break;
            }
            else {
              currentLine = "";
            }
          } else if (c != '\r') {
            currentLine += c;
          }
          
        }
      }
      header = "";
      client.stop();
//      Serial.println("Client Disconnected");
      Serial.println();
  }
}

void dispense() {
  Serial.println("Dispensing...");
  servo1.write(30);
  delay(100);
  servo1.write(-40);
  delay(250);
  servo1.write(0);
  delay(150);
}

void ledProgress() {
  
}

//void moveServo(Servo servo, int from, int to, int speed) {
//  for(int pos = from; pos < to; pos += speed) {
//    servo.write(pos);
//    delay(10);
//  }
//}

void initServo() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50);
  servo1.attach(servoPin1, 500, 2400);
}

void initWifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
