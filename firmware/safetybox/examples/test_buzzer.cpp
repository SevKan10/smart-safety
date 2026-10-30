#include <Arduino.h>

constexpr int BUZZER_PIN = 6;
constexpr uint32_t ON_TIME_MS = 250;
constexpr uint32_t OFF_TIME_MS = 250;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ON_TIME_MS);
  digitalWrite(BUZZER_PIN, LOW);
  delay(OFF_TIME_MS);
}
