# Smart ATM Mini (Arduino Project)

### 📌 Project Information
- **Project Name**: Smart ATM Mini
- **Course**: IOT102 – IA1901
- **Group**: 1
- **Team Members**:
  - Thiều Xuân Giang (HE181920)
  - Phạm Tuấn Anh (HE181780)
  - Phạm Văn Tân Thành (HE182364)
  - Lưu Đình Tiến (HE180052)

---

## 🎯 Project Idea
Mô phỏng hệ thống máy ATM mini sử dụng Arduino. Hệ thống cho phép người dùng:
- Đăng nhập bằng ID & mật khẩu
- Kiểm tra số dư, nạp tiền, rút tiền, chuyển khoản
- Cảnh báo bằng LED & buzzer khi nhập sai mật khẩu
- Hiển thị thông tin qua màn hình LCD

---

## 🛠️ Features Implemented
- Xác thực người dùng qua ID và mật khẩu
- Hiển thị thông tin trên LCD 16x2
- Điều khiển LED xanh/đỏ theo trạng thái
- Âm thanh cảnh báo qua buzzer
- Điều khiển quạt theo cảm biến nhiệt độ (LM35)
- Giao tiếp Bluetooth HC-06 để điều khiển từ xa

---

## ⚙️ Technologies & Hardware Used
| Thiết bị / Module | Mô tả |
|-------------------|-------|
| Arduino Uno R3    | Vi điều khiển chính |
| LCD 16x2          | Hiển thị thông tin đăng nhập và trạng thái |
| Keypad 4x4        | Nhập ID và mật khẩu |
| LED đỏ/xanh       | Báo trạng thái đăng nhập |
| Buzzer            | Cảnh báo khi nhập sai |
| Cảm biến nhiệt LM35 | Kiểm tra nhiệt độ môi trường |
| Quạt 5V           | Làm mát tự động khi nhiệt độ cao |
| Bluetooth HC-06   | Giao tiếp với thiết bị di động |

---

## 🧪 Functional Flow
- Người dùng nhập ID → nếu tồn tại → nhập mật khẩu
- Nếu đúng → hiển thị chào mừng + LED xanh + âm báo
- Nếu sai → báo lỗi + LED đỏ + âm báo (tăng dần)
- Sai 3 lần → khóa thẻ, báo động dài

---

## 📊 Sensor & Control Logic
- Nhiệt độ > 30°C → bật quạt
- Nhấn `*` → hiển thị nhiệt độ trên LCD
- Nhấn `C` → thoát khỏi chế độ nhiệt độ

---

## 🔐 Account Simulation
- Có 4 tài khoản mẫu (`1111`, `2222`, `3333`, `4444`)
- Gán tên và số dư cụ thể từng tài khoản
- Cho phép nạp/rút/chuyển tiền giữa các tài khoản

---

## 🔁 Future Development
- Tích hợp RFID thay cho nhập ID
- Giao tiếp WiFi điều khiển từ xa
- Kết nối cơ sở dữ liệu để mô phỏng thực tế

---

## 💡 Use Cases
- Bài tập thực hành IOT, hệ thống nhúng
- Mô phỏng hệ thống bảo mật đơn giản
- Dạy học về xác thực, cảm biến và hiển thị

---

## 📜 License
This project is for academic and educational purposes.

