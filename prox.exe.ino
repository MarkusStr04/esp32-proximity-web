#define SENSOR_PIN 4
#define LED_PIN 2

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int state = digitalRead(SENSOR_PIN);

  if (state == LOW) {
    Serial.println("OBIECT DETECTAT (usa deschisa)");
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println("NU e nimic");
    digitalWrite(LED_PIN, LOW);
  }

  delay(500);
}