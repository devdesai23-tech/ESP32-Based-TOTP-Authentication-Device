# ESP32-TOTP-Authenticator-Google-Auth
ESP32 based TOTP (Google Authenticator) using OLED display and NTP time sync
# 🔐 ESP32 TOTP Authenticator (Google Auth Compatible)

## 📌 Overview
This project is a hardware-based Time-Based One-Time Password (TOTP) generator using an ESP32 and a 0.96" OLED display.  
It works similarly to Google Authenticator by generating a new 6-digit OTP every 30 seconds.

---

## 🚀 Features
- Generates secure 6-digit TOTP codes
- Uses NTP for accurate time synchronization
- Displays OTP on OLED screen (SH1106)
- Countdown timer for OTP expiration
- Fully offline after time sync
- Compact and portable design

---

## 🧰 Components Required
- ESP32 Development Board
- 0.96" OLED Display (SH1106, I2C)
- TP4056 Charging Module (with protection)
- 18650 Li-ion Battery (single cell)
- Battery Holder
- Jumper wires

---

## 🔌 Wiring Diagram

### OLED → ESP32
- VCC → 3.3V
- GND → GND
- SDA → GPIO 21
- SCL → GPIO 22

### Power Setup
- Battery → TP4056 (B+ / B-)
- TP4056 OUT → Boost Converter → 5V → ESP32

⚠️ Do NOT power ESP32 directly from battery (unstable voltage)

---

## ⚙️ Software Requirements
- Arduino IDE / PlatformIO
- Libraries:
  - WiFi (built-in)
  - NTPClient
  - U8g2
  - mbedTLS (built-in)

---

## 📂 Project Structure
ESP32-TOTP-Authenticator/
│── src/
│   ├── main.ino
│   └── config.h (ignored)
│── .gitignore
│── README.md

---

## 🔐 Security Setup

Create a file named `config.h` inside `src/`:

```cpp
#ifndef CONFIG_H
#define CONFIG_H

const char *ssid = "YOUR_WIFI";
const char *password = "YOUR_PASSWORD";
String secretBase32 = "YOUR_SECRET_KEY";

#endif
```

⚠️ This file is ignored using `.gitignore` to protect sensitive data.

---

## ▶️ How to Use

1. Install required libraries
2. Add your WiFi and secret key in `config.h`
3. Upload code to ESP32
4. Connect OLED display
5. Power the device
6. View OTP on screen

---

## 📸 Output
- Large 6-digit OTP displayed
- Countdown timer (30 seconds)

---

## 💡 Future Improvements
- Multiple account support
- Button navigation
- Battery level indicator
- Enclosure design

---

## ⚠️ Disclaimer
Do not upload your real WiFi credentials or secret keys to GitHub.

---

## ❤️ Author
Made with ESP32 by Dev
