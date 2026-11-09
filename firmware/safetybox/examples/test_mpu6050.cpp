#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define SDA_PIN 8
#define SCL_PIN 9

Adafruit_MPU6050 mpu;

unsigned long lastTime = 0;
// Chỉ tính vận tốc trên mặt phẳng 2D (X và Y)
float velocityX = 0.0, velocityY = 0.0;

// Biến lưu sai số tĩnh (Offset) của cảm biến
float offsetX = 0.0, offsetY = 0.0;

// Ngưỡng lọc nhiễu tĩnh (m/s^2)
const float ACCEL_THRESHOLD = 0.25; 

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!mpu.begin()) {
    Serial.println("Lỗi kết nối MPU6050!");
    while (1) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // --- BƯỚC 1: HIỆU CHUẨN TĨNH (CALIBRATION X & Y) ---
  Serial.println("Đang hiệu chuẩn MPU... Giữ thiết bị ĐỨNG YÊN!");
  float sumX = 0, sumY = 0;
  int samples = 200;

  for (int i = 0; i < samples; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    sumX += a.acceleration.x;
    sumY += a.acceleration.y;
    delay(10);
  }

  // Tính gia tốc trung bình X, Y khi đứng yên
  offsetX = sumX / samples;
  offsetY = sumY / samples;

  Serial.println("Hiệu chuẩn xong!");
  lastTime = millis();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  // --- BƯỚC 2: TRỪ SAI SỐ TĨNH ĐÃ CALIB (Đã sửa lỗi thiếu dấu trừ) ---
  float accelX = a.acceleration.x - offsetX;
  float accelY = a.acceleration.y - offsetY;

  // --- BƯỚC 3: LỌC NGƯỠNG NHIỄU (DEADZONE) ---
  if (abs(accelX) < ACCEL_THRESHOLD) accelX = 0.0;
  if (abs(accelY) < ACCEL_THRESHOLD) accelY = 0.0;

  // --- BƯỚC 4: TÍCH PHÂN & THUẬT TOÁN ZUPT (Chỉ trên 2 trục X, Y) ---
  if (accelX == 0.0 && accelY == 0.0) {
    // Đứng yên -> Force vận tốc X, Y về 0
    velocityX = 0.0;
    velocityY = 0.0;
  } else {
    // Chỉ tích phân gia tốc X và Y
    velocityX += accelX * dt;
    velocityY += accelY * dt;
  }

  // --- BƯỚC 5: TÍNH TỐC ĐỘ MẶT PHẲNG 2D ---
  float totalSpeed2D = sqrt(velocityX * velocityX + velocityY * velocityY);

  Serial.print("Vận tốc X: "); Serial.print(velocityX, 2);
  Serial.print(" m/s | Vận tốc Y: "); Serial.print(velocityY, 2);
  Serial.print(" m/s | Tốc độ mặt phẳng: "); Serial.print(totalSpeed2D * 3.6, 2); Serial.println(" km/h");

  delay(20);
}