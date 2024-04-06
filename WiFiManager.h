#ifndef WiFiManager_h
#define WiFiManager_h

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

class WiFiManager {
  public:
    void startAP(const char* ssid, const char* password);  
    static void handleNetConfig(AsyncWebServerRequest *request);
};

#endif
