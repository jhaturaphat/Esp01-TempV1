
#include <Arduino.h>
#include "WlanManager.h"
#include "Router.h"

// สร้างออปเจ็กต์ WlanManager
WlanManager wlan;
//สร้างออบเจ็กต์ mServer
Router router;
void setup() {
  Serial.begin(115200);

  // สร้าง SSID ขึ้นมาจาก Chip ID
  String apSsid = "ESP_" + wlan.chipID();
  
  // กำหนดรหัส WiFi สำหรับโหมด Access Point
  const char* apPassword = "password";
  
  // เริ่มโหมด Access Point
  wlan.startAP(apSsid.c_str(), apPassword);
  
  router.begin();
}

void loop() {
  // โค้ดใน loop ที่คุณต้องการ
}
