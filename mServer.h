#ifndef mServer_h
#define mServer_h

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "LittleFS.h"
#endif
#include <ESPAsyncWebServer.h>

class mServer {
  public:
    void begin();
    void index();
    String processor(const String path);
};

#endif
