#include <Arduino.h>

constexpr int GPS_RX_PIN = 4;
constexpr int GPS_TX_PIN = 5;
constexpr uint32_t GPS_BAUD = 9600;

HardwareSerial gps(1);

void setup() {
  Serial.begin(115200);
  gps.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("GPS test started. Waiting for NMEA data...");
}

void loop() {
  while (gps.available()) {
    Serial.write(gps.read());
  }
}
