#ifndef Router_h
#define Router_h

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "LittleFS.h"
#include <ESPAsyncWebServer.h>
#include "DevConfig.h"


class Router {
  public:
    void begin();
    void start();
    String processor(const String path);    
    void setTemperature(float temp1, float temp2);
    void notFound(AsyncWebServerRequest *request);
    bool handleNtp(AsyncWebServerRequest *request);
    void setScanNetwork(String ssid);
    void handleAlarmCfg(AsyncWebServerRequest *request);
    void handleRange(AsyncWebServerRequest *request);

  private:
    float temperature1 = 0.0;
    float temperature2 = 0.0;   
    String listSsid = "{}"; 
};

#endif
