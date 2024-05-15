#ifndef DevConfig_h
#define DevConfig_h

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

#include "LittleFS.h"
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

  class DevConfig {
    public:
      static void handleAlarmCfg(AsyncWebServerRequest *request);
      static bool handleNtp(AsyncWebServerRequest *request);
      static void handleAlarmRange(AsyncWebServerRequest *request);
      static void handleLineConf(AsyncWebServerRequest *request);
  };

#endif