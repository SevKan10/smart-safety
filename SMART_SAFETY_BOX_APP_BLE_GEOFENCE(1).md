# SMART SAFETY BOX
## Hệ thống hỗ trợ an toàn, bảo mật và cứu hộ khẩn cấp thông minh cho xe máy

---

## 1. Tổng quan

**SMART SAFETY BOX** là một thiết bị điện tử độc lập được lắp trên xe máy, có nhiệm vụ kết hợp:

- Xác thực người dùng và mở khóa xe bằng điện thoại qua Bluetooth Low Energy (BLE)
- Quản lý quyền truy cập nhiều người dùng
- Phát hiện tai nạn dựa trên nhiều loại dữ liệu cảm biến
- Định vị GPS
- Gửi cảnh báo và hỗ trợ cứu hộ khẩn cấp
- Ghi nhận dữ liệu sự kiện để phục vụ phân tích
- Hỗ trợ chìa khóa vật lý trong trường hợp được cấp quyền
- Kết nối với ứng dụng điện thoại để cấu hình và quản lý

Thiết kế **không yêu cầu thay đổi cấu tạo của mũ bảo hiểm**. Mũ bảo hiểm vẫn thực hiện chức năng bảo vệ đầu như bình thường, trong khi Smart Safety Box đảm nhiệm các chức năng điện tử, an toàn, bảo mật và kết nối.

---

# 2. Ý tưởng cốt lõi

Thay vì sử dụng Wi-Fi AP để cấu hình thiết bị, hệ thống chuyển sang mô hình:

```text
                 ┌──────────────────────┐
                 │      MOBILE APP      │
                 │                      │
                 │ • Tài khoản           │
                 │ • UID                 │
                 │ • Quyền truy cập      │
                 │ • Digital Key         │
                 │ • Cấu hình xe         │
                 └──────────┬───────────┘
                            │
                         Bluetooth
                            │
                            ▼
              ┌──────────────────────────┐
              │    SMART SAFETY BOX      │
              │                          │
              │ ESP32                    │
              │ BLE Authentication       │
              │ IMU                      │
              │ GPS                      │
              │ Accident Detection       │
              │ Vehicle Access Control   │
              └──────────┬───────────────┘
                         │
                 ┌───────┴────────┐
                 ▼                ▼
             Xe máy          Hệ thống an toàn
                             và cứu hộ
```

Điện thoại trở thành một dạng **Digital Key**, còn Smart Safety Box là thiết bị xác thực đặt trên xe.

---

# 3. Mục tiêu của đề tài

## 3.1. Mục tiêu tổng quát

Nghiên cứu và phát triển một hệ thống tích hợp trên xe máy có khả năng:

1. Xác thực người sử dụng bằng điện thoại thông qua BLE.
2. Quản lý quyền sử dụng xe cho nhiều người.
3. Cho phép mở khóa xe khi thiết bị hợp lệ được xác thực.
4. Hỗ trợ chế độ sử dụng chìa khóa vật lý theo quyền được cấp.
5. Phát hiện tình huống tai nạn dựa trên dữ liệu đa cảm biến.
6. Xác định vị trí xe bằng GPS.
7. Cảnh báo và hỗ trợ liên lạc khẩn cấp.
8. Ghi lại dữ liệu trước và sau sự kiện để phục vụ nghiên cứu.
9. Xây dựng nền tảng có thể mở rộng thành các nền tảng khác trong tương lai, nhưng phiên bản đầu tiên tập trung vào Android app.

## 3.2. Phạm vi triển khai hiện tại

Trong giai đoạn hiện tại, đề tài **chỉ tập trung phát triển ứng dụng Android** để giao tiếp với Smart Safety Box qua BLE và cung cấp giao diện quản lý cho người dùng.

- **Sản phẩm ưu tiên:** Android app.
- **Thiết bị mục tiêu:** điện thoại Android có hỗ trợ Bluetooth Low Energy và GPS.
- **Web dashboard:** chưa triển khai trong giai đoạn đầu; chỉ là hướng phát triển tương lai.
- **iOS:** chưa thuộc phạm vi MVP và chỉ cân nhắc sau khi Android app cùng giao thức BLE đã ổn định.

Do người phát triển chưa có nhiều kinh nghiệm viết ứng dụng, quá trình triển khai có thể sử dụng **vibe coding**: dùng AI để hỗ trợ tạo mã, giải thích lỗi, chia nhỏ tính năng và viết kiểm thử. AI là công cụ hỗ trợ; mỗi thay đổi vẫn cần được kiểm tra trên Android emulator hoặc điện thoại Android thật.

---

# 4. Điện thoại như Digital Key

## 4.1. Ý tưởng

Người dùng không cần mang theo một thiết bị Bluetooth riêng.

Điện thoại thông minh đã có BLE nên có thể đóng vai trò:

```text
Smartphone
    ↓
Digital Key
    ↓
BLE
    ↓
Smart Safety Box
    ↓
Authentication
    ↓
Vehicle Access
```

Khi người dùng đến gần xe, ứng dụng có thể phát hiện Smart Safety Box và thực hiện quá trình xác thực.

---

# 5. Không chỉ kiểm tra Bluetooth

Một thiết kế đơn giản như:

```text
BLE Connected → Unlock
```

không đủ an toàn.

Hệ thống nên sử dụng:

```text
BLE Discovery
      ↓
Authentication Challenge
      ↓
Digital Key
      ↓
Cryptographic Response
      ↓
Check UID
      ↓
Check Permission
      ↓
Check Vehicle State
      ↓
UNLOCK
```

Ví dụ:

```text
Smart Safety Box
       │
       │ Challenge
       ▼
    Smartphone
       │
       │ HMAC / Authentication Response
       ▼
Smart Safety Box
       │
       ├── UID hợp lệ?
       ├── Digital Key hợp lệ?
       ├── Quyền sử dụng hợp lệ?
       └── Key còn hiệu lực?
              │
             YES
              ↓
            UNLOCK
```

Mục tiêu là tránh việc chỉ cần phát hiện tín hiệu BLE là có thể mở xe.

---

# 6. Quản lý người dùng

Hệ thống hỗ trợ nhiều người sử dụng một xe.

Ví dụ:

```text
Vehicle
│
├── OWNER
│   └── Khang
│
├── ADMIN
│   └── Người thân A
│
└── USER / SLAVE
    └── Người thân B
```

Mỗi người dùng có thể được gán:

- UID
- Tài khoản
- Digital Key
- Vai trò
- Quyền truy cập
- Trạng thái hoạt động
- Thời hạn sử dụng nếu cần

---

# 7. Phân quyền

## OWNER

Quyền cao nhất:

- Quản lý xe
- Thêm/xóa người dùng
- Cấp hoặc thu hồi Digital Key
- Cấu hình hệ thống
- Quản lý quyền mở xe
- Cấu hình liên hệ khẩn cấp
- Cho phép hoặc vô hiệu hóa chìa khóa vật lý
- Xem lịch sử sự kiện

## ADMIN

Có thể được cấp:

- Mở xe
- Nhận cảnh báo
- Xem vị trí
- Quản lý một số người dùng
- Cấu hình một số chức năng

Quyền cụ thể do OWNER thiết lập.

## USER / SLAVE

Có thể được cấp:

- Mở xe
- Nhận thông báo
- Xem một số thông tin xe

Không được thay đổi các cấu hình quan trọng.

---

# 8. Ba phương thức truy cập xe

## 8.1. Phone Key

Người dùng có điện thoại được cấp quyền:

```text
Phone
  ↓ BLE
Smart Safety Box
  ↓
Authentication
  ↓
Permission Check
  ↓
UNLOCK
```

---

## 8.2. Family / Shared Digital Key

OWNER có thể cấp quyền cho người thân.

Ví dụ:

```text
OWNER
Khang
   │
   ├── Digital Key → Người thân A
   │
   └── Digital Key → Người thân B
```

Không cần tạo một chìa khóa vật lý riêng cho từng người.

---

## 8.3. Physical Key / Admin Override

Trong trường hợp người dùng không có điện thoại, hệ thống vẫn có thể cho phép sử dụng chìa khóa vật lý nếu OWNER/ADMIN đã cấp quyền.

Ví dụ:

```text
App
 ↓
Access Control
 ↓
Physical Key = ENABLE
 ↓
Smart Safety Box
 ↓
Cho phép sử dụng chìa khóa vật lý
```

Điều này giúp hệ thống không phụ thuộc hoàn toàn vào điện thoại.

---

# 9. Chế độ Auto Unlock

Có thể phát triển chế độ:

```text
Người dùng đến gần xe
        ↓
Smartphone phát hiện BLE
        ↓
Smart Safety Box xác thực
        ↓
Người dùng có quyền?
        ↓
       YES
        ↓
      UNLOCK
```

Tuy nhiên, không nên chỉ dựa vào RSSI để quyết định mở khóa.

RSSI chỉ nên được sử dụng như một yếu tố xác định khoảng cách tương đối.

Quá trình mở khóa nên dựa trên:

- BLE
- Authentication
- Digital Key
- UID
- Permission
- Trạng thái thiết bị

---

# 10. Smart Safety Box

## 10.1. Các thành phần chính

Có thể sử dụng:

- ESP32 / ESP32-S3
- IMU 6 trục hoặc 9 trục
- GPS
- Bluetooth Low Energy
- Bộ nhớ lưu dữ liệu
- Buzzer
- Nút SOS
- Mạch điều khiển khóa/relay tùy kiến trúc xe
- Nguồn DC từ xe
- Pin dự phòng nếu cần

---

# 11. Hệ thống an toàn

Smart Safety Box không phát hiện tai nạn chỉ dựa vào góc nghiêng.

Thay vào đó sử dụng nhiều yếu tố:

```text
Accelerometer
       +
Gyroscope
       +
Orientation / Tilt
       +
GPS Speed
       +
Motion State
       +
Sudden Deceleration
       +
Impact
       ↓
Accident Detection
```

---

# 12. Accident Score

Có thể xây dựng một chỉ số đánh giá khả năng xảy ra tai nạn:

```text
Accident Score =
    f(Acceleration,
      Angular Velocity,
      Orientation,
      Speed,
      Deceleration,
      Motion State,
      Impact)
```

Không nên cố định các trọng số một cách tùy ý.

Các ngưỡng và trọng số cần được xác định thông qua quá trình thử nghiệm.

Ví dụ:

```text
Score thấp
→ Hoạt động bình thường

Score trung bình
→ Cảnh báo người dùng

Score cao
→ Xác nhận tai nạn

Score rất cao
→ Chuyển sang quy trình khẩn cấp
```

---

# 13. Giảm cảnh báo giả

Đây là một trong những vấn đề nghiên cứu chính của đề tài.

Hệ thống phải phân biệt:

- Rẽ bình thường
- Quay đầu
- Phanh mạnh
- Đi qua ổ gà
- Dắt xe
- Dừng xe
- Đỗ xe
- Nâng xe
- Xe bị nghiêng khi dựng
- Xe bị ngã
- Va chạm
- Dừng đột ngột sau va chạm

Do đó không thể chỉ dùng:

```text
Tilt > Threshold → Accident
```

mà cần phân tích dữ liệu theo thời gian.

---

# 14. Quy trình khi phát hiện tai nạn

```text
Normal
   ↓
Abnormal Motion Detected
   ↓
Accident Score Calculation
   ↓
Possible Accident
   ↓
Warning + Countdown
   ↓
Người dùng xác nhận?
   │
   ├── YES → Emergency Procedure
   │
   └── CANCEL → Return Normal
```

Việc có khoảng thời gian xác nhận giúp giảm trường hợp hệ thống tự động gửi cảnh báo khi người dùng:

- phanh gấp
- đi qua ổ gà
- làm xe đổ khi đỗ
- thao tác với xe

---

# 15. Emergency Contact Hierarchy

Hệ thống có thể sử dụng mô hình:

```text
OWNER
  │
  └── Emergency Contacts
          │
          ├── ADMIN
          │
          ├── SLAVE 1
          │
          └── SLAVE 2
```

Khi có sự cố:

```text
Accident
   ↓
Confirm / Timeout
   ↓
Notify ADMIN
   ↓
ADMIN không phản hồi
   ↓
Notify SLAVE 1
   ↓
SLAVE 1 không phản hồi
   ↓
Notify SLAVE 2
```

Có thể mở rộng thành nhiều chiến lược:

- Gọi tuần tự
- Gửi thông báo đồng thời
- Gửi SMS
- Gọi điện
- Push Notification
- Zalo hoặc nền tảng liên lạc khác tùy khả năng triển khai

---

# 16. GPS và vị trí

Khi xảy ra sự cố, hệ thống có thể ghi nhận:

- Latitude
- Longitude
- Speed
- Timestamp
- Hướng di chuyển nếu GPS hỗ trợ

Thông báo có thể chứa:

```text
🚨 PHÁT HIỆN SỰ CỐ

Xe: Motorbike 01
Người dùng: Khang

Thời gian:
2026-XX-XX XX:XX

Tốc độ trước sự kiện:
XX km/h

Vị trí:
Latitude: XX.XXXXXX
Longitude: XX.XXXXXX

Trạng thái:
Possible Accident
```

---

# 17. Black Box

Smart Safety Box có thể lưu dữ liệu trước và sau sự kiện.

Ví dụ:

```text
Before Event: 10 seconds
After Event : 20 seconds
```

Dữ liệu:

```text
Timestamp
Ax
Ay
Az
Gx
Gy
Gz
Orientation
Speed
GPS
Accident Score
Motion State
```

Ví dụ:

```text
Time     Ax    Ay    Az    Gx    Gy    Gz    Speed    Score
T-10     ...   ...   ...   ...   ...   ...   ...      ...
T-09     ...   ...   ...   ...   ...   ...   ...      ...
...
T+20     ...   ...   ...   ...   ...   ...   ...      ...
```

Black Box rất hữu ích cho:

- Phân tích tai nạn
- Phân tích cảnh báo giả
- Huấn luyện thuật toán
- Đánh giá thuật toán
- Chứng minh kết quả nghiên cứu

---

# 18. Ứng dụng điện thoại

App là giao diện chính của người dùng.

## Dashboard

```text
┌─────────────────────────────┐
│       SMART SAFETY          │
├─────────────────────────────┤
│                             │
│ 🏍️ My Vehicle               │
│                             │
│ ● Connected                 │
│                             │
│ 🔐 Vehicle Locked           │
│                             │
│ [ Unlock ]                  │
│                             │
├─────────────────────────────┤
│ 🗺️ Location                 │
│ 🚨 Safety                   │
│ 👥 Users                    │
│ 🔑 Digital Keys             │
│ 📋 Event History            │
│ ⚙️ Settings                 │
└─────────────────────────────┘
```

---

# 19. Quản lý Digital Key

App có thể cung cấp:

```text
Digital Keys

📱 Khang's Phone
   OWNER
   Active

📱 Nguyễn A
   ADMIN
   Active

📱 Nguyễn B
   USER
   Active
```

Các thao tác:

- Add User
- Generate Digital Key
- Disable Key
- Revoke Key
- Reset Key
- Temporary Access
- Set Expiration
- Change Permission

---

# 20. Cấu hình ban đầu

Thay vì phát Wi-Fi AP để cấu hình, Smart Safety Box sử dụng BLE để thiết lập lần đầu.

Quy trình:

```text
Smart Safety Box
       ↓
BLE Pairing / Provisioning
       ↓
Mobile App
       ↓
Create / Login Account
       ↓
Register Vehicle
       ↓
Register OWNER
       ↓
Configure Emergency Contacts
       ↓
Configure Vehicle Access
       ↓
Finish
```

Wi-Fi không còn là phương thức cấu hình chính.

---

# 21. Backend

Kiến trúc có thể phát triển thành:

```text
             ┌──────────────────┐
             │   Mobile App     │
             └────────┬─────────┘
                      │
                  Internet
                      │
             ┌────────▼─────────┐
             │     Backend      │
             │                  │
             │ Auth             │
             │ User             │
             │ Vehicle          │
             │ Permission       │
             │ Event            │
             │ Notification     │
             └────────┬─────────┘
                      │
                  Internet
                      │
             ┌────────▼─────────┐
             │ Smart Safety Box │
             └──────────────────┘
```

Có thể triển khai giai đoạn đầu bằng:

- Firebase Authentication
- Firestore hoặc Realtime Database
- Firebase Cloud Messaging

Sau này có thể chuyển hoặc mở rộng sang:

- FastAPI
- PostgreSQL
- REST API
- WebSocket
- MQTT nếu thực sự cần

---

# 22. Web Dashboard — định hướng tương lai

Web dashboard **không thuộc phạm vi phát triển hiện tại**. Nội dung dưới đây chỉ mô tả khả năng mở rộng sau khi Android app, firmware và backend đã ổn định.

Trong tương lai, hệ thống có thể phát triển thêm Web Dashboard:

```text
Web Dashboard
      │
      ├── Vehicle Management
      ├── User Management
      ├── Digital Keys
      ├── Emergency Contacts
      ├── Location
      ├── Event History
      └── System Status
```

---

# 23. Kiến trúc tổng thể

```text
                         ┌─────────────────┐
                         │   MOBILE APP    │
                         │                 │
                         │ Auth            │
                         │ Digital Key     │
                         │ Users           │
                         │ Vehicle         │
                         │ Safety          │
                         └───────┬─────────┘
                                 │
                              BLE / Internet
                                 │
              ┌──────────────────┴──────────────────┐
              │                                     │
              ▼                                     ▼
     ┌──────────────────┐                  ┌──────────────────┐
     │ SMART SAFETY BOX │                  │     BACKEND      │
     │                  │                  │                  │
     │ ESP32            │                  │ Authentication   │
     │ BLE              │                  │ Database         │
     │ IMU              │                  │ Permissions      │
     │ GPS              │                  │ Events           │
     │ Buzzer           │                  │ Notification     │
     │ Access Control   │                  └──────────────────┘
     │ Black Box        │
     └────────┬─────────┘
              │
              ▼
          MOTORCYCLE
```

---

# 24. Các trạng thái của hệ thống

Có thể xây dựng State Machine:

```text
BOOT
 │
 ▼
INITIALIZING
 │
 ├── Unconfigured → PROVISIONING
 │
 └── Configured
        │
        ▼
     LOCKED
        │
        ├── Valid Digital Key → UNLOCKED
        │
        ├── Physical Key Allowed → UNLOCKED
        │
        └── Unauthorized Attempt → SECURITY ALERT
                                     │
                                     ▼
                              LOG EVENT / ALERT
```

Khi xe đang hoạt động:

```text
UNLOCKED / RIDING
       │
       ├── Normal Motion
       │
       ├── Abnormal Motion
       │       ↓
       │  Accident Analysis
       │
       └── Accident
               ↓
        Confirmation
               ↓
        Emergency Process
```

---

# 25. Chống trộm

Các lớp bảo vệ có thể gồm:

## Lớp 1 — Physical Security

Khóa cơ và phần cứng xe.

## Lớp 2 — BLE Authentication

Thiết bị phải xác thực Digital Key.

## Lớp 3 — UID / Permission

Chỉ người dùng được cấp quyền mới có thể truy cập.

## Lớp 4 — Key Revocation

OWNER có thể thu hồi Digital Key.

## Lớp 5 — Unauthorized Access Logging

Ghi nhận các lần truy cập không hợp lệ.

## Lớp 6 — Alert

Có thể gửi thông báo khi phát hiện hành vi đáng ngờ.

---

# 26. Tình huống sử dụng

## Tình huống 1 — Chủ xe

```text
Khang đến gần xe
      ↓
Phone phát hiện BLE
      ↓
Digital Key Authentication
      ↓
OWNER
      ↓
Xe được mở
```

## Tình huống 2 — Người thân

```text
Người thân đến xe
      ↓
Phone
      ↓
BLE
      ↓
USER / ADMIN
      ↓
Có quyền
      ↓
Xe được mở
```

## Tình huống 3 — Người thân không có điện thoại

```text
OWNER bật Physical Key Permission
      ↓
Người thân sử dụng chìa khóa
      ↓
Smart Safety Box cho phép
      ↓
Xe hoạt động
```

## Tình huống 4 — Điện thoại bị mất

```text
OWNER mở App từ thiết bị khác
      ↓
Digital Keys
      ↓
Revoke Lost Phone
      ↓
Digital Key bị vô hiệu hóa
```

## Tình huống 5 — Tai nạn

```text
IMU + GPS
      ↓
Accident Score
      ↓
Possible Accident
      ↓
Countdown
      ↓
Không hủy
      ↓
ADMIN
      ↓
SLAVE nếu ADMIN không phản hồi
```

---

# 27. Các bài toán nghiên cứu chính

Đề tài không chỉ tập trung vào số lượng tính năng mà tập trung vào các bài toán có thể đo lường.

## Bài toán 1 — Accident Detection

Làm sao phát hiện tai nạn mà không phụ thuộc vào một cảm biến duy nhất?

## Bài toán 2 — False Positive

Làm sao phân biệt tai nạn với:

- Phanh mạnh
- Ổ gà
- Rẽ
- Quay đầu
- Dắt xe
- Đỗ xe
- Xe bị nghiêng khi dựng?

## Bài toán 3 — Authentication

Làm sao sử dụng BLE làm Digital Key nhưng vẫn đảm bảo xác thực người dùng?

## Bài toán 4 — Access Control

Làm sao quản lý nhiều người dùng với các mức quyền khác nhau?

## Bài toán 5 — Emergency Response

Làm sao tự động liên lạc với người phù hợp khi người lái không phản hồi?

---

# 28. Phương pháp kiểm thử

Không thực hiện các thử nghiệm nguy hiểm với người đang điều khiển xe.

Có thể xây dựng các bài kiểm thử có kiểm soát:

| Trường hợp | Mục đích |
|---|---|
| Chạy bình thường | Kiểm tra False Positive |
| Phanh nhẹ | Kiểm tra thuật toán |
| Phanh mạnh | Kiểm tra ngưỡng |
| Ổ gà | Kiểm tra nhiễu |
| Rẽ | Kiểm tra Gyroscope |
| Quay đầu | Kiểm tra Orientation |
| Dắt xe | Kiểm tra Motion State |
| Dừng xe | Kiểm tra trạng thái |
| Xe đổ có kiểm soát | Kiểm tra Accident Detection |
| Va chạm mô phỏng an toàn | Kiểm tra Impact |
| Điện thoại hợp lệ | Kiểm tra Authentication |
| Điện thoại không hợp lệ | Kiểm tra Security |
| Thu hồi Digital Key | Kiểm tra Access Control |
| Không có điện thoại | Kiểm tra Physical Key |

---

# 29. Chỉ số đánh giá

## Accident Detection

- Detection Accuracy
- False Positive Rate
- False Negative Rate
- Precision
- Recall
- Detection Time

## Emergency

- Confirmation Time
- Notification Time
- Contact Response Time

## BLE Security

- Authentication Success Rate
- Unauthorized Access Rejection Rate
- Key Revocation Success

## System

- BLE Connection Time
- Power Consumption
- GPS Acquisition Time
- System Reliability

---

# 30. Điểm mới của đề tài

Các điểm có thể nhấn mạnh:

### 1. Không phụ thuộc vào mũ bảo hiểm

Thiết bị hoạt động độc lập, không cần sửa đổi mũ.

### 2. Multi-factor Accident Detection

Không sử dụng riêng góc nghiêng mà kết hợp:

- Gia tốc
- Vận tốc góc
- Tư thế
- Vận tốc
- Trạng thái chuyển động
- Dữ liệu GPS

### 3. Smartphone Digital Key

Sử dụng điện thoại như một chìa khóa số thông qua BLE.

### 4. Role-Based Access Control

Quản lý nhiều người dùng theo UID và quyền.

### 5. Physical Key Fallback

Cho phép sử dụng chìa khóa vật lý theo quyền được cấp.

### 6. Emergency Contact Hierarchy

Có ADMIN và SLAVE, cho phép xây dựng quy trình liên lạc dự phòng.

### 7. Event Black Box

Lưu dữ liệu trước và sau sự kiện để nghiên cứu và đánh giá thuật toán.

---

# 31. Công nghệ dự kiến

## Hardware

- ESP32 / ESP32-S3
- IMU
- GPS
- Buzzer
- Buttons
- Memory
- Power Management
- Vehicle Interface

## Firmware

- C/C++
- ESP-IDF hoặc Arduino Framework
- BLE
- Sensor Fusion
- State Machine
- Accident Detection
- Data Logging
- OTA

## Mobile

### Lựa chọn cho phiên bản đầu tiên

Phiên bản đầu tiên ưu tiên **Android app**. Có thể chọn một trong hai hướng sau:

- **Flutter:** lựa chọn khuyến nghị cho vibe coding vì cấu trúc giao diện tương đối thống nhất và dễ mở rộng sang iOS trong tương lai nếu cần.
- **Android Native / Kotlin:** lựa chọn phù hợp nếu muốn tối ưu sâu cho Android và BLE.

React Native chưa cần đưa vào MVP để tránh tăng số lượng công nghệ phải học và phải xử lý.

### Nguyên tắc vibe coding

Để giảm rủi ro khi phát triển bằng AI, mỗi lần yêu cầu AI chỉ nên tập trung vào một tính năng nhỏ:

1. Mô tả rõ mục tiêu, màn hình, dữ liệu đầu vào và kết quả mong muốn.
2. Yêu cầu AI giải thích các file sẽ thay đổi trước khi viết mã.
3. Chạy thử ngay trên Android emulator hoặc điện thoại thật sau mỗi thay đổi.
4. Lưu lỗi, log BLE và ảnh chụp màn hình khi kiểm thử để AI có đủ ngữ cảnh sửa lỗi.
5. Không yêu cầu AI viết toàn bộ ứng dụng trong một lần; ưu tiên hoàn thành từng màn hình và từng luồng.

Thứ tự triển khai app Android nên là: màn hình kết nối BLE → đọc trạng thái thiết bị → đăng nhập/tài khoản → Digital Key → mở khóa có xác thực → quản lý người dùng → cảnh báo và lịch sử sự kiện.

## Backend

Giai đoạn đầu:

- Firebase Authentication
- Firestore / Realtime Database
- Firebase Cloud Messaging

Giai đoạn mở rộng:

- FastAPI
- PostgreSQL
- REST API
- WebSocket

---

# 32. Lộ trình phát triển

## Phase 1 — Hardware

- ESP32
- BLE
- IMU
- GPS
- Power

## Phase 2 — Firmware

- Sensor reading
- BLE communication
- State Machine
- Data logging

## Phase 3 — Android App (MVP)

- Login
- Vehicle
- BLE pairing
- Digital Key
- Unlock
- User management
- Kiểm thử trên Android emulator và điện thoại Android thật
- Hoàn thiện các luồng chính bằng vibe coding có kiểm thử

## Phase 4 — Security

- Challenge-response
- Digital Key
- UID
- Permission
- Key revocation

## Phase 5 — Accident Detection

- Sensor fusion
- Accident Score
- Threshold calibration
- False-positive testing

## Phase 6 — Emergency

- GPS
- Countdown
- Emergency contacts
- Notification
- ADMIN / SLAVE fallback

## Phase 7 — Black Box

- Event recording
- Data export
- Analysis

## Phase 8 — Evaluation

- Test dataset
- Metrics
- Comparison
- Optimization

---

# 33. Hướng phát triển tương lai

Có thể mở rộng:

- iOS sau khi Android app ổn định
- Smartwatch
- NFC backup key
- UWB proximity
- Geofencing
- Anti-theft alarm
- Remote vehicle status
- OTA firmware
- Web dashboard sau giai đoạn Android app và backend ổn định
- Cloud analytics
- Machine Learning cho accident detection
- AI phân loại tình huống
- Fleet management

---



# 33A. Hàng rào ảo (Geofencing)

Hệ thống có thể bổ sung **hàng rào ảo (Geofence)** để giám sát khu vực hoạt động của xe và tăng cường khả năng chống trộm. Chức năng này không chỉ đơn thuần cảnh báo khi xe ra khỏi vùng mà còn có thể kết hợp với trạng thái khóa xe, GPS và Digital Key để đánh giá hành vi đáng ngờ.

## 33A.1. Khái niệm

OWNER có thể tạo một khu vực an toàn trên Mobile App bằng cách chọn vị trí trung tâm và bán kính. Ví dụ:

```text
             HÀNG RÀO ẢO
        ┌───────────────────┐
        │                   │
        │      🏠 Nhà       │
        │                   │
        │        🏍️         │
        │                   │
        └───────────────────┘
              300 mét
```

Các thông tin cấu hình có thể gồm:

- Tên Geofence
- Latitude / Longitude của tâm
- Bán kính
- Trạng thái Enable / Disable
- Chế độ cảnh báo
- Người nhận cảnh báo
- Điều kiện kích hoạt

## 33A.2. Các chế độ Geofence

### Normal Geofence

Xe được phép rời khỏi vùng nhưng hệ thống gửi thông báo cho người dùng:

```text
⚠️ Xe đã rời khỏi khu vực an toàn.
```

Phù hợp để theo dõi xe hoặc người thân.

### Anti-theft Geofence

Khi xe đang ở trạng thái `LOCKED` mà GPS phát hiện xe rời khỏi khu vực an toàn, hệ thống chuyển sang trạng thái nghi ngờ trộm:

```text
LOCKED
   ↓
GPS Position
   ↓
Outside Geofence?
   ↓
YES
   ↓
🚨 POSSIBLE THEFT
   ↓
Buzzer / Alert
   ↓
Mobile App
```

### Family / Restricted Zone

OWNER có thể thiết lập khu vực được phép cho người thân. Khi xe ra khỏi khu vực, ADMIN hoặc các liên hệ được cấp quyền có thể nhận cảnh báo.

## 33A.3. Không quyết định chỉ bằng một mẫu GPS

GPS có sai số và vị trí có thể dao động quanh biên Geofence. Vì vậy không nên thực hiện:

```text
GPS > Radius → Alert
```

chỉ dựa trên một mẫu dữ liệu. Hệ thống nên xác nhận trạng thái bằng nhiều mẫu GPS liên tiếp và có vùng đệm.

Ví dụ sử dụng hysteresis:

```text
Geofence radius = 500 m

< 500 m       → INSIDE
500–530 m     → BOUNDARY
> 530 m       → OUTSIDE
```

Cách này giúp giảm cảnh báo giả do sai số GPS.

## 33A.4. State Machine của Geofence

```text
INSIDE
   │
   │ GPS samples
   ▼
BOUNDARY
   │
   │ Xác nhận nhiều mẫu
   ▼
OUTSIDE
   │
   ├── Alert
   ├── Event Log
   └── Tracking
```

Khi xe quay lại vùng an toàn, hệ thống có thể ghi nhận sự kiện `GEOFENCE_ENTERED` và gửi thông báo nếu người dùng bật tùy chọn này.

## 33A.5. Kết hợp Geofence với Security Engine

Geofence nên được tích hợp với các thành phần bảo mật hiện có thay vì hoạt động độc lập:

```text
                    SMART SAFETY BOX
                           │
          ┌────────────────┼────────────────┐
          ▼                ▼                ▼
       GPS Zone        Vehicle State     Digital Key
          │                │                │
          ▼                ▼                ▼
       INSIDE/OUTSIDE    LOCKED/RIDING    Authorized?
          │                │                │
          └────────────────┼────────────────┘
                           ▼
                    SECURITY ENGINE
                           │
             ┌─────────────┴─────────────┐
             ▼                           ▼
        NORMAL EVENT                SUSPICIOUS
                                         │
                                         ▼
                                  🚨 ALERT OWNER
```

Ví dụ: xe đang khóa tại nhà, không có Digital Key hợp lệ nhưng vị trí bắt đầu di chuyển ra khỏi Geofence. Hệ thống có thể tạo sự kiện nghi ngờ trộm và gửi cảnh báo cho OWNER.

```text
🚨 CẢNH BÁO CHỐNG TRỘM

Xe đang rời khỏi khu vực an toàn.
Trạng thái: Locked
Digital Key: Không xác thực
Vị trí: GPS hiện tại
```

## 33A.6. Dữ liệu Geofence

Có thể lưu trong Backend:

```text
geofenceId
vehicleId
name
centerLatitude
centerLongitude
radius
boundaryBuffer
mode
enabled
alertOnExit
alertOnEnter
createdBy
createdAt
```

Event History có thể ghi:

```text
GEOFENCE_ENTERED
GEOFENCE_EXITED
GEOFENCE_BOUNDARY
POSSIBLE_THEFT
```

## 33A.7. Giao diện Mobile App

Dashboard có thể bổ sung mục:

```text
🗺️ Geofence

Home Zone
● Active

Radius: 500 m
Mode: Anti-theft

[ Edit ]   [ Disable ]

-------------------------

[ + Add Geofence ]
```

Màn hình cấu hình cho phép OWNER:

- Chọn vị trí trên bản đồ
- Thiết lập bán kính
- Đặt tên khu vực
- Chọn chế độ
- Bật/tắt cảnh báo khi ra khỏi vùng
- Bật/tắt cảnh báo khi quay lại vùng
- Chọn người nhận cảnh báo

## 33A.8. Vai trò của Geofence trong đề tài

Geofencing mở rộng hệ thống từ một thiết bị xác thực và an toàn thành một nền tảng có khả năng **giám sát không gian hoạt động của xe**. Khi kết hợp:

- GPS
- Vehicle State
- Digital Key
- Permission
- Geofence
- Event Logging

hệ thống có thể đánh giá tình huống theo nhiều yếu tố thay vì chỉ dựa vào một cảm biến hoặc một điều kiện đơn lẻ.

Đây cũng là một thành phần phù hợp để phát triển thành **Security Engine** của Smart Safety Box.

# 34. Định hướng nghiên cứu

Đề tài nên được trình bày theo hướng:

> **Không chỉ tạo một thiết bị mở khóa xe bằng Bluetooth, mà nghiên cứu một hệ thống tích hợp giữa xác thực người dùng, kiểm soát truy cập, phát hiện bất thường và hỗ trợ cứu hộ khẩn cấp.**

Trọng tâm nghiên cứu:

```text
             SMART SAFETY BOX
                    │
        ┌───────────┼───────────┐
        │           │           │
        ▼           ▼           ▼
   SECURITY      SAFETY      RESCUE
        │           │           │
      BLE         IMU/GPS    Emergency
   Digital Key    Detection   Response
        │           │           │
        └───────────┼───────────┘
                    ▼
               MOBILE APP
```

---

# 35. Tên đề tài đề xuất

### Tên tiếng Việt

**Nghiên cứu và phát triển hệ thống hỗ trợ an toàn, xác thực người dùng và cứu hộ khẩn cấp cho xe máy dựa trên Bluetooth, cảm biến quán tính và định vị GPS**

### Tên ngắn

**SMART SAFETY BOX – Hệ thống an toàn thông minh cho xe máy**

### Tên tiếng Anh

**Research and Development of an Intelligent Motorcycle Safety, User Authentication and Emergency Response System Based on Bluetooth, Inertial Sensors and GPS**

---

# 36. Tóm tắt ý tưởng

```text
                    SMARTPHONE
                        │
                 Bluetooth / BLE
                        │
                        ▼
              ┌──────────────────┐
              │ SMART SAFETY BOX │
              │                  │
              │ Digital Key      │
              │ Authentication   │
              │ Access Control   │
              │ IMU              │
              │ GPS              │
              │ Accident Detect  │
              │ Black Box        │
              └────────┬─────────┘
                       │
             ┌─────────┴─────────┐
             ▼                   ▼
        VEHICLE ACCESS      SAFETY / RESCUE
             │                   │
        Digital Key          Accident Score
        Physical Key         GPS
        Permissions          Emergency
             │                   │
             └─────────┬─────────┘
                       ▼
                  MOBILE APP
```

**Ý tưởng cốt lõi:** biến điện thoại thành chìa khóa số có xác thực, đồng thời biến Smart Safety Box thành một trung tâm an toàn và bảo mật độc lập cho xe máy.
