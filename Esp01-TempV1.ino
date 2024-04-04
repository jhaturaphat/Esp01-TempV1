#include <Arduino.h>
#include "WiFiManager.h"
#include "mServer.h"

// สร้างออปเจ็กต์ WiFiManager
WiFiManager wifiManager;
//สร้างออบเจ็กต์ mServer
mServer websrv;
void setup() {
  Serial.begin(115200);
  
  // ดึง Chip ID ของ ESP8266
  String chipID = String(ESP.getChipId());
  
  // สร้าง SSID ขึ้นมาจาก Chip ID
  String apSsid = "ESP_" + chipID;
  
  // กำหนดรหัส WiFi สำหรับโหมด Access Point
  const char* apPassword = "password";
  
  // เริ่มโหมด Access Point
  wifiManager.startAP(apSsid.c_str(), apPassword);

  websrv.begin();
}

void loop() {
  // โค้ดใน loop ที่คุณต้องการ
}
