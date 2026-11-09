//http://maps.google.com/maps?q=loc:
#include <TinyGPS++.h>  // Include TinyGPS++ library
#include <HardwareSerial.h> // Dùng HardwareSerial trên ESP32

TinyGPSPlus gps;

// Khai báo chân UART2 cho GPS: RX = 5, TX = 4
#define RX_PIN 4
#define TX_PIN 5
HardwareSerial gpsSerial(2); // Sử dụng UART số 2

void setup(void) {
  Serial.begin(115200); // Tốc độ Serial Monitor
  gpsSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); // Khởi tạo UART2 baudrate 9600
  
  Serial.println("Đang khởi động GPS trên ESP32 UART2 (RX: 5, TX: 4)...");
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        Serial.print("Latitude   = ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude  = ");
        Serial.println(gps.location.lng(), 6);
      }
      else
        Serial.println("Location Invalid");
        
      if (gps.altitude.isValid()) {
        Serial.print("Altitude   = ");
        Serial.print(gps.altitude.meters());
        Serial.println(" meters");
      }
      else
        Serial.println("Altitude Invalid");
        
      if (gps.speed.isValid()) {
        Serial.print("Speed      = ");
        Serial.print(gps.speed.kmph());
        Serial.println(" kmph");
      }
      else
        Serial.println("Speed Invalid");
        
      if (gps.time.isValid()) {
        Serial.print("Time (GMT) : ");
        int hour = gps.time.hour() + 7; // Chuyển sang giờ Việt Nam (GMT+7)
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
      else
        Serial.println("Time Invalid");
        
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
      else
        Serial.println("Date Invalid");
        
      if (gps.satellites.isValid()) {
        Serial.print("Satellites = ");
        Serial.println(gps.satellites.value());
      }
      else
        Serial.println("Satellites Invalid");
        
      Serial.println("-----------------------------------");
    }
  }
}