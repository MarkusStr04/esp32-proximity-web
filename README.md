## ESP32 Smart Door Monitor (IoT Project)

An IoT-based system that monitors door activity using a proximity sensor and logs each event in real-time to a cloud database.


## Overview

This project uses an ESP32 microcontroller and an IR proximity sensor to detect when a door is opened.
Each event is recorded with a timestamp and stored in Firebase Realtime Database.


##  Features

-  WiFi connectivity (ESP32)
-  Cloud data logging (Firebase Realtime Database)
-  Real-time timestamp using NTP
-  Event-based triggering (no redundant data)
-  LED debug indicator (optional)

##  How It Works

1. The IR sensor detects door state.
2. ESP32 reads the sensor value.
3. When a state change is detected:
   - A timestamp is generated using NTP.
   - Data is sent to Firebase.
4. Events are stored as structured records in the database.

##  Data Format

Each event is stored as
json
{
  "status": "OPEN",
  "timestamp": "yy-mm-dd hh:mm:ss"
}
