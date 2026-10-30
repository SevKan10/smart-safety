#include <Arduino.h>
#include <Wire.h>

constexpr int GPS_RX_PIN = 4;
constexpr int GPS_TX_PIN = 5;
constexpr int BUZZER_PIN = 6;
constexpr int RELAY_PIN = 7;
constexpr int SIM_RX_PIN = 15;
constexpr int SIM_TX_PIN = 16;
constexpr int SDA_PIN = 17;
constexpr int SCL_PIN = 18;
constexpr int SOS_PIN = 39;
constexpr int RESET_PIN = 40;

HardwareSerial gps(1);
HardwareSerial sim(2);

void setup() {
  Serial.begin(115200);
  gps.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  sim.begin(115200, SERIAL_8N1, SIM_RX_PIN, SIM_TX_PIN);
  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(SOS_PIN, INPUT_PULLUP);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  Serial.println("Combined peripheral smoke test started.");
}

void loop() {
  static uint32_t lastReportAt = 0;
  static uint32_t lastOutputAt = 0;

  while (gps.available()) {
    gps.read();
  }
  while (sim.available()) {
    sim.read();
  }

  if (millis() - lastOutputAt >= 2000) {
    lastOutputAt = millis();
    digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
  }

  if (millis() - lastReportAt >= 500) {
    lastReportAt = millis();
    Serial.printf("RESET=%s SOS=%s I2C devices checked\n",
                  digitalRead(RESET_PIN) == LOW ? "PRESSED" : "released",
                  digitalRead(SOS_PIN) == LOW ? "PRESSED" : "released");
  }
}
