#ifndef WiFiManager_h
#define WiFiManager_h

#include <ESP8266WiFi.h>

class WiFiManager {
  public:
    void startAP(const char* ssid, const char* password);
    
};

#endif
