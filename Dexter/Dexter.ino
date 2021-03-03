#include <WiFi.h>
#include <ESP32Servo.h>

Servo servo1;
const int servoPin1 = 16;
const int initialPos = 70;
const int finalPos = 30;
int led1 = 13; //red
int led2 = 12; //red
int led3 = 14;//yellow
int led4 = 27; //yellow
int led5 = 26; //green
int led6 = 25; //green
// keep track of points/ led
int led_count = 0;

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
  initLED();
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

              if(header.indexOf("/ledprogress") > 0) {
                ledProgress();
                if(led_count == 6) {
                  dispense();
                  led_count = 0;
                }
              }
              
              client.println("<button><a href='/ledprogress'>Dispense</a></button>");
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
  digitalWrite(led1, LOW);
  delay(400);
  digitalWrite(led2, LOW);
  delay(400);
  digitalWrite(led3, LOW);
  delay(400);
  digitalWrite(led4, LOW);
  delay(400);
  digitalWrite(led5, LOW);
  delay(400);
  digitalWrite(led6, LOW);
  delay(10);
  servo1.write(finalPos);
  delay(300);
  servo1.write(initialPos);
  delay(150);
}


void ledProgress() {
  led_count += 1;
  Serial.print("Progress: ");
  Serial.println(led_count);
  if (led_count >= 1) {
    digitalWrite(led1, HIGH);
  }
  if(led_count >= 2) {
    digitalWrite(led2, HIGH);
  }
  if(led_count >= 3) {
    digitalWrite(led3, HIGH);
  }
  if(led_count >= 4) {
    digitalWrite(led4, HIGH);
  }
  if(led_count >= 5) {
    digitalWrite(led5, HIGH);
  }
  if(led_count >= 6) {
    digitalWrite(led6, HIGH);
  }
  delay(400);
}

void initLED() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
}

void initServo() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50);
  servo1.attach(servoPin1, 500, 2400);
  servo1.write(initialPos);
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
