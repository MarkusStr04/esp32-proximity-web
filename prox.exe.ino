#include <WiFi.h>
#include <Firebase.h>
#include "time.h"

// pins
#define SENSOR_PIN 18
//#define LED_PIN 2

// WiFi and Firebase info
const char* ssid = "WIFI NAME HERE";
const char* password = "WIFI PASSWORD HERE";
const char* FIREBASE_HOST = "FIREBASE LINK HERE";

// store previous sensor state
int lastState = HIGH;
Firebase firebase(FIREBASE_HOST);

// NTP settings 
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7200;   // Change for your timezone This is GMT+2
const int daylightOffset_sec = 3600; // Daylight saving

void setup() {
  pinMode(SENSOR_PIN, INPUT); //set sensor as Input
  //pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);  // start serial

  // connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

//delay until wifi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // initialize NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("NTP initialized!");
  
}
// get current timestamp
String getTimestamp() {
  struct tm timeinfo; //struct to store date data
  if(!getLocalTime(&timeinfo)){ //getLocalTime get the current time and stores it in timeinfo, if it fails the if statement executes 
    return "0000-00-00 00:00:00"; // fallback
  }
  char buf[25]; //buffer to store the whole date
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buf);
}

void loop() {

  int currentState = digitalRead(SENSOR_PIN); // read sensor

   // detect state change
  if (currentState != lastState) {

  
    if (currentState == LOW) {
      Serial.println("USA OPEN - SENDING DATA");

      if (WiFi.status() == WL_CONNECTED) {
 
        String path = "/door_events/" + String(millis());
        
        String timestamp = getTimestamp(); 
        
        String status = "OPEN";

         // send data to Firebase
         if (firebase.setString(path + "/status", status)) {
          firebase.setString(path + "/timestamp", timestamp);
          Serial.println("Trimis cu succes!");
        } else {
          Serial.println("Eroare: ");
        }
      }
    }
    lastState = currentState; // update previous state
  }
  delay(100);
}