#include "mbedtls/md.h"
#include <NTPClient.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// ================= WIFI =================
const char *ssid = "YOUR_WIFI";
const char *password = "YOUR_PASSWORD";

// ================= SECRET KEY (UPPERCASE, NO SPACES) =================
String secretBase32 = "YOUR_SECRET";

// ================= OLED =================
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// ================= NTP (UTC TIME) =================
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

// ================= BASE32 DECODE =================
const char *base32Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

int base32Decode(const char *encoded, uint8_t *result, int bufSize) {
  int buffer = 0, bitsLeft = 0, count = 0;

  for (int i = 0; encoded[i] != '\0'; i++) {
    char ch = toupper(encoded[i]);
    if (ch == '=')
      break;

    const char *ptr = strchr(base32Chars, ch);
    if (!ptr)
      continue;

    buffer <<= 5;
    buffer |= (ptr - base32Chars);
    bitsLeft += 5;

    if (bitsLeft >= 8) {
      result[count++] = (buffer >> (bitsLeft - 8)) & 0xFF;
      bitsLeft -= 8;

      if (count >= bufSize)
        break;
    }
  }
  return count;
}

// ================= TOTP =================
String generateTOTP() {
  uint8_t key[64];
  int keyLength = base32Decode(secretBase32.c_str(), key, sizeof(key));

  unsigned long currentTime = timeClient.getEpochTime();
  unsigned long timestep = currentTime / 30;

  uint8_t msg[8];
  for (int i = 7; i >= 0; i--) {
    msg[i] = timestep & 0xFF;
    timestep >>= 8;
  }

  uint8_t hash[20];
  const mbedtls_md_info_t *md = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);

  mbedtls_md_hmac(md, key, keyLength, msg, 8, hash);

  int offset = hash[19] & 0x0F;
  int binary = ((hash[offset] & 0x7F) << 24) |
               ((hash[offset + 1] & 0xFF) << 16) |
               ((hash[offset + 2] & 0xFF) << 8) | (hash[offset + 3] & 0xFF);

  int otp = binary % 1000000;

  char result[7];
  sprintf(result, "%06d", otp);

  return String(result);
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  u8g2.begin();

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(5, 30, "Connecting WiFi...");
  u8g2.sendBuffer();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  timeClient.begin();
}

// ================= LOOP =================
void loop() {
  // Auto-updates only when the 60s interval has passed, non-blocking
  timeClient.update();

  static int lastSecond = -1;
  int currentSecond = timeClient.getSeconds();

  // Only refresh the OLED and recalculate when the exact second changes
  if (currentSecond != lastSecond) {
    lastSecond = currentSecond;

    String otp = generateTOTP();
    int remaining = 30 - (currentSecond % 30);

    // Debug (optional)
    Serial.print("OTP: ");
    Serial.println(otp);

    u8g2.clearBuffer();

    // BIG OTP
    u8g2.setFont(u8g2_font_logisoso32_tr);
    u8g2.drawStr(5, 45, otp.c_str());

    // Timer
    u8g2.setFont(u8g2_font_6x10_tr);
    char timerStr[20];
    sprintf(timerStr, "Expires in: %ds", remaining);
    u8g2.drawStr(5, 62, timerStr);

    u8g2.sendBuffer();
  }

  // Small delay to yield to background WiFi tasks
  delay(10);
}
