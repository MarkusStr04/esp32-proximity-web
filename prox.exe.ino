#include <WiFi.h>
#include <Firebase.h>
#include "time.h"
// pini
#define SENSOR_PIN 18
//#define LED_PIN 2

const char* ssid = "DIGI-wj5P";
const char* password = "X55hzbJ74G";
const char* FIREBASE_HOST = "https://esp32-house-entry-alert-default-rtdb.europe-west1.firebasedatabase.app";

// retinem starea anterioara
int lastState = HIGH;
Firebase firebase(FIREBASE_HOST);

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7200;   // +1 ora pentru Romania
const int daylightOffset_sec = 3600; // ora de vara
void setup() {
  pinMode(SENSOR_PIN, INPUT);
  //pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);

  // conectare WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("NTP initialized!");

  
}
String getTimestamp() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "0000-00-00 00:00:00"; // fallback
  }
  char buf[25];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buf);
}

void loop() {

  int currentState = digitalRead(SENSOR_PIN);

  // detectam schimbare stare
  if (currentState != lastState) {

    // doar la deschidere trimitem alerta
    if (currentState == LOW) {
      Serial.println("USA DESCHISA - trimit alerta");

      if (WiFi.status() == WL_CONNECTED) {
 
        String path = "/door_events/" + String(millis());
        
        String timestamp = getTimestamp(); 
        
        String status = "OPEN";
         if (firebase.setString(path + "/status", status)) {
          firebase.setString(path + "/timestamp", timestamp);
          Serial.println("Trimis cu succes!");
        } else {
          Serial.println("Eroare: ");
        }
      }
    }
    lastState = currentState;
  }
  delay(200);
}