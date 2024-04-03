#include "WiFiManager.h"

void WiFiManager::startAP(const char* ssid, const char* password) {
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
}
