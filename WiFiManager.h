#ifndef WiFiManager_h
#define WiFiManager_h

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
// #include <FS.h> // ต้องเพิ่มไลบรารี FS
#include "LittleFS.h"
#include <ArduinoJson.h> // ต้องเพิ่มไลบรารี ArduinoJson

class WiFiManager {
  public:
    void startAP(const char* ssid, const char* password);  
    static void handleWlanConfig(AsyncWebServerRequest *request);
    bool WlanSetup();

};

#endif
