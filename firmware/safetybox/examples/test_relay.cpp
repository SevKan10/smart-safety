#include <Arduino.h>

constexpr int RELAY_PIN = 7;
constexpr uint32_t RELAY_TIME_MS = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("Relay test started.");
}

void loop() {
  Serial.println("Relay ON");
  digitalWrite(RELAY_PIN, HIGH);
  delay(RELAY_TIME_MS);

  Serial.println("Relay OFF");
  digitalWrite(RELAY_PIN, LOW);
  delay(RELAY_TIME_MS);
}
