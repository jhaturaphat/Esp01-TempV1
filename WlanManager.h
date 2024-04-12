#ifndef WlanManager_h
#define WlanManager_h

#ifdef ESP32
#include <WiFi.h>
// #include <ESP32WiFi.h>   // สำหรับ ESP32  มัน Error ถ้าเอา comment ออก
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#elif defined(ESP8266)  // สำหรับ ESP8266
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#endif
#include <ESPAsyncWebServer.h>
// #include <FS.h> // ต้องเพิ่มไลบรารี FS
#include "LittleFS.h"
#include <AsyncJson.h>
#include <ArduinoJson.h> // ต้องเพิ่มไลบรารี ArduinoJson

class WlanManager {
  public:
    void startAP(const char* ssid, const char* password);  
    static void handleWlanConfig(AsyncWebServerRequest *request);
    bool WlanSetup();
    void updateWlanConfig(const char* fixIP);
    void updateWlanConfig(const char* newSSID, const char* newPassword);
    String chipID();
    String wifichk = "true";
    bool WifiHealthy();
};

#endif

/*
#ifdef ESP32
#include <ESP32WiFi.h>   // สำหรับ ESP32
#elif defined(ESP8266)
#include "coredecls.h"
#include <ESP8266WiFi.h> // สำหรับ ESP8266
#endif*/
