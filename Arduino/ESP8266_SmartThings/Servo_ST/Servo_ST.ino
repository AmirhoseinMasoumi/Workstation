#include <ESP8266WiFi.h>
#include <SmartThingsESP8266WiFi.h>
#include <Servo.h>

// Wifi and SmartThings Credentials
const char* ssid = "yourSSID";
const char* password = "yourPassword";
const char* st_device_id = "deviceID";
const char* st_access_token = "token";

// Servo Setup
Servo myServo;

int servoPin = D1;

void setup() {
  // Connect Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
}

// Connect to SmartThings
SmartThings.begin(st_device_id, st_access_token);

// Attach Servo
myServo.attach(servoPin);
}

void loop() {
  String switchValue = SmartThings.getVirtualSwitch("switch");

  if(switchValue == "on") {
    myServo.write(90);
  }
  else if(switchValue == "off") {
    myServo.write(0);
  }
  delay(200);
}