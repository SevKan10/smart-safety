#include <Arduino.h>

constexpr int SIM_RX_PIN = 15;
constexpr int SIM_TX_PIN = 16;
constexpr uint32_t SIM_BAUD = 115200;

HardwareSerial sim(2);
uint32_t lastCommandAt = 0;

void setup() {
  Serial.begin(115200);
  sim.begin(SIM_BAUD, SERIAL_8N1, SIM_RX_PIN, SIM_TX_PIN);
  delay(1000);
  Serial.println("SIM A7680C test started.");
}

void loop() {
  while (sim.available()) {
    Serial.write(sim.read());
  }
  while (Serial.available()) {
    sim.write(Serial.read());
  }

  if (millis() - lastCommandAt >= 3000) {
    lastCommandAt = millis();
    sim.println("AT");
  }
}
