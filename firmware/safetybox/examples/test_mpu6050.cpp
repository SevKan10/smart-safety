#include <Arduino.h>
#include <Wire.h>

constexpr int SDA_PIN = 17;
constexpr int SCL_PIN = 18;
constexpr uint8_t MPU6050_ADDRESS = 0x68;
constexpr uint8_t WHO_AM_I_REGISTER = 0x75;

bool readRegister(uint8_t reg, uint8_t &value) {
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0 || Wire.requestFrom(MPU6050_ADDRESS, 1) != 1) {
    return false;
  }
  value = Wire.read();
  return true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("MPU6050 test started.");
}

void loop() {
  uint8_t whoAmI = 0;
  if (readRegister(WHO_AM_I_REGISTER, whoAmI)) {
    Serial.printf("MPU6050 found, WHO_AM_I = 0x%02X\n", whoAmI);
  } else {
    Serial.println("MPU6050 not responding.");
  }
  delay(1000);
}
