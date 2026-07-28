# 🔐 ESP32 TOTP Authenticator with OLED

This project generates Time-Based One-Time Passwords (TOTP) using an ESP32 and displays them on a 0.96" OLED screen.

## 📦 Components Used

* ESP32
* 0.96" OLED Display (SH1106 / I2C)
* TP4056 Charging Module
* 18650 Li-ion Battery (Single Cell)
* Battery Holder

## ⚙️ Features

* Generates 6-digit TOTP codes (like Google Authenticator)
* Syncs time using NTP
* Displays OTP + countdown timer
* Works fully offline after sync

## 🔌 Wiring

### OLED → ESP32

* VCC → 3.3V
* GND → GND
* SDA → GPIO 21
* SCL → GPIO 22

## 🔋 Power Setup

Battery → TP4056 → ESP32 (via 5V or 3.3V regulator)


## 🚀 Future Improvements

* Add buttons to switch accounts
* Add battery level indicator
* Store multiple secrets



Made with ❤️ using ESP32
