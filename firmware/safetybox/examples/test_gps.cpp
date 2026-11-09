#include <TinyGPS++.h> 
// Trên ESP32-S3, Arduino Core đã tích hợp sẵn Serial1 và Serial2 nên không cần tự khai báo biến HardwareSerial mới.

TinyGPSPlus gps;

// Chân GPIO 4 và 5 trên ESP32-S3 hoàn toàn an toàn để dùng làm UART
// Nối: TX của GPS -> RX_PIN (4) của ESP32-S3
//      RX của GPS -> TX_PIN (5) của ESP32-S3
#define RX_PIN 4
#define TX_PIN 5

unsigned long lastGpsStatus = 0;

void setup(void) {
  Serial.begin(115200); // Tốc độ Serial Monitor
  
  // Khởi tạo UART2 có sẵn với cấu hình baudrate 9600 cho ESP32-S3
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); 
  Serial.println("Đang khởi động GPS trên ESP32-S3 UART2 (RX: 5, TX: 4)...");
}

void loop() {
  while (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      
      if (gps.location.isValid()) {
        float lat = gps.location.lat();
        float lng = gps.location.lng();
        
        Serial.print("Latitude   = ");
        Serial.println(lat, 6);
        Serial.print("Longitude  = ");
        Serial.println(lng, 6);
        
        // In ra link Google Maps trực tiếp
        Serial.print("Google Maps: http://maps.google.com/maps?q=loc:");
        Serial.print(lat, 6);
        Serial.print(",");
        Serial.println(lng, 6);
      }
      else {
        Serial.println("Location Invalid (Đang tìm vệ tinh...)");
      }
        
      if (gps.altitude.isValid()) {
        Serial.print("Altitude   = ");
        Serial.print(gps.altitude.meters());
        Serial.println(" meters");
      }
        
      if (gps.speed.isValid()) {
        Serial.print("Speed      = ");
        Serial.print(gps.speed.kmph());
        Serial.println(" kmph");
      }
        
      if (gps.time.isValid()) {
        Serial.print("Time (VN)  : ");
        int hour = gps.time.hour() + 7; // Giờ Việt Nam
        if (hour >= 24) hour -= 24;
        
        if (hour < 10) Serial.print("0");
        Serial.print(hour);
        Serial.print(":");
        if (gps.time.minute() < 10) Serial.print("0");
        Serial.print(gps.time.minute());
        Serial.print(":");
        if (gps.time.second() < 10) Serial.print("0");
        Serial.println(gps.time.second());
      }
        
      if (gps.date.isValid()) {
        Serial.print("Date       : ");
        if (gps.date.day() < 10) Serial.print("0");
        Serial.print(gps.date.day());
        Serial.print("/");
        if (gps.date.month() < 10) Serial.print("0");
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.println(gps.date.year());
      }
        
      if (gps.satellites.isValid()) {
        Serial.print("Satellites = ");
        Serial.println(gps.satellites.value());
      }
        
      Serial.println("-----------------------------------");
    }
  }

  // Cảnh báo nếu module GPS chưa kết nối đúng dây hoặc module chưa gửi dữ liệu
  if (millis() - lastGpsStatus >= 2000) {
    lastGpsStatus = millis();
    Serial.print("Đang chờ GPS | bytes: ");
    Serial.print(gps.charsProcessed());
    Serial.print(" | câu có fix: ");
    Serial.print(gps.sentencesWithFix());
    Serial.print(" | checksum lỗi: ");
    Serial.println(gps.failedChecksum());
    
    if (gps.charsProcessed() < 10) {
      Serial.println("CẢNH BÁO: Không nhận được dữ liệu GPS. Hãy kiểm tra lại dây nối TX/RX.");
    }
  }
}