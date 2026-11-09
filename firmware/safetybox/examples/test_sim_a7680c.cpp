#include <Arduino.h>

constexpr int SIM_RX_PIN = 18;
constexpr int SIM_TX_PIN = 17;
constexpr uint32_t SIM_BAUD = 115200;

// Sử dụng Serial2 cho module SIM trên ESP32-S3
#define sim Serial2

// Hàm phụ trợ: Gửi lệnh AT và đợi phản hồi để in ra ngay lập tức
void sendATCommand(const char* command, int waitTime = 1000) {
  Serial.print("\n>>> GỬI LỆNH: ");
  Serial.println(command);
  
  sim.println(command); // Gửi lệnh xuống module SIM
  delay(waitTime);      // Đợi module xử lý và trả lời
  
  // Đọc và in tất cả những gì module phản hồi
  while (sim.available()) {
    Serial.write(sim.read());
  }
  Serial.println("-----------------------------------");
}

// Hàm gọi điện thoại
void makeCall(String phoneNumber) {
  Serial.print("\n>>> ĐANG GỌI TỚI SỐ: ");
  Serial.println(phoneNumber);
  
  sim.print("ATD");
  sim.print(phoneNumber);
  sim.println(";"); // Bắt buộc có chấm phẩy
  
  delay(1000);
  while (sim.available()) {
    Serial.write(sim.read());
  }
  Serial.println("Gõ ATH và Enter để cúp máy.");
}

// Hàm gửi tin nhắn
void sendSMS(String phoneNumber, String message) {
  Serial.println("\n>>> BẮT ĐẦU GỬI SMS...");
  
  // 1. Chuyển sang chế độ Text
  sim.println("AT+CMGF=1");
  delay(500);
  while(sim.available()) Serial.write(sim.read());

  // 2. Nhập số điện thoại người nhận
  sim.print("AT+CMGS=\"");
  sim.print(phoneNumber);
  sim.println("\"");
  delay(1000); // Chờ dấu > xuất hiện
  
  // 3. Ghi nội dung tin nhắn
  sim.print(message);
  delay(100);
  
  // 4. Gửi mã CTRL+Z (Ký tự thứ 26 trong bảng mã ASCII) để ra lệnh chốt gửi
  sim.write(26); 
  
  Serial.println("\nĐã nạp nội dung, đang chờ nhà mạng xác nhận (có thể mất 3-5 giây)...");
  
  // Chờ và in phản hồi từ nhà mạng (sẽ trả về +CMGS: [ID tin nhắn] và chữ OK)
  delay(5000); 
  while(sim.available()) {
    Serial.write(sim.read());
  }
  Serial.println("\n-----------------------------------");
}

void setup() {
  Serial.begin(115200);
  sim.begin(SIM_BAUD, SERIAL_8N1, SIM_RX_PIN, SIM_TX_PIN);
  
  // Đợi module 4G khởi động xong nguồn (rất quan trọng)
  delay(3000); 
  
  Serial.println("\n--- BẮT ĐẦU TEST MODULE SIM A7680C ---");
  
  // Gửi lệnh AT mồi để đồng bộ baudrate
  sim.println("AT"); delay(100);
  sim.println("AT"); delay(500);
  while(sim.available()) sim.read(); // Xóa bộ đệm rác ban đầu

  // ================================================================
  // DANH SÁCH CÁC LỆNH TEST (XÓA DẤU // ĐỂ SỬ DỤNG)
  // ================================================================
  
  // 1. Kiểm tra kết nối cơ bản (Trả về OK là mạch đã thông)
  sendATCommand("AT", 1000);

  // 2. Kiểm tra SIM (Trả về +CPIN: READY là đã nhận SIM)
  sendATCommand("AT+CPIN?", 1000);

  // 3. Kiểm tra chất lượng sóng (Số đầu tiên từ 15-31 là sóng khỏe)
  sendATCommand("AT+CSQ", 1000);

  // 4. Kiểm tra đăng ký mạng (Trả về +CREG: 0,1 là đã vào mạng thành công)
  sendATCommand("AT+CREG?", 1000);

  // 5. Kiểm tra nhà mạng đang dùng (VD: Viettel, Mobifone...)
  sendATCommand("AT+COPS?", 2000);

  // 6. Kiểm tra trạng thái 4G/LTE chi tiết (Lệnh riêng của A7680C)
  sendATCommand("AT+CPSI?", 2000);

  // 7. Đọc mã số seri in trên phôi thẻ SIM
  sendATCommand("AT+CCID", 1000);
  
  // ================================================================

  Serial.println("\nHoàn tất test tự động. Giờ bạn có thể gõ lệnh trực tiếp vào ô ở trên!");

  // ================================================================
  // 8. GỌI ĐIỆN THOẠI (Bỏ dấu // để test)
  makeCall("08xxxxxxxx"); // Thay bằng số điện thoại bạn muốn gọi  
  // 9. GỬI TIN NHẮN (Bỏ dấu // để test)
  sendSMS("08xxxxxxxx", "Xin chào! Đây là tin nhắn test từ ESP32-S3 A7680C."); // Thay bằng số điện thoại và nội dung tin nhắn
  // ================================================================
}

void loop() {
  // Chế độ Passthrough: Cho phép bạn gõ lệnh tay từ máy tính
  
  // 1. Đọc dữ liệu từ module SIM in lên màn hình
  while (sim.available()) {
    Serial.write(sim.read());
  }
  
  // 2. Đọc lệnh bạn gõ từ màn hình gửi xuống module SIM
  while (Serial.available()) {
    sim.write(Serial.read());
  }
}