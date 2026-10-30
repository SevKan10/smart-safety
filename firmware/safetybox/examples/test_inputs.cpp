#include <Arduino.h>

constexpr int RESET_PIN = 40;
constexpr int SOS_PIN = 39;

void setup() {
  Serial.begin(115200);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(SOS_PIN, INPUT_PULLUP);
  Serial.println("RESET/SOS input test started.");
}

void loop() {
  Serial.printf("RESET: %s, SOS: %s\n",
                digitalRead(RESET_PIN) == LOW ? "PRESSED" : "released",
                digitalRead(SOS_PIN) == LOW ? "PRESSED" : "released");
  delay(250);
}
